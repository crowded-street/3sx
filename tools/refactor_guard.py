#!/usr/bin/env python3
"""Mechanically check that a refactor did not change behaviour-carrying tokens.

The refactoring campaign (docs/refactoring/) allows only structural changes:
extracting functions, adding guard clauses, naming predicates. None of those
may alter a numeric literal, a string, or a character constant.

This script extracts every literal from a file before and after a change and
compares the multisets. If a constant appeared, vanished, or changed value,
the refactor is not behaviour-preserving and must be reverted.

It is deliberately dumb and has no opinion about style. It answers exactly one
question: did any constant change?

Usage:
    python tools/refactor_guard.py src/path/file.c            # working tree vs HEAD
    python tools/refactor_guard.py --base main src/path/file.c
    python tools/refactor_guard.py --all                      # every changed .c/.cpp
    python tools/refactor_guard.py --combined --base HEAD src/path/file.c src/path/new.c

Exit codes:
    0  no literal changed
    1  a literal changed, or the file could not be compared
"""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
from collections import Counter
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent

# Order matters. Strings and chars are consumed first so digits inside them are
# not read as numbers. Identifiers are consumed next and discarded: this
# codebase's type names (s8, u16, s32) end in digits, and without this the
# scanner reports the "16" in `s16` as a numeric literal.
TOKEN_RE = re.compile(
    r"""
      (?P<string>"(?:[^"\\\n]|\\.)*")
    | (?P<char>'(?:[^'\\\n]|\\.)*')
    | (?P<ident>[A-Za-z_][A-Za-z_0-9]*)
    | (?P<number>
          0[xX][0-9a-fA-F]+[uUlL]*
        | 0[bB][01]+[uUlL]*
        | (?:\d+\.\d*|\.\d+|\d+)(?:[eE][+-]?\d+)?[fFuUlL]*
      )
    """,
    re.VERBOSE,
)

COMMENT_RE = re.compile(
    r"""
      (?P<keep>"(?:[^"\\\n]|\\.)*" | '(?:[^'\\\n]|\\.)*')
    | (?P<block>/\*.*?\*/)
    | (?P<line>//[^\n]*)
    """,
    re.VERBOSE | re.DOTALL,
)


def strip_comments(src: str) -> str:
    def repl(m: re.Match) -> str:
        if m.group("keep"):
            return m.group("keep")
        return " "
    return COMMENT_RE.sub(repl, src)


def strip_numeric_suffix(tok: str) -> str:
    """Drop a C integer/float suffix without eating the literal's own digits.

    `f` and `F` are hex digits as well as the float suffix, so stripping them
    from a hex literal silently rewrites its value: `0x1FF` becomes `0x1` and
    normalises to 1, which is indistinguishable from the literal `1`. A
    substituted constant hiding behind that collision would pass the guard.
    """
    suffix = "uUlL" if tok[:2].lower() in ("0x", "0b") else "uUlLfF"
    return tok.rstrip(suffix)


def normalise_number(tok: str) -> str:
    """Fold away suffix/format differences that do not change the value."""
    body = strip_numeric_suffix(tok)
    try:
        base = integer_base(body)
        if base is not None:
            return str(int(body, base))
        if is_float_literal(body):
            return repr(float(body))
        return str(int(body))
    except ValueError:
        return tok


def integer_base(body: str) -> int | None:
    prefix = body[:2].lower()
    if prefix == "0x":
        return 16
    if prefix == "0b":
        return 2
    # Leading zeros are octal in C; preserve that meaning.
    if is_octal_literal(body):
        return 8
    return None


def is_octal_literal(body: str) -> bool:
    if len(body) <= 1:
        return False
    if body[0] != "0":
        return False
    return body.isdigit()


def is_float_literal(body: str) -> bool:
    return any(char in body for char in ".eE")


def literals(src: str) -> Counter:
    out = Counter()
    for m in TOKEN_RE.finditer(strip_comments(src)):
        if m.group("string"):
            out["str " + m.group("string")] += 1
        elif m.group("char"):
            out["chr " + m.group("char")] += 1
        elif m.group("ident"):
            continue  # consumed so trailing digits are not read as literals
        else:
            out["num " + normalise_number(m.group("number"))] += 1
    return out


#: Control-flow keywords that take a parenthesis and would otherwise scan as calls.
NOT_CALLS = frozenset(
    ("if", "for", "while", "switch", "return", "sizeof", "do", "else", "case", "defined")
)

CALL_RE = re.compile(r"\b([A-Za-z_][A-Za-z_0-9]*)\s*\(")

#: An identifier that is *not* followed by "(" - how a function reads once it is
#: passed by pointer rather than called. Only names declared with --fnptr are
#: looked up here.
BARE_NAME_RE = re.compile(r"\b([A-Za-z_][A-Za-z_0-9]*)\b(?!\s*\()")


def calls(src: str, fnptrs: frozenset = frozenset()) -> Counter:
    """Multiset of called function names, ignoring definitions and declarations.

    This catches the failure mode a literal fingerprint misses: a call dropped
    or duplicated inside a branch that was rewritten, with every constant still
    in place.

    A definition looks like a call to this scanner, which is deliberate - it
    makes the two legitimate shapes easy to recognise. Measured against real
    campaign commits:

      Recipe E (extract)      nothing removed; each new helper +2
                              (its definition, and the one call that replaced
                              the block)
      Recipe C/D (dedup)      the shared callees drop by the number of copies
                              removed; the helper is +1 per call site, +1 for
                              its definition

    Anything else wants explaining. A name that disappears entirely is a FAIL.

    `fnptrs` names functions the agent has declared are now passed by pointer
    (Recipe F). A name handed to a helper as `f` rather than written `f(...)`
    stops matching CALL_RE, so the fingerprint would read it as a vanished
    call; each bare mention of a declared name counts as one call instead. The
    declaration is the agent's, not a guess, so the counts still have to
    balance afterwards - the check keeps its strength.
    """
    cleaned = strip_preprocessor_lines(strip_comments(src))
    out = Counter()
    for m in CALL_RE.finditer(cleaned):
        name = m.group(1)
        if name in NOT_CALLS:
            continue
        out[name] += 1
    if fnptrs:
        for m in BARE_NAME_RE.finditer(cleaned):
            name = m.group(1)
            if name in fnptrs:
                out[name] += 1
    return out


def strip_preprocessor_lines(src: str) -> str:
    """Drop #define, #include and friends before counting calls.

    A function-like macro's definition line - `#define LO_2_BYTES(_val) ...` -
    reads as a call to the name it defines, so moving a macro into a header made
    the call fingerprint say a call had vanished. A macro *definition* is not a
    call; its *uses* still are, and those are counted as before.
    """
    return re.sub(r"^[ \t]*#[^\n]*(?:\\\n[^\n]*)*", "", src, flags=re.MULTILINE)


def report_call_changes(rel: str, before: Counter, after: Counter, strict: bool) -> bool:
    removed = before - after
    added = after - before
    if not removed and not added:
        print("OK    " + rel + "  (" + str(sum(before.values())) + " call sites unchanged)")
        return True

    vanished = [name for name in removed if after[name] == 0]
    if vanished:
        print("FAIL  " + rel + "  - a call vanished from the file")
        for name in sorted(vanished):
            print("        gone      " + name + " (was called " + str(before[name]) + "x)")
        if any(after[name] == 0 for name in vanished) and (after - before):
            print("        if one of these was renamed - legal for a file-local static, never")
            print("        for a function another file can see - declare it and run again:")
            print("          --renamed OLD=NEW")
        return False

    label = "FAIL " if strict else "WARN "
    print(label + " " + rel + "  - call counts moved")
    for name, count in sorted(removed.items()):
        print("        -" + str(count) + "  " + name)
    for name, count in sorted(added.items()):
        tag = "  <-- new name" if before[name] == 0 else ""
        print("        +" + str(count) + "  " + name + tag)
    print("        expected: extract removes nothing and adds each helper twice (its")
    print("        definition and its call); deduplicate drops the shared callees by the")
    print("        copies removed. Anything else wants explaining.")
    return not strict


def apply_renames(counts: Counter, renames: dict) -> Counter:
    """Rewrite old names to new ones in a call fingerprint.

    A renamed file-local static would otherwise read as a vanished call. The
    rename is declared by the agent rather than guessed, so the check keeps its
    strength: counts still have to balance after the substitution.
    """
    if not renames:
        return counts
    out = Counter()
    for name, count in counts.items():
        out[renames.get(name, name)] += count
    return out


def check_calls(rel: str, base: str, strict: bool = False, renames: dict | None = None,
                fnptrs: frozenset = frozenset()) -> bool:
    """Compare the call fingerprint of one file against `base`."""
    before = git_show(base, rel)
    if before is None:
        print("SKIP  " + rel + "  (not in " + base + "; new file)")
        return True

    path = REPO / rel
    if not path.is_file():
        print("FAIL  " + rel + "  (deleted from working tree)")
        return False

    after = path.read_text(encoding="utf-8", errors="replace")
    return report_call_changes(rel, apply_renames(calls(before), renames or {}),
                               calls(after, fnptrs), strict)


def strip_include_lines(src: str) -> str:
    return re.sub(r"^\s*#\s*include[^\n]*\n", "", src, flags=re.MULTILINE)


def git_show(ref: str, rel: str) -> str | None:
    proc = subprocess.run(
        ["git", "show", ref + ":" + rel],
        cwd=REPO, capture_output=True, text=True, encoding="utf-8", errors="replace",
    )
    return proc.stdout if proc.returncode == 0 else None


def changed_files(base: str) -> list[str]:
    proc = subprocess.run(
        ["git", "diff", "--name-only", base, "--", "*.c", "*.cpp"],
        cwd=REPO, capture_output=True, text=True,
    )
    return [p.strip() for p in proc.stdout.splitlines() if p.strip()]


def dump_removed(after: Counter, removed: Counter) -> None:
    vanished = {literal for literal in removed if after[literal] == 0}
    for literal, count in sorted(removed.items()):
        tag = "  <-- gone from the file entirely" if literal in vanished else ""
        print("        removed x" + str(count) + "  " + literal + tag)


def dump_added(before: Counter, added: Counter) -> None:
    for literal, count in sorted(added.items()):
        tag = "  <-- value is new to this file" if before[literal] == 0 else ""
        print("        added   x" + str(count) + "  " + literal + tag)


def dump_changes(before: Counter, after: Counter, removed: Counter, added: Counter) -> None:
    dump_removed(after, removed)
    dump_added(before, added)


#: The only literals an extracted boolean helper introduces: its own returns.
PREDICATE_RETURNS = {"num 0", "num 1"}


def is_extract_to_predicate(before: Counter, removed: Counter, added: Counter) -> bool:
    """True for the one shape where added literals do not mean a substitution.

    Extracting a duplicated block into a named boolean helper - Recipe D or P -
    removes the block's literals from every call site but one, and the helper
    brings its own `return 0;` and `return 1;`. That reads as "a count dropped
    while another rose", which is otherwise the signature of a substituted
    constant.

    It is only this shape when every added literal is a 0 or a 1 that the file
    already contained, and no removed value vanished from the file entirely. A
    substitution that happens to land on 0 or 1 still has to take its value from
    somewhere, and that source value vanishing is what the caller checks next.
    """
    if not added:
        return False
    if any(literal not in PREDICATE_RETURNS for literal in added):
        return False
    return all(before[literal] > 0 for literal in added)


def report_removed_literals(rel: str, before: Counter, after: Counter, strict: bool) -> bool:
    removed = before - after
    added = after - before
    vanished = [literal for literal in removed if after[literal] == 0]
    if added and not (is_extract_to_predicate(before, removed, added) and not vanished):
        print("FAIL  " + rel + "  - a constant was substituted")
        dump_changes(before, after, removed, added)
        return False
    if added:
        label = "FAIL " if strict else "WARN "
        print(label + " " + rel + "  - copies removed, 0/1 returns added")
        dump_changes(before, after, removed, added)
        print("        looks like extracting a duplicated block into a named boolean")
        print("        helper: the call sites lose the block's literals and the helper")
        print("        brings its own returns. Legal for Recipes D and P - confirm the")
        print("        helper is the block, not a rewrite of it.")
        return not strict
    if vanished:
        print("FAIL  " + rel + "  - a constant vanished from the file")
        dump_changes(before, after, removed, added)
        return False
    label = "FAIL " if strict else "WARN "
    print(label + " " + rel + "  - copies removed, every value still present")
    dump_changes(before, after, removed, added)
    print("        looks like deduplication. Legal for Recipes D and P, but a")
    print("        deleted duplicate block looks the same - have a human confirm.")
    return not strict


def report_literal_changes(rel: str, before: Counter, after: Counter, strict: bool) -> bool:
    removed = before - after
    added = after - before
    if not removed and not added:
        print("OK    " + rel + "  (" + str(sum(before.values())) + " literals unchanged)")
        return True
    if removed:
        return report_removed_literals(rel, before, after, strict)
    label = "FAIL " if strict else "WARN "
    print(label + " " + rel + "  - literals added, none removed")
    dump_changes(before, after, removed, added)
    return not strict


def check(rel: str, base: str, strict: bool = False) -> bool:
    """Return True if the file is clean.

    Three outcomes, by what the literal counts did:

    FAIL - a value vanished from the file entirely, or a count dropped while a
    brand-new value appeared. Both are the signature of an altered constant
    (`30` becomes `31`) or of deleted logic.

    WARN - counts dropped, but every value is still present somewhere and no
    new value appeared. That is what deduplication looks like: extracting a
    repeated condition into one named predicate removes copies of its literals
    without changing any of them. Recipes D and P both do this legitimately.

    WARN - counts only went up. A new guard clause brings its own `return 0`.

    Use --strict to fail on warnings too.

    The blind spot is deliberate: deleting one copy of a duplicated block looks
    identical to deduplicating it. Recipe D therefore still needs human review.
    """
    before = git_show(base, rel)
    if before is None:
        print("SKIP  " + rel + "  (not in " + base + "; new file)")
        return True

    path = REPO / rel
    if not path.is_file():
        print("FAIL  " + rel + "  (deleted from working tree)")
        return False

    after = path.read_text(encoding="utf-8", errors="replace")
    return report_literal_changes(rel, literals(before), literals(after), strict)


def collect_combined_literals(rels: list[str], base: str) -> tuple[Counter, Counter] | None:
    before = Counter()
    after = Counter()
    for rel in rels:
        old = git_show(base, rel)
        if old is not None:
            before.update(literals(strip_include_lines(old)))
        path = REPO / rel
        if not path.is_file():
            print("FAIL  " + rel + "  (deleted from working tree)")
            return None
        after.update(literals(strip_include_lines(path.read_text(encoding="utf-8", errors="replace"))))
    return before, after


def report_combined_changes(before: Counter, after: Counter, strict: bool) -> bool:
    removed = before - after
    added = after - before
    if not removed and not added:
        print("OK    combined group  (" + str(sum(before.values())) + " literals unchanged)")
        return True
    if removed and added:
        print("FAIL  combined group  - a constant was substituted")
        print("        removed: " + str(dict(removed)))
        print("        added:   " + str(dict(added)))
        return False
    if removed:
        print("WARN  combined group  - literal counts dropped, no values were added")
        print("        removed: " + str(dict(removed)))
        return not strict
    print("WARN  combined group  - literals added, none removed")
    print("        added: " + str(dict(added)))
    return not strict


def check_combined(rels: list[str], base: str, strict: bool) -> bool:
    """Compare literals across a group, allowing constants to move files."""
    counts = collect_combined_literals(rels, base)
    if counts is None:
        return False
    return report_combined_changes(*counts, strict)


def check_calls_combined(rels: list[str], base: str, strict: bool, renames: dict,
                         fnptrs: frozenset = frozenset()) -> bool:
    """Compare the call fingerprint of a group of files as one.

    This is the check Recipe S needs. A split moves whole functions into a new
    file, so every file on its own reads as calls vanishing or appearing; only
    the union of the group is meant to be unchanged. Files absent from `base`
    contribute nothing to the before side, which is exactly right for the new
    file a split creates.
    """
    before = Counter()
    after = Counter()
    for rel in rels:
        old = git_show(base, rel)
        if old is not None:
            before += calls(old)
        path = REPO / rel
        if not path.is_file():
            print("FAIL  " + rel + "  (deleted from working tree)")
            return False
        after += calls(path.read_text(encoding="utf-8", errors="replace"), fnptrs)

    return report_call_changes("combined group", apply_renames(before, renames), after, strict)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("files", nargs="*", help="paths relative to the repo root")
    ap.add_argument("--base", default="HEAD", help="git ref to compare against (default HEAD)")
    ap.add_argument("--all", action="store_true", help="check every changed .c/.cpp")
    ap.add_argument("--combined", action="store_true",
                    help="compare all supplied files as one group, so a Recipe S split that "
                         "moves whole functions between them reads as unchanged. Works with "
                         "--calls too.")
    ap.add_argument("--strict", action="store_true",
                    help="fail on added literals and reduced-count warnings")
    ap.add_argument("--calls", action="store_true",
                    help="compare called-function fingerprints instead of literals")
    ap.add_argument("--renamed", action="append", metavar="OLD=NEW", default=[],
                    help="declare that a file-local static was renamed, so --calls does "
                         "not read it as a vanished call. Repeatable. Renaming a function "
                         "another file can see is not a campaign refactor.")
    ap.add_argument("--fnptr", action="append", metavar="NAME", default=[],
                    help="declare that NAME is now passed by pointer instead of called "
                         "directly (Recipe F), so --calls counts the bare mention as the "
                         "call it replaced. Repeatable.")
    args = ap.parse_args()

    fnptrs = frozenset(name.strip() for name in args.fnptr)

    renames = {}
    for pair in args.renamed:
        if "=" not in pair:
            ap.error("--renamed wants OLD=NEW, got: " + pair)
        old_name, new_name = pair.split("=", 1)
        renames[old_name.strip()] = new_name.strip()

    targets = changed_files(args.base) if args.all else args.files
    if not targets:
        print("nothing to check")
        return 0

    if args.combined:
        group = [p.replace("\\", "/") for p in targets]
        if args.calls:
            passed = check_calls_combined(group, args.base, args.strict, renames, fnptrs)
        else:
            passed = check_combined(group, args.base, args.strict)
        if not passed:
            print()
            print("BLOCKED - this is not a legal campaign refactor.")
            return 1
        print()
        print("PASS - no call was dropped or duplicated." if args.calls
              else "PASS - no constant was removed or altered.")
        return 0

    ok = True
    for rel in targets:
        rel = rel.replace("\\", "/")
        if args.calls:
            passed = check_calls(rel, args.base, args.strict, renames, fnptrs)
        else:
            passed = check(rel, args.base, args.strict)
        if not passed:
            ok = False

    print()
    if ok:
        if args.calls:
            print("PASS - no call was dropped or duplicated.")
            return 0
        print("PASS - no constant was removed or altered.")
        return 0
    print("BLOCKED - this is not a legal campaign refactor.")
    print("Revert with:  git checkout -- <file>")
    return 1


if __name__ == "__main__":
    sys.exit(main())

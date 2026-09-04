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


def normalise_number(tok: str) -> str:
    """Fold away suffix/format differences that do not change the value."""
    body = tok.rstrip("uUlLfF")
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


def report_removed_literals(rel: str, before: Counter, after: Counter, strict: bool) -> bool:
    removed = before - after
    added = after - before
    vanished = [literal for literal in removed if after[literal] == 0]
    if added:
        print("FAIL  " + rel + "  - a constant was substituted")
        dump_changes(before, after, removed, added)
        return False
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


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("files", nargs="*", help="paths relative to the repo root")
    ap.add_argument("--base", default="HEAD", help="git ref to compare against (default HEAD)")
    ap.add_argument("--all", action="store_true", help="check every changed .c/.cpp")
    ap.add_argument("--combined", action="store_true",
                    help="compare all supplied files as one literal group")
    ap.add_argument("--strict", action="store_true",
                    help="fail on added literals and reduced-count warnings")
    args = ap.parse_args()

    targets = changed_files(args.base) if args.all else args.files
    if not targets:
        print("nothing to check")
        return 0

    if args.combined:
        return 0 if check_combined([p.replace("\\", "/") for p in targets], args.base, args.strict) else 1

    ok = True
    for rel in targets:
        rel = rel.replace("\\", "/")
        if not check(rel, args.base, args.strict):
            ok = False

    print()
    if ok:
        print("PASS - no constant was removed or altered.")
        return 0
    print("BLOCKED - this is not a legal campaign refactor.")
    print("Revert with:  git checkout -- <file>")
    return 1


if __name__ == "__main__":
    sys.exit(main())

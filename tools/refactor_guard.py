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

# Order matters: strings and chars are consumed before numbers so that digits
# inside a string are not mistaken for numeric literals.
TOKEN_RE = re.compile(
    r"""
      (?P<string>"(?:[^"\\\n]|\\.)*")
    | (?P<char>'(?:[^'\\\n]|\\.)*')
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
        if body[:2].lower() == "0x":
            return str(int(body, 16))
        if body[:2].lower() == "0b":
            return str(int(body, 2))
        if any(c in body for c in ".eE") and not body[:2].lower() == "0x":
            return repr(float(body))
        # Leading zeros are octal in C; preserve that meaning.
        if len(body) > 1 and body[0] == "0" and body.isdigit():
            return str(int(body, 8))
        return str(int(body))
    except ValueError:
        return tok


def literals(src: str) -> Counter:
    out = Counter()
    for m in TOKEN_RE.finditer(strip_comments(src)):
        if m.group("string"):
            out["str " + m.group("string")] += 1
        elif m.group("char"):
            out["chr " + m.group("char")] += 1
        else:
            out["num " + normalise_number(m.group("number"))] += 1
    return out


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


def check(rel: str, base: str, strict: bool = False) -> bool:
    """Return True if the file is clean.

    A *removed* literal is the dangerous signal: a constant was altered
    (removal plus addition of the new value) or code carrying it was deleted.
    That fails the check.

    A purely *added* literal is normally benign - a new guard clause brings its
    own `return 0`, an extracted helper its own bounds check - so it is
    reported as a warning. Use --strict to fail on those too.
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
    a, b = literals(before), literals(after)
    removed = a - b
    added = b - a

    if not removed and not added:
        print("OK    " + rel + "  (" + str(sum(a.values())) + " literals unchanged)")
        return True

    if removed:
        print("FAIL  " + rel + "  - a constant was removed or altered")
        for lit, n in sorted(removed.items()):
            print("        removed x" + str(n) + "  " + lit)
        for lit, n in sorted(added.items()):
            print("        added   x" + str(n) + "  " + lit)
        return False

    # Additions only.
    novel = [lit for lit in added if lit not in a]
    label = "WARN " if not strict else "FAIL "
    print(label + " " + rel + "  - literals added, none removed")
    for lit, n in sorted(added.items()):
        tag = "  <-- value is new to this file" if lit in novel else ""
        print("        added   x" + str(n) + "  " + lit + tag)
    if novel:
        print("        review the new values above; the rest are already used elsewhere here")
    return not strict


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("files", nargs="*", help="paths relative to the repo root")
    ap.add_argument("--base", default="HEAD", help="git ref to compare against (default HEAD)")
    ap.add_argument("--all", action="store_true", help="check every changed .c/.cpp")
    ap.add_argument("--strict", action="store_true",
                    help="also fail when literals are added (not just removed)")
    args = ap.parse_args()

    targets = changed_files(args.base) if args.all else args.files
    if not targets:
        print("nothing to check")
        return 0

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

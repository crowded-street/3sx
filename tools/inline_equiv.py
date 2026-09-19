#!/usr/bin/env python3
"""Inline this campaign's own helpers again and diff against a git ref.

`refactor_guard.py` compares multisets of literals and of call sites, so it
cannot see a *transposition*: two arguments swapped inside one compound literal,
or two helper arguments passed the wrong way round, leave both fingerprints
identical. That is the mistake a scripted Recipe A, D or V pass is most likely to
make, and on a file with hundreds of call sites it is not something reading
catches either.

This rebuilds each function's body by substituting every call to a named helper
with the helper's own body, its parameters replaced by the arguments at that call
site, and compares the result - whitespace normalised - against the same
function before the change. A helper that merely moved code around is invisible
to it; a helper that received its arguments in the wrong order is not.

    python tools/inline_equiv.py --base HEAD~1 --helper check_two_specials \
        src/sf33rd/Source/Game/com/ck_pass.c

Helpers are inlined only where the call is the whole of a `return` statement or
a whole statement, which is the shape every fold in this campaign produces.
"""

import argparse
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

FUNC = re.compile(r'^(?:static\s+)?[A-Za-z_]\w*\s+\**(\w+)\s*\(([^;{]*)\)\s*\{', re.M)


def split_args(text):
    out, depth, cur = [], 0, ''
    for ch in text:
        if ch in '([{':
            depth += 1
        elif ch in ')]}':
            depth -= 1
        if ch == ',' and depth == 0:
            out.append(cur.strip())
            cur = ''
        else:
            cur += ch
    if cur.strip():
        out.append(cur.strip())
    return out


def functions(src):
    """{name: (parameter names, body including braces)}"""
    out = {}
    for m in FUNC.finditer(src):
        depth = 0
        for k in range(m.end() - 1, len(src)):
            if src[k] == '{':
                depth += 1
            elif src[k] == '}':
                depth -= 1
                if depth == 0:
                    break
        params = [p.strip().split()[-1].lstrip('*')
                  for p in split_args(m.group(2)) if p.strip() not in ('', 'void')]
        out[m.group(1)] = (params, src[m.end() - 1:k + 1])
    return out


def inline(body, helpers, depth=0):
    if depth > 8:
        raise ValueError('helper recursion too deep')
    for name, (params, hbody) in helpers.items():
        pattern = re.compile(r'(return\s+)?\b%s\(' % re.escape(name))
        while True:
            m = pattern.search(body)
            if not m:
                break
            d, k = 1, m.end()
            while d:
                if body[k] in '([{':
                    d += 1
                elif body[k] in ')]}':
                    d -= 1
                k += 1
            end = k
            while body[end:end + 1] in (' ', ';'):
                end += 1
            args = split_args(body[m.end():k - 1])
            subs = dict(zip(params, args))
            inner = hbody[1:-1]
            if subs:
                inner = re.sub(r'\b(%s)\b' % '|'.join(map(re.escape, subs)),
                               lambda mm: subs[mm.group(1)], inner)
            body = body[:m.start()] + inline(inner, helpers, depth + 1) + body[end:]
    return body


def norm(text):
    return re.sub(r'\s+', ' ', text).strip()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('files', nargs='+')
    ap.add_argument('--base', default='HEAD')
    ap.add_argument('--helper', action='append', default=[],
                    help='a helper this change created; repeat for each')
    args = ap.parse_args()

    old, new = {}, {}
    for path in args.files:
        rel = os.path.relpath(os.path.abspath(path), ROOT)
        try:
            old.update(functions(subprocess.check_output(['git', 'show', '%s:%s' % (args.base, rel)],
                                                         cwd=ROOT).decode()))
        except subprocess.CalledProcessError:
            pass
        if os.path.exists(path):
            new.update(functions(open(path).read()))

    helpers = {h: new[h] for h in args.helper if h in new}
    missing = [h for h in args.helper if h not in new]
    if missing:
        print('no such helper: %s' % ', '.join(missing))
        return 1

    bad = 0
    for name, (_, body) in sorted(old.items()):
        if name in helpers:
            continue
        if name not in new:
            print('MISSING %s' % name)
            bad += 1
            continue
        before, after = norm(body), norm(inline(new[name][1], helpers))
        if before != after:
            bad += 1
            print('DIFFERS %s\n   before: %s\n   after:  %s' % (name, before[:300], after[:300]))
    print('%s: %d functions, %d differ'
          % (', '.join(os.path.basename(f) for f in args.files), len(old) - len(helpers), bad))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())

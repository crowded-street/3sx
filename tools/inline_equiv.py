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


def unchoice(body, choices):
    """Put back a helper that only chose between two other 1/0 answers.

    `if (C) { return A(...); } return D(...);` called as `if (NAME(...)) break;`
    came from `if (C) { if (A(...)) break; } else { if (D(...)) break; }`, and
    that is the only shape this reverses.
    """
    shape = re.compile(r'^\s*\{\s*if \((?P<cond>[^\n]*?)\) \{\s*return (?P<a>\w+\([^;]*\));\s*\}'
                       r'\s*return (?P<d>\w+\([^;]*\));\s*\}\s*$', re.S)
    for name, (params, hbody) in choices.items():
        m = shape.match(hbody)
        if not m:
            raise ValueError('%s is not a two-way choice' % name)
        call = re.compile(r'if \(%s\(([^;]*?)\)\) \{\s*break;\s*\}' % re.escape(name))
        while True:
            hit = call.search(body)
            if not hit:
                break
            body = (body[:hit.start()]
                    + 'if (%s) { if (%s) { break; } } else { if (%s) { break; } }'
                    % (m.group('cond'), m.group('a'), m.group('d'))
                    + body[hit.end():])
    return body


def unladder(body, ladders):
    """Put a 1/0 ladder helper back inline.

    A Recipe C extraction out of a `switch` arm turns `if (a) break;` into
    `if (a) return 1;` inside the helper and leaves `if (helper(...)) break;`
    behind. Reversing it is the same rewrite backwards, and it is the only way
    to compare the arm against what it was.
    """
    for name, (params, hbody) in ladders.items():
        pattern = re.compile(r'if \(%s\(([^;]*?)\)\) \{\s*(break|return 1);\s*\}' % re.escape(name))
        while True:
            m = pattern.search(body)
            if not m:
                break
            args = split_args(m.group(1))
            subs = dict(zip(params, args))
            inner = hbody[1:-1]
            if subs:
                inner = re.sub(r'\b(%s)\b' % '|'.join(map(re.escape, subs)),
                               lambda mm: subs[mm.group(1)], inner)
            inner = re.sub(r'\s*return 0;\s*$', '', inner)
            inner = inner.replace('return 1;', m.group(2) + ';')
            body = body[:m.start()] + inner.strip() + body[m.end():]
    return body


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
    ap.add_argument('--choice', action='append', default=[],
                    help='a helper that only picks between two 1/0 answers; repeat for each')
    ap.add_argument('--ladder', action='append', default=[],
                    help='a 1/0 helper lifted out of a switch arm; repeat for each')
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
    ladders = {h: new[h] for h in args.ladder if h in new}
    choices = {h: new[h] for h in args.choice if h in new}
    missing = [h for h in args.helper + args.ladder + args.choice if h not in new]
    if missing:
        print('no such helper: %s' % ', '.join(missing))
        return 1

    bad, checked = 0, 0
    for name, (_, body) in sorted(old.items()):
        if name in helpers or name in ladders or name in choices:
            continue
        if name not in new:
            print('MISSING %s' % name)
            bad += 1
            continue
        checked += 1
        before, after = norm(body), norm(inline(unladder(unchoice(new[name][1], choices), ladders), helpers))
        if before != after:
            bad += 1
            print('DIFFERS %s\n   before: %s\n   after:  %s' % (name, before[:300], after[:300]))
    print('%s: %d functions, %d differ'
          % (', '.join(os.path.basename(f) for f in args.files),
             checked, bad))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())

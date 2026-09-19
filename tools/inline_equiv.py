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


def unpredicate(body, predicates):
    """Put a Recipe P named predicate back where it was.

    The helper is `{ return EXPR; }` and the call sites are whole expressions,
    so the reconstruction is EXPR verbatim - no added parentheses, because the
    original had none and the comparison is textual.
    """
    shape = re.compile(r'^\s*\{\s*return (?P<expr>.*);\s*\}\s*$', re.S)
    for name, (params, hbody) in predicates.items():
        m = shape.match(hbody)
        if not m:
            raise ValueError('%s is not a single-expression predicate' % name)
        call = re.compile(r'\b%s\(' % re.escape(name))
        while True:
            hit = call.search(body)
            if not hit:
                break
            d, k = 1, hit.end()
            while d:
                if body[k] in '([{':
                    d += 1
                elif body[k] in ')]}':
                    d -= 1
                k += 1
            subs = dict(zip(params, split_args(body[hit.end():k - 1])))
            expr = m.group('expr')
            if subs:
                expr = re.sub(r'\b(%s)\b' % '|'.join(map(re.escape, subs)),
                              lambda mm: subs[mm.group(1)], expr)
            body = body[:hit.start()] + expr + body[k:]
    return body


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
        pattern = re.compile(r'if \(%s\(([^;]*?)\)\) \{\s*(break|return 0|return 1|return);\s*\}' % re.escape(name))
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


ARM = re.compile(r'\n\n(?=    (?:case [^:]+|default):)')


def arms(name, fs, seen=None):
    """{label: statements} for a function whose body is one switch.

    Recipe X moves later arms into a helper reached through `default`, so the
    comparison that matters is arm by arm with the chain followed. A `default`
    whose whole body is a call to another switch on the same expression is
    replaced by that switch's arms.
    """
    seen = (seen or set()) | {name}
    params, body = fs[name]
    m = re.match(r'\s*\{\s*switch \(([^\n]*)\) \{(.*)\}\s*\}\s*$', body, re.S)
    if not m:
        return {'body': norm(body)}
    out = {}
    for part in ARM.split(m.group(2).strip('\n')):
        label = re.match(r'\s*(case ([^:]+)|default):', part)
        if not label:
            raise ValueError('unparsed arm in %s: %r' % (name, part[:60]))
        key = (label.group(2) or 'default').strip()
        stmt = norm(part[label.end():])
        call = re.match(r'^(\w+)\(([^;]*)\); break;$', stmt)
        if key == 'default' and call and call.group(1) in fs and call.group(1) not in seen:
            out.update(arms(call.group(1), fs, seen))
            continue
        out[key] = stmt
    return out


def trace(name, fs, locals_, seen=None):
    """The ordered (callee, arguments) a function runs, helpers expanded.

    Used where a fold moved where a local is computed - a helper that casts
    `wk->wu.target_adrs` itself instead of taking the caller's `em` runs the
    same calls with the same arguments, but the text no longer matches. Pass
    `--local em=(WORK*)wk->wu.target_adrs` and the two sides compare equal.
    """
    seen = (seen or set()) | {name}
    params, body = fs[name]
    out = []
    for m in re.finditer(r'\b([A-Za-z_]\w*)\(', body):
        callee = m.group(1)
        if callee in ('if', 'while', 'for', 'switch', 'sizeof', 'return'):
            continue
        d, k = 1, m.end()
        while d:
            if body[k] in '([{':
                d += 1
            elif body[k] in ')]}':
                d -= 1
            k += 1
        args = [locals_.get(a, a) for a in split_args(body[m.end():k - 1])]
        if callee in fs and callee not in seen:
            sub = dict(zip(fs[callee][0], args))
            for c, a in trace(callee, fs, locals_, seen):
                out.append((c, [re.sub(r'\b(%s)\b' % '|'.join(map(re.escape, sub)),
                                       lambda mm: sub[mm.group(1)], x) if sub else x
                                for x in a]))
        else:
            out.append((callee, args))
    return out


def norm(text):
    """Whitespace-insensitive form.

    Runs of whitespace collapse to one space, and a space just inside a bracket
    goes entirely - re-wrapping a call across lines adds exactly those, and
    without this a pure re-indent reads as a change. Whitespace is never
    semantic in C outside a string literal, and this comparison is already lossy
    inside one.
    """
    text = re.sub(r'\s+', ' ', text).strip()
    return re.sub(r'([(\[]) | ([)\]])', lambda m: m.group(1) or m.group(2), text)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('files', nargs='+')
    ap.add_argument('--base', default='HEAD')
    ap.add_argument('--helper', action='append', default=[],
                    help='a helper this change created; repeat for each')
    ap.add_argument('--switch', action='store_true',
                    help='compare switch arms, following a Recipe X default chain')
    ap.add_argument('--trace', action='store_true',
                    help='compare ordered call traces instead of text')
    ap.add_argument('--local', action='append', default=[],
                    help='NAME=EXPR: a local the fold stopped passing, e.g. '
                         'em=(WORK*)wk->wu.target_adrs')
    ap.add_argument('--predicate', action='append', default=[],
                    help='a Recipe P named predicate; repeat for each')
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
    # A helper that already existed before the change is inlined on both sides,
    # so the comparison is between two fully expanded bodies rather than
    # between an expanded one and a call.
    was = {h: old[h] for h in args.helper if h in old}
    ladders = {h: new[h] for h in args.ladder if h in new}
    choices = {h: new[h] for h in args.choice if h in new}
    predicates = {h: new[h] for h in args.predicate if h in new}
    missing = [h for h in args.helper + args.ladder + args.choice + args.predicate
               if h not in new]
    if missing:
        print('no such helper: %s' % ', '.join(missing))
        return 1

    bad, checked = 0, 0
    for name, (_, body) in sorted(old.items()):
        if name in helpers or name in ladders or name in choices or name in predicates:
            continue
        if name not in new:
            print('MISSING %s' % name)
            bad += 1
            continue
        checked += 1
        if args.switch:
            before, after = arms(name, old), arms(name, new)
            if before != after:
                bad += 1
                print('DIFFERS %s (switch arms)' % name)
                for key in sorted(set(before) | set(after)):
                    if before.get(key) != after.get(key):
                        print('   arm %s\n     before: %s\n     after:  %s'
                              % (key, before.get(key), after.get(key)))
            continue
        if args.trace:
            locals_ = dict(x.split('=', 1) for x in args.local)
            if trace(name, old, locals_) != trace(name, new, locals_):
                bad += 1
                print('DIFFERS %s (call trace)' % name)
                for x, y in zip(trace(name, old, locals_) + [None] * 40,
                                trace(name, new, locals_) + [None] * 40):
                    if x != y:
                        print('   before: %s\n   after:  %s' % (x, y))
                        break
            continue
        before = norm(inline(body, was))
        after = norm(inline(unladder(unchoice(unpredicate(new[name][1], predicates), choices),
                                     ladders), helpers))
        if before != after:
            bad += 1
            print('DIFFERS %s\n   before: %s\n   after:  %s' % (name, before[:300], after[:300]))
    print('%s: %d functions, %d differ'
          % (', '.join(os.path.basename(f) for f in args.files),
             checked, bad))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())

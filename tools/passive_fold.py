#!/usr/bin/env python3
"""Mechanical transformations for the COM passive pattern scripts.

The twenty files in src/sf33rd/Source/Game/com/passive/ are one shape repeated
a few thousand times: a pattern function that switches on the script's step
counter, CP_Index[wk->wu.id][0], and runs one engine call per step.

    void Passive14_0122(PLW* wk) {
        switch (CP_Index[wk->wu.id][0]) {
        case 0:
            Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x71, 0x71, 0x72, 0x73}, 2);
            break;

        default:
            End_Pattern(wk);
            break;
        }
    }

Two campaign recipes apply to that shape, and both are too repetitive to do by
hand at this scale:

  fold    Recipe V.  Families of three or more pattern functions whose bodies
          are identical apart from the *arguments* of their calls share one
          skeleton.  Every varying argument is written out in full, in
          positional order, at its own call site, and the helper does nothing
          with it but pass it to the argument position it already occupied.
          Parameter types are copied from the callee's own prototype, so no
          value is ever narrowed or widened.

  xsplit  Recipe X.  A pattern function with more arms than the complexity
          threshold allows keeps its first six, and reaches the rest through
          its default arm.  Case labels are never renumbered.

  verify  The check that matters.  Both transformations are supposed to be
          invisible once the helpers are inlined again, so this rebuilds the
          step -> statement map of every pattern function on both sides of the
          change and diffs them.  A renamed parameter, a transposed argument or
          a dropped arm shows up here; refactor_guard.py cannot see any of them.
"""

import argparse
import collections
import glob
import json
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SWITCH_HEAD = 'switch (CP_Index[wk->wu.id][0]) {'


# --------------------------------------------------------------------------
# prototypes
# --------------------------------------------------------------------------

def load_prototypes():
    """Map callee name -> list of declared parameters, from every header."""
    decl = re.compile(r'\b([A-Za-z_][\w \*]*?)\b(\w+)\s*\(([^;{)]*)\)\s*;', re.S)
    protos = {}
    for header in glob.glob(os.path.join(ROOT, 'src', '**', '*.h'), recursive=True):
        text = open(header, errors='ignore').read()
        text = re.sub(r'/\*.*?\*/', '', text, flags=re.S)
        text = re.sub(r'//[^\n]*', '', text)
        for m in decl.finditer(text):
            args = m.group(3).strip()
            params = [] if args in ('', 'void') else [a.strip() for a in args.split(',')]
            protos.setdefault(m.group(2), params)
    return protos


def param_type(protos, callee, index):
    params = protos.get(callee)
    if params is None or index >= len(params):
        return None
    m = re.match(r'^(.*?)([A-Za-z_]\w*)$', params[index].strip())
    if not m:
        return None
    return m.group(1).strip(), m.group(2)


# --------------------------------------------------------------------------
# parsing
# --------------------------------------------------------------------------

FUNC = re.compile(r'^(static )?void (\w+)\(\s*PLW\* wk[^)]*\)\s*\{', re.M)


def functions(src):
    """[(name, start, end, is_static)] for every top-level PLW* function."""
    out = []
    for m in FUNC.finditer(src):
        depth = 0
        end = m.end()
        for k in range(m.end() - 1, len(src)):
            if src[k] == '{':
                depth += 1
            elif src[k] == '}':
                depth -= 1
                if depth == 0:
                    end = k + 1
                    break
        out.append((m.group(2), m.start(), end, bool(m.group(1))))
    return out


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


CALL = re.compile(r'\b([A-Z]\w+)\(')


def skeletonize(body, protos):
    """Replace every argument of every known call with a numbered placeholder."""
    slots, out, i = [], [], 0
    while True:
        m = CALL.search(body, i)
        if not m:
            out.append(body[i:])
            break
        name = m.group(1)
        if name not in protos:
            out.append(body[i:m.end()])
            i = m.end()
            continue
        depth, j = 1, m.end()
        while depth:
            if body[j] in '([{':
                depth += 1
            elif body[j] in ')]}':
                depth -= 1
            j += 1
        args = split_args(body[m.end():j - 1])
        out.append(body[i:m.end()])
        out.append(', '.join('\x00%d\x00' % (len(slots) + k) for k in range(len(args))))
        out.append(')')
        for k, a in enumerate(args):
            slots.append((a, name, k))
        i = j
    return ''.join(out), slots


def snake(name):
    return re.sub(r'(?<=[a-z0-9])(?=[A-Z])', '_', name).lower()


COLUMN_LIMIT = 120


def signature(name, params):
    """A definition header wrapped the way .clang-format would wrap it."""
    flat = 'static void %s(%s) ' % (name, ', '.join(params))
    if len(flat) <= COLUMN_LIMIT:
        return flat
    return 'static void %s(\n    %s\n) ' % (name, ', '.join(params))


def call(indent, name, args):
    """A call statement wrapped the way .clang-format would wrap it."""
    flat = '%s%s(%s);' % (indent, name, ', '.join(args))
    if len(flat) <= COLUMN_LIMIT:
        return flat
    inner = indent + '    '
    return '%s%s(\n%s\n%s);' % (indent, name,
                                 ',\n'.join(inner + a for a in args), indent)


# --------------------------------------------------------------------------
# Recipe V - fold a family onto one skeleton
# --------------------------------------------------------------------------

def find_families(src, protos, min_members=3, max_params=3):
    fams = collections.defaultdict(list)
    for name, a, b, is_static in functions(src):
        if is_static or not re.match(r'^\w+_\d+$', name):
            continue
        full = src[a:b]
        body = full[full.index('{'):]
        sk, slots = skeletonize(body, protos)
        fams[sk].append((name, a, b, slots))
    out = []
    for sk, members in fams.items():
        if len(members) < min_members:
            continue
        count = len(members[0][3])
        vary = [i for i in range(count)
                if len({m[3][i][0] for m in members}) > 1]
        if not vary or len(vary) > max_params:
            continue
        out.append((sk, members, vary))
    out.sort(key=lambda r: (-len(r[1]), r[1][0][0]))
    return out


def fold(path, protos, min_members=3, max_params=3):
    src = open(path).read()
    families = find_families(src, protos, min_members, max_params)
    helpers, edits, used = [], [], set()
    for sk, members, vary in families:
        slots = members[0][3]
        callees = []
        for _, callee, _ in slots:
            if not callees or callees[-1] != callee:
                callees.append(callee)
        steps = [snake(c) for c in callees if c != 'End_Pattern']
        base = 'pattern_' + '_'.join(steps[:3])
        name, n = base, 2
        while name in used:
            name, n = '%s_%d' % (base, n), n + 1
        params, names = [], {}
        for i in vary:
            _, callee, index = slots[i]
            t = param_type(protos, callee, index)
            if t is None:
                params = None
                break
            ty, pname = t
            pname = snake(pname)
            while pname in names.values():
                pname += '_b'
            names[i] = pname
            params.append((ty, pname))
        if params is None:
            continue
        used.add(name)
        body = sk
        for i, (value, _, _) in enumerate(slots):
            body = body.replace('\x00%d\x00' % i, names[i] if i in vary else value)
        helpers.append(signature(name, ['PLW* wk'] + ['%s %s' % p for p in params]) + body + '\n')
        for member, a, b, mslots in members:
            args = ['wk'] + [mslots[i][0] for i in vary]
            edits.append((a, b, 'void %s(PLW* wk) {\n%s\n}'
                          % (member, call('    ', name, args))))
    if not edits:
        return 0, 0
    return apply(path, src, edits, helpers)


# --------------------------------------------------------------------------
# Recipe X - split a long dispatch at its default arm
# --------------------------------------------------------------------------

ARM_SPLIT = re.compile(r'\n\n(?=    (?:case \d+|default):)')


def arms_of(full):
    m = re.match(r'(?:static )?void \w+\(\s*PLW\* wk[^)]*\)\s*\{\n    '
                 + re.escape(SWITCH_HEAD) + r'\n(.*)\n    \}\n\}$', full, re.S)
    if not m:
        return None, None
    parts = ARM_SPLIT.split(m.group(1))
    cases = [p for p in parts if p.lstrip().startswith('case')]
    default = [p for p in parts if p.lstrip().startswith('default')]
    if len(default) != 1 or len(cases) + 1 != len(parts):
        return None, None
    return cases, default[0]


def xsplit(path, max_cases=6):
    src = open(path).read()
    helpers, edits = [], []
    for name, a, b, is_static in functions(src):
        if is_static:
            continue
        cases, default = arms_of(src[a:b])
        if cases is None or len(cases) <= max_cases:
            continue
        head, tail = cases[:max_cases], cases[max_cases:]
        first = re.match(r'\s*case (\d+):', tail[0]).group(1)
        hname = '%s_from_step_%s' % (snake(name), first)
        helpers.append('static void %s(PLW* wk) {\n    %s\n%s\n    }\n}\n'
                       % (hname, SWITCH_HEAD, '\n\n'.join(tail + [default])))
        edits.append((a, b, 'void %s(PLW* wk) {\n    %s\n%s\n    }\n}'
                      % (name, SWITCH_HEAD,
                         '\n\n'.join(head + ['    default:\n        %s(wk);\n        break;' % hname]))))
    if not edits:
        return 0, 0
    return apply(path, src, edits, helpers)


def apply(path, src, edits, helpers):
    edits.sort(key=lambda e: -e[0])
    out = src
    for a, b, text in edits:
        out = out[:a] + text + out[b:]
    anchor = min(i for i in (out.find('static void pattern'), out.find('static void passive'),
                             out.find('void Passive')) if i >= 0)
    out = out[:anchor] + '\n'.join(helpers) + '\n' + out[anchor:]
    open(path, 'w').write(out)
    return len(helpers), len(edits)


# --------------------------------------------------------------------------
# verify - inline the helpers again and compare step maps
# --------------------------------------------------------------------------

def norm(text):
    return re.sub(r'\s+', ' ', text).strip()


def step_map(src, name, bodies, seen=None):
    """{'0': stmt, ..., 'default': stmt} for a pattern function, helpers inlined."""
    seen = seen or set()
    if name in seen:
        raise ValueError('recursive helper %s' % name)
    seen = seen | {name}
    full, params = bodies[name]
    m = re.match(r'\s*\{\s*(\w+)\(\s*wk\s*(?:,\s*)?(.*?)\s*\);\s*\}\s*$', full, re.S)
    if m and m.group(1) in bodies:
        # a folded one-liner: bind the arguments and inline the skeleton
        target, targs = m.group(1), split_args(m.group(2))
        inner = step_map(src, target, bodies, seen)
        _, tparams = bodies[target]
        subs = dict(zip(tparams, targs))
        return {k: re.sub(r'\b(%s)\b' % '|'.join(map(re.escape, subs)),
                          lambda mm: subs[mm.group(1)], v) if subs else v
                for k, v in inner.items()}
    m = re.match(r'\s*\{\s*' + re.escape(SWITCH_HEAD) + r'(.*)\}\s*\}\s*$', full, re.S)
    if not m:
        return {'body': norm(full)}
    out = {}
    for part in ARM_SPLIT.split(m.group(1).strip('\n')):
        label = re.match(r'\s*(case (\d+)|default):', part)
        if not label:
            raise ValueError('unparsed arm in %s: %r' % (name, part[:60]))
        key = label.group(2) or 'default'
        stmt = norm(part[label.end():])
        if key == 'default':
            call = re.match(r'^(\w+)\(wk\); break;$', stmt)
            if call and call.group(1) in bodies and call.group(1) != 'End_Pattern':
                out.update(step_map(src, call.group(1), bodies, seen))
                continue
        out[key] = stmt
    return out


def collect(srcs):
    bodies = {}
    for src in srcs:
        for name, a, b, _ in functions(src):
            full = src[a:b]
            sig = full[:full.index('{')]
            params = [p.strip().split()[-1].lstrip('*')
                      for p in split_args(sig[sig.index('(') + 1:sig.rindex(')')])][1:]
            bodies[name] = (full[full.index('{'):], params)
    return bodies


def verify(base_ref, paths, quiet=False):
    olds, news = [], []
    for path in paths:
        rel = os.path.relpath(os.path.abspath(path), ROOT)
        try:
            olds.append(subprocess.check_output(['git', 'show', '%s:%s' % (base_ref, rel)],
                                                cwd=ROOT).decode())
        except subprocess.CalledProcessError:
            pass                                    # a file this split created
        news.append(open(path).read())
    old_bodies, new_bodies = collect(olds), collect(news)
    targets = sorted(n for n in old_bodies if re.match(r'^\w+_\d+$', n))
    bad = 0
    for name in targets:
        if name not in new_bodies:
            print('MISSING %s' % name)
            bad += 1
            continue
        before = step_map(None, name, old_bodies)
        after = step_map(None, name, new_bodies)
        if before != after:
            bad += 1
            print('DIFFERS %s' % name)
            for key in sorted(set(before) | set(after)):
                if before.get(key) != after.get(key):
                    print('   step %s\n     before: %s\n     after:  %s'
                          % (key, before.get(key), after.get(key)))
    if not quiet:
        print('%s: %d pattern functions, %d differ' % (', '.join(map(os.path.basename, paths)),
                                                       len(targets), bad))
    return bad


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('command', choices=['fold', 'xsplit', 'verify', 'families'])
    ap.add_argument('files', nargs='+')
    ap.add_argument('--base', default='HEAD')
    ap.add_argument('--min-members', type=int, default=3)
    ap.add_argument('--max-params', type=int, default=3)
    ap.add_argument('--max-cases', type=int, default=6)
    args = ap.parse_args()

    if args.command == 'verify':
        sys.exit(1 if verify(args.base, args.files) else 0)

    protos = load_prototypes()
    for path in args.files:
        if args.command == 'families':
            for sk, members, vary in find_families(open(path).read(), protos,
                                                   args.min_members, args.max_params):
                callees = []
                for _, callee, _ in members[0][3]:
                    if not callees or callees[-1] != callee:
                        callees.append(callee)
                print('%3d members %d args  %s' % (len(members), len(vary), '+'.join(callees)))
        elif args.command == 'fold':
            h, e = fold(path, protos, args.min_members, args.max_params)
            print('%s: %d skeletons, %d pattern functions folded' % (os.path.basename(path), h, e))
        else:
            h, e = xsplit(path, args.max_cases)
            print('%s: %d dispatch tails split' % (os.path.basename(path), h))


if __name__ == '__main__':
    main()

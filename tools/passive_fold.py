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
import itertools
import json
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SWITCH_HEAD = 'switch (CP_Index[wk->wu.id][0]) {'

# The COM script folders share one shape under two names: Game/com/passive
# spells a script Passive14_0122 and Game/com/active spells it Pattern14_0122,
# behind dispatchers called Passive14 and Computer14. FAMILY carries which.
FAMILY = {'script': 'Passive', 'dispatcher': 'Passive', 'shared': 'pass_patterns',
          'prefix': '', 'what': 'passive', 'folder': 'passive'}


def set_family(name):
    if name == 'active':
        FAMILY.update(script='Pattern', dispatcher='Computer', shared='active_patterns',
                      prefix='active_', what='active', folder='active')


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

FUNC = re.compile(r'^(static )?void (\w+)\(\s*PLW\* wk[^{;]*\)\s*\{', re.M)


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


def signature(name, params, qualifier='static '):
    """A definition header wrapped the way .clang-format would wrap it."""
    flat = '%svoid %s(%s) ' % (qualifier, name, ', '.join(params))
    if len(flat) + len('{') <= COLUMN_LIMIT:
        return flat
    return '%svoid %s(\n    %s\n) ' % (qualifier, name, ', '.join(params))


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
        # A script with no steps at all: every value of the counter ends it.
        base = FAMILY['prefix'] + 'pattern_' + ('_'.join(steps[:3]) or 'end_immediately')
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

# The decompilation is not consistent about the blank line before an arm, and a
# checker that silently glues two arms together is worse than no checker. This
# one tolerates either spelling and is used only for verification; the strict
# form above is what generated code is written with.
ARM_SPLIT_LOOSE = re.compile(r'\n\n?(?=    (?:case \d+|default):)')


def arms_of(full):
    m = re.match(r'(?:static )?void \w+\(\s*PLW\* wk[^{;]*\)\s*\{\n    '
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
    candidates = [out.find('static void ' + FAMILY['prefix'] + 'pattern'),
                  out.find('static void ' + FAMILY['dispatcher'].lower()),
                  out.find('void %s(PLW* wk)' % re.search(r'void (%s\d+)\(' % FAMILY['dispatcher'],
                                                          out).group(1)),
                  out.find('void %s' % FAMILY['script'])]
    anchor = min(i for i in candidates if i >= 0)
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
    for part in ARM_SPLIT_LOOSE.split(m.group(1).strip('\n')):
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
            params = [(re.search(r'\(\*(\w+)\)', p) or re.search(r'(\w+)\s*$', p)).group(1)
                      for p in split_args(sig[sig.index('(') + 1:sig.rindex(')')])][1:]
            bodies[name] = (full[full.index('{'):], params)
    return bodies


# --------------------------------------------------------------------------
# Recipe V, across the whole folder
# --------------------------------------------------------------------------

SHARED_C = """/**
 * @file %(shared)s.c
 * COM %(What)s: pattern skeletons shared by every character
 *
 * %(Article)s %(what)s pattern script is a switch on the step counter with one engine
 * call per step, and the same step sequences recur across characters. The
 * skeletons here are what those patterns have in common. Each one is exactly
 * the body its call sites used to hold, with the arguments of its calls taken
 * as parameters and written out in full at each call site.
 */

#include "sf33rd/Source/Game/com/%(folder)s/%(shared)s.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

"""

SHARED_H = """/*
 * Pattern skeletons shared by every character's %(what)s scripts.
 * See %(shared)s_1step.c and its siblings.
 */

#ifndef %(GUARD)s
#define %(GUARD)s

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

"""


STEP_FILE = {1: '_1step.c', 2: '_2step.c', 3: '_3step.c', 4: '_long.c'}


def shared_destination(paths, cases):
    """The shared file a skeleton of this many steps belongs in.

    `reshard` names them by exact step count and splits an overfull one, so
    prefer a file for exactly this count and fall back to the by-band naming
    the first split used.
    """
    for want in ('_%dstep.c' % cases, '_%dstep_' % cases, STEP_FILE.get(min(cases, 4), '')):
        hit = [p for p in paths if want and want in os.path.basename(p)]
        if hit:
            return sorted(hit)[0]
    return sorted(paths)[-1]


def shared_files(folder):
    return sorted(glob.glob(os.path.join(folder, FAMILY['shared'] + '_*.c')))


def rewrite_shared_header(folder):
    """Regenerate pass_patterns.h from whatever the shared files now define."""
    decls = []
    for path in shared_files(folder):
        src = open(path).read()
        for name, a, b, is_static in functions(src):
            decls.append(rewrap(src[a:b][:src[a:b].index('{')].rstrip() + ';'))
    header = os.path.join(folder, FAMILY['shared'] + '.h')
    text = open(header).read()
    keep = text[:text.index('#include "types.h"') + len('#include "types.h"')]
    open(header, 'w').write(keep + '\n\n' + '\n'.join(sorted(decls)) + '\n\n#endif\n')


SHARED_DOC = """/**
 * @file %(name)s
 * COM Passive: pattern skeletons shared by every character
 *
 * %(what)s
 *
 * A passive pattern script is a switch on the step counter with one engine call
 * per step, and the same step sequences recur across characters. Each skeleton
 * here is exactly the body its call sites used to hold, with the arguments of
 * its calls taken as parameters and written out in full at each call site.
 */

"""


def reshard(folder, max_lines=900):
    """Redistribute the shared skeletons into files by how many steps they run."""
    existing = shared_files(folder) or [os.path.join(folder, FAMILY['shared'] + '.c')]
    includes = re.search(r'(#include[^\n]*\n)+', open(existing[0]).read()).group(0)
    buckets = collections.defaultdict(list)
    for path in existing:
        src = open(path).read()
        for name, a, b, is_static in functions(src):
            buckets[len(re.findall(r'case \d+:', src[a:b]))].append((name, src[a:b]))
    for path in existing:
        os.remove(path)

    made = []
    for steps in sorted(buckets):
        group = sorted(buckets[steps])
        parts, cur, lines = [], [], 0
        for item in group:
            n = item[1].count('\n') + 2
            if cur and lines + n > max_lines:
                parts.append(cur)
                cur, lines = [], 0
            cur.append(item)
            lines += n
        parts.append(cur)
        for i, part in enumerate(parts):
            stem = FAMILY['shared'] + '_%dstep' % steps
            name = '%s.c' % stem if i == 0 else '%s_%d.c' % (stem, i + 1)
            what = 'The %s-step patterns.' % {1: 'one', 2: 'two', 3: 'three', 4: 'four',
                                              5: 'five', 6: 'six'}.get(steps, steps)
            if len(parts) > 1:
                what += ' Part %d of %d, in name order.' % (i + 1, len(parts))
            open(os.path.join(folder, name), 'w').write(
                SHARED_DOC % {'name': name, 'what': what} + includes + '\n'
                + '\n\n'.join(body for _, body in part) + '\n')
            made.append(name)
    rewrite_shared_header(folder)
    return made


def dedup_shared(folder):
    """Collapse shared skeletons that are byte-identical to one another."""
    paths = shared_files(folder)
    groups = collections.defaultdict(list)
    for path in paths:
        src = open(path).read()
        for name, a, b, is_static in functions(src):
            groups[_shape(src[a:b])].append((path, name, a, b))
    renames, removals = {}, collections.defaultdict(list)
    for shape, members in groups.items():
        if len(members) < 2:
            continue
        keeper = sorted(members, key=lambda m: m[1])[0][1]
        for path, name, a, b in members:
            if name == keeper:
                continue
            renames[name] = keeper
            removals[path].append((a, b))
    if not renames:
        return 0
    for path in paths:
        src = open(path).read()
        for a, b in sorted(removals[path], reverse=True):
            end = b
            while src[end:end + 1] == '\n':
                end += 1
            src = src[:a] + src[end:]
        open(path, 'w').write(src)
    for path in glob.glob(os.path.join(folder, '*.c')) + glob.glob(os.path.join(folder, '*.h')):
        src = open(path).read()
        new = src
        for old, keeper in renames.items():
            new = re.sub(r'\b%s\b' % old, keeper, new)
        if new != src:
            open(path, 'w').write(new)
    rewrite_shared_header(folder)
    return len(renames)


def gfold(paths, protos, min_members=3, max_params=3, shared=None):
    """Fold families that span the whole folder into one shared skeleton file."""
    sources = {p: open(p).read() for p in paths}
    fams = collections.defaultdict(list)
    for path, src in sources.items():
        for name, a, b, is_static in functions(src):
            full = src[a:b]
            if is_static or not re.match(r'^%s\d+_\d+$' % FAMILY['script'], name) \
                    or SWITCH_HEAD not in full:
                continue
            sk, slots = skeletonize(full[full.index('{'):], protos)
            fams[sk].append((path, name, a, b, slots))

    folder = os.path.dirname(paths[0])
    used = set()
    for path in shared_files(folder):
        used |= {n for n, a, b, st in functions(open(path).read())}
    helpers, decls, edits = [], [], collections.defaultdict(list)
    work = []
    for sk, members in sorted(fams.items(), key=lambda kv: (-len(kv[1]), kv[1][0][1])):
        slots0 = members[0][4]
        identical = not any(len({m[4][i][0] for m in members}) > 1 for i in range(len(slots0)))
        if len(members) < (2 if identical else min_members):
            continue
        vary = [i for i in range(len(slots0)) if len({m[4][i][0] for m in members}) > 1]
        if not vary:
            # Nothing varies: these scripts are byte-identical. That is Recipe D
            # rather than Recipe V, so two instances are enough and the skeleton
            # takes no parameters beyond wk.
            work.append((sk, members, []))
            continue
        if len(vary) <= max_params:
            work.append((sk, members, vary))
            continue
        # More varying arguments than a skeleton may take without tripping Excess
        # Number of Function Arguments. Parameterise the max_params slots that
        # keep the most members together and specialise on the rest: the family
        # becomes several skeletons, each with the other values written into it.
        best = None
        for keep in itertools.combinations(vary, max_params):
            fixed = [i for i in vary if i not in keep]
            groups = collections.defaultdict(list)
            for m in members:
                groups[tuple(m[4][i][0] for i in fixed)].append(m)
            usable = [g for g in groups.values() if len(g) >= min_members]
            score = (sum(len(g) for g in usable), -len(usable))
            if usable and (best is None or score > best[0]):
                best = (score, list(keep), usable)
        if best is None:
            continue
        for group in best[2]:
            vary_here = [i for i in best[1] if len({m[4][i][0] for m in group}) > 1]
            if vary_here:
                work.append((sk, group, vary_here))

    for sk, members, vary in work:
        slots0 = members[0][4]
        params, names = [], {}
        for i in vary:
            _, callee, index = slots0[i]
            t = param_type(protos, callee, index)
            if t is None:
                params = None
                break
            ty, pname = t[0], snake(t[1])
            while pname in names.values():
                pname += '_b'
            names[i] = pname
            params.append('%s %s' % (ty, pname))
        if params is None:
            continue
        callees = []
        for _, callee, _ in slots0:
            if not callees or callees[-1] != callee:
                callees.append(callee)
        steps = [snake(c) for c in callees if c != 'End_Pattern']
        # A script with no steps at all: every value of the counter ends it.
        base = FAMILY['prefix'] + 'pattern_' + ('_'.join(steps[:3]) or 'end_immediately')
        name, n = base, 2
        while name in used:
            name, n = '%s_%d' % (base, n), n + 1
        used.add(name)
        body = sk
        for i, (value, _, _) in enumerate(slots0):
            body = body.replace('\x00%d\x00' % i, names[i] if i in vary else value)
        helpers.append((signature(name, ['PLW* wk'] + params, '') + body + '\n',
                        len(re.findall(r'case \d+:', body))))
        decls.append(signature(name, ['PLW* wk'] + params, '').rstrip() + ';')
        for path, member, a, b, mslots in members:
            args = ['wk'] + [mslots[i][0] for i in vary]
            edits[path].append((a, b, 'void %s(PLW* wk) {\n%s\n}'
                                % (member, call('    ', name, args))))
    if not helpers:
        return 0, 0

    existing = shared_files(folder)
    if existing:
        for text, cases in helpers:
            dest = shared_destination(existing, cases)
            open(dest, 'a').write('\n' + text)
        rewrite_shared_header(folder)
    else:
        fmt = {'shared': FAMILY['shared'], 'what': FAMILY['what'], 'folder': FAMILY['folder'],
               'What': FAMILY['what'].capitalize(), 'GUARD': FAMILY['shared'].upper() + '_H',
               'Article': 'An' if FAMILY['what'][0] in 'aeiou' else 'A'}
        open(os.path.join(folder, FAMILY['shared'] + '.c'), 'w').write(
            SHARED_C % fmt + '\n'.join(t for t, _ in helpers))
        open(os.path.join(folder, FAMILY['shared'] + '.h'), 'w').write(
            SHARED_H % fmt + '\n'.join(rewrap(d) for d in decls) + '\n\n#endif\n')

    total = 0
    for path, es in edits.items():
        src = sources[path]
        for a, b, text in sorted(es, key=lambda e: -e[0]):
            src = src[:a] + text + src[b:]
        if FAMILY['shared'] + '.h' not in src:
            src = src.replace('#include "common.h"',
                              '#include "sf33rd/Source/Game/com/%s/%s.h"\n#include "common.h"'
                              % (FAMILY['folder'], FAMILY['shared']), 1)
        open(path, 'w').write(src)
        total += len(es)
    return len(helpers), total


# --------------------------------------------------------------------------
# Recipe D - one copy of a skeleton the characters share verbatim
# --------------------------------------------------------------------------

def _shape(full):
    """(body, parameter list) with whitespace normalised, for identity tests."""
    sig = full[:full.index('{')]
    params = re.sub(r'^\s*(static )?void \w+\(', '', sig).rstrip().rstrip(')')
    return re.sub(r'\s+', ' ', full[full.index('{'):]), re.sub(r'\s+', ' ', params)


def dedup(paths, shared_paths, header):
    """Point every verbatim copy of a skeleton at one shared definition."""
    shared, shared_src = {}, {p: open(p).read() for p in shared_paths}
    for path, src in shared_src.items():
        for name, a, b, is_static in functions(src):
            shared[_shape(src[a:b])] = name

    sources = {p: open(p).read() for p in paths}
    groups = collections.defaultdict(list)
    for path, src in sources.items():
        for name, a, b, is_static in functions(src):
            full = src[a:b]
            # The per-character helpers this campaign made: a Recipe X tail is
            # named for the script it continues (pattern14_0026_from_step_6) and
            # a Recipe S rename prefixes the dispatcher (computer14_pattern_*).
            if not re.match(r'^(%s|%s)\d+_' % (FAMILY['dispatcher'].lower(), FAMILY['script'].lower()),
                            name) or SWITCH_HEAD not in full:
                continue
            groups[_shape(full)].append((path, name, a, b, full))

    renames, additions, removals = {}, [], collections.defaultdict(list)
    for shape, members in groups.items():
        if shape in shared:
            target = shared[shape]
        elif len(members) > 1:
            first = members[0][1]
            tail = re.match(r'^(?:%s|%s)\d+_\d+_from_step_(\d+)$'
                            % (FAMILY['dispatcher'].lower(), FAMILY['script'].lower()), first)
            if tail:
                # A Recipe X tail: name it for the steps it runs, as the fold
                # names a skeleton, plus the label it starts at.
                steps = []
                for callee in re.findall(r'\b([A-Z]\w+)\(', members[0][4]):
                    if callee != 'End_Pattern' and (not steps or steps[-1] != callee):
                        steps.append(callee)
                base = '%spattern_%s_from_step_%s' % (FAMILY['prefix'],
                                                      '_'.join(snake(c) for c in steps[:3]),
                                                      tail.group(1))
            else:
                base = re.sub(r'_\d+$', '',
                              re.sub(r'^(?:%s|%s)\d+_'
                                     % (FAMILY['dispatcher'].lower(), FAMILY['script'].lower()),
                                     '', first))
            target, n = base, 2
            while target in set(shared.values()):
                target, n = '%s_%d' % (base, n), n + 1
            additions.append((target, members[0][4],
                              len(re.findall(r'case \d+:', members[0][4]))))
            shared[shape] = target
        else:
            continue
        for path, name, a, b, full in members:
            renames[name] = target
            removals[path].append((a, b))

    if not renames:
        return 0, 0

    for target, full, cases in additions:
        dest = shared_destination(shared_paths, cases)
        text = re.sub(r'^(static )?void \w+\(', 'void %s(' % target, full, count=1)
        shared_src[dest] = shared_src[dest].rstrip('\n') + '\n\n' + text + '\n'
    for path, src in shared_src.items():
        open(path, 'w').write(rewrap(src))

    decls = []
    for path in shared_paths:
        src = open(path).read()
        for name, a, b, is_static in functions(src):
            decls.append(rewrap(src[a:b][:src[a:b].index('{')].rstrip() + ';'))
    text = open(header).read()
    keep = text[:text.index('#include "types.h"') + len('#include "types.h"')]
    open(header, 'w').write(keep + '\n\n' + '\n'.join(sorted(decls)) + '\n\n#endif\n')

    touched = 0
    for path, src in sources.items():
        for a, b in sorted(removals[path], reverse=True):
            end = b
            while src[end:end + 1] == '\n':
                end += 1
            src = src[:a] + src[end:]
        for old, new in renames.items():
            src = re.sub(r'\b%s\b' % old, new, src)
        if FAMILY['shared'] + '.h' not in src:
            src = src.replace('#include "common.h"',
                              '#include "sf33rd/Source/Game/com/%s/%s.h"\n#include "common.h"'
                              % (FAMILY['folder'], FAMILY['shared']), 1)
        open(path, 'w').write(rewrap(src))
        touched += len(removals[path])

    for internal in sorted(set(glob.glob(os.path.join(
            os.path.dirname(paths[0]), 'pass*_internal.h')))):
        text = open(internal).read()
        gone = re.compile(r'^void (%s)\((?:[^;]*?)\);\n' % '|'.join(map(re.escape, renames)),
                          re.M | re.S)
        open(internal, 'w').write(gone.sub('', text))
    with open(os.path.join(os.path.dirname(paths[0]), '.dedup-renames'), 'w') as fh:
        for old, new in sorted(renames.items()):
            fh.write('%s=%s\n' % (old, new))
    return len(additions), touched


# --------------------------------------------------------------------------
# Recipe F - one skeleton, the differing call passed in
# --------------------------------------------------------------------------

def skeleton_with_callees(body, protos):
    """Like skeletonize, but the callee names are placeholders too."""
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
        out.append(body[i:m.start()])
        out.append('\x00%d\x00(' % len(slots))
        slots.append(('callee', name, None))
        out.append(', '.join('\x00%d\x00' % (len(slots) + k) for k in range(len(args))))
        out.append(')')
        for k, a in enumerate(args):
            slots.append(('arg', a, (name, k)))
        i = j
    return ''.join(out), slots


def types_of(protos, callee):
    out = []
    for param in protos.get(callee, []):
        m = re.match(r'^(.*?)([A-Za-z_]\w*)$', param.strip())
        out.append(m.group(1).strip() if m else param.strip())
    return tuple(out)


def ffold(path, protos, min_members=3, max_params=3):
    src = open(path).read()
    fams = collections.defaultdict(list)
    for name, a, b, is_static in functions(src):
        full = src[a:b]
        if is_static or not re.match(r'^(%s\d+_\d+|pattern_\w+)$' % FAMILY['script'], name) \
                or SWITCH_HEAD not in full:
            continue
        body = full[full.index('{'):]
        sk, slots = skeleton_with_callees(body, protos)
        key = (sk, tuple(types_of(protos, v) for k, v, _ in slots if k == 'callee'))
        fams[key].append((name, a, b, slots))
    helpers, edits, used = [], [], set()
    for (sk, sigs), members in sorted(fams.items(), key=lambda kv: (-len(kv[1]), kv[1][0][0])):
        if len(members) < min_members:
            continue
        slots0 = members[0][3]
        vary = [i for i in range(len(slots0))
                if len({m[3][i][1] for m in members}) > 1]
        if not vary or len(vary) > max_params or not any(slots0[i][0] == 'callee' for i in vary):
            continue
        params, names, ok = [], {}, True
        for i in vary:
            kind, value, origin = slots0[i]
            if kind == 'callee':
                pname = 'step' if 'step' not in names.values() else 'step_%d' % len(names)
                params.append(('void (*%s)(%s)' % (pname, ', '.join(types_of(protos, value))), pname))
            else:
                t = param_type(protos, origin[0], origin[1])
                if t is None:
                    ok = False
                    break
                ty, pname = snake(t[1]), snake(t[1])
                while pname in names.values():
                    pname += '_b'
                params.append(('%s %s' % (t[0], pname), pname))
            names[i] = params[-1][1]
        if not ok:
            continue
        pieces = []
        for i, (kind, value, _) in enumerate(slots0):
            if kind == 'callee':
                pieces.append(names[i] if i in vary else snake(value))
        base = 'pattern_' + '_'.join(p for p in pieces if p != 'end_pattern')
        name, n = base, 2
        while name in used:
            name, n = '%s_%d' % (base, n), n + 1
        used.add(name)
        body = sk
        for i, (kind, value, _) in enumerate(slots0):
            body = body.replace('\x00%d\x00' % i, names[i] if i in vary else value)
        helpers.append(signature(name, ['PLW* wk'] + [p for p, _ in params]) + body + '\n')
        for member, a, b, mslots in members:
            args = ['wk'] + [mslots[i][1] for i in vary]
            edits.append((a, b, 'void %s(PLW* wk) {\n%s\n}' % (member, call('    ', name, args))))
    if not edits:
        return 0, 0
    return apply(path, src, edits, helpers)


def verify(base_ref, paths, quiet=False):
    olds, news = [], []
    for path in paths:
        rel = os.path.relpath(os.path.abspath(path), ROOT)
        try:
            olds.append(subprocess.check_output(['git', 'show', '%s:%s' % (base_ref, rel)],
                                                cwd=ROOT).decode())
        except subprocess.CalledProcessError:
            pass                                    # a file this split created
        if os.path.exists(path):
            news.append(open(path).read())          # else: a file this split removed
    old_bodies, new_bodies = collect(olds), collect(news)
    targets = sorted(n for n in old_bodies if re.match(r'^%s\d+_\d+$' % FAMILY['script'], n))
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


# --------------------------------------------------------------------------
# Recipe S - split the file
# --------------------------------------------------------------------------

SPLIT_DOC = """/*
 * Internal declarations for the %(stem)s pattern scripts.
 *
 * %(stem)s.c holds the dispatcher and %(table)s, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */
"""


def relocate_helpers(src, spans):
    """Move each campaign skeleton just above its first call site."""
    order, bodies = [], {}
    for name, a, b, is_static in spans:
        bodies[name] = src[a:b]
        order.append(name)
    helpers = [n for n in order if n.startswith('pattern_') or '_from_step_' in n]
    rest = [n for n in order if n not in set(helpers)]
    out = []
    pending = set(helpers)
    for name in rest:
        for h in helpers:
            if h in pending and re.search(r'\b%s\(' % h, bodies[name]):
                out.append(h)
                pending.discard(h)
        out.append(name)
    return [n for n in helpers if n in pending] + out, bodies


def rewrap(text):
    """Re-apply .clang-format's wrapping to any line a rename pushed over 120."""
    out = []
    for line in text.split('\n'):
        if len(line) <= COLUMN_LIMIT:
            out.append(line)
            continue
        m = re.match(r'^(static )?void (\w+)\((.*)\) \{$', line)
        if m:
            out.append(signature(m.group(2), split_args(m.group(3)), m.group(1) or '') + '{')
            continue
        m = re.match(r'^(\s*)(\w+)\((.*)\);$', line)
        if m:
            out.append(call(m.group(1), m.group(2), split_args(m.group(3))))
            continue
        out.append(line)
    return '\n'.join(out)


def split(path, max_funcs=90, max_lines=900):
    src = open(path).read()
    stem = os.path.basename(path)[:-2]
    prefix = re.search(r'^void (%s\d+)\(PLW\* wk\) \{' % FAMILY['dispatcher'],
                       src, re.M).group(1).lower() + '_'
    for name, a, b, is_static in reversed(functions(src)):
        if name.startswith('pattern_'):
            src = re.sub(r'\b%s\b' % name, prefix + name, src)
    spans = functions(src)
    head = src[:spans[0][1]].rstrip('\n') + '\n'
    tail = src[spans[-1][2]:].lstrip('\n')
    order, bodies = relocate_helpers(src, spans)

    chunks, cur, lines = [], [], 0
    for name in order:
        n = bodies[name].count('\n') + 2
        if cur and (len(cur) >= max_funcs or lines + n > max_lines):
            chunks.append(cur)
            cur, lines = [], 0
        cur.append(name)
        lines += n
    chunks.append(cur)
    if len(chunks) == 1:
        return 0

    where = {name: i for i, chunk in enumerate(chunks) for name in chunk}
    external = []                       # (name, declaration, comment)
    for name in order:
        users = {where[u] for u in order if u != name and re.search(r'\b%s\(' % name, bodies[u])}
        if name.startswith(FAMILY['script']) and where[name] != 0:
            external.append(name)       # the table in chunk 0 names it
        elif users - {where[name]}:
            external.append(name)

    includes = re.findall(r'^#include .*$', head, re.M)
    internal = '%s_internal.h' % stem
    decls = []
    for name in external:
        sig = bodies[name][:bodies[name].index('{')].strip()
        decls.append(re.sub(r'^static ', '', sig) + ';')
    guard = internal.upper().replace('.', '_')
    table = re.search(r'void \(\*const (\w+)\)?\[', head)
    decls = [rewrap(d) for d in decls] if False else decls
    header = (SPLIT_DOC % {'stem': stem, 'table': re.search(r'\(\*const (\w+)\[', head).group(1)}
              + '\n#ifndef %s\n#define %s\n\n#include "sf33rd/Source/Game/com/com_sub.h"\n#include "structs.h"\n#include "types.h"\n\n'
              % (guard, guard)
              + '\n'.join(decls) + '\n\n#endif\n')
    open(os.path.join(os.path.dirname(path), internal), 'w').write(header)

    ext = set(external)
    parts = []
    for i, chunk in enumerate(chunks):
        text = []
        for name in chunk:
            body = bodies[name]
            if name in ext:
                body = re.sub(r'^static ', '', body)
            text.append(body)
        parts.append(rewrap('\n\n'.join(text)))

    banner = head.split('*/')[0] + '*/\n'
    incl = '\n'.join(includes[:1] + ['#include "sf33rd/Source/Game/com/%s/%s"' % (FAMILY['folder'], internal)]
                     + includes[1:]) + '\n'
    middle = re.sub(r'^#include .*\n', '', head.split('*/', 1)[1], flags=re.M).strip('\n')
    open(path, 'w').write(banner + '\n' + incl + '\n' + middle + '\n\n' + parts[0] + '\n\n' + tail)
    for i in range(1, len(chunks)):
        name = '%s_%d.c' % (stem, i + 1)
        first = next(n for n in chunks[i] if n[0].isupper())
        doc = (banner.replace('%s.c' % stem, name).rstrip('\n')
               .rsplit('\n', 1)[0] + '\n * Pattern scripts from %s on.\n */\n' % first)
        open(os.path.join(os.path.dirname(path), name), 'w').write(doc + '\n' + incl + '\n' + parts[i] + '\n')
    return len(chunks)


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('command', choices=['fold', 'gfold', 'dedup', 'reshard', 'ffold', 'xsplit', 'split', 'verify', 'families'])
    ap.add_argument('files', nargs='+')
    ap.add_argument('--base', default='HEAD')
    ap.add_argument('--family', default='passive', choices=['passive', 'active'],
                    help='which COM script folder: passive spells a script '
                         'Passive14_0122, active spells it Pattern14_0122')
    ap.add_argument('--min-members', type=int, default=3)
    ap.add_argument('--max-params', type=int, default=3)
    ap.add_argument('--max-cases', type=int, default=6)
    ap.add_argument('--max-funcs', type=int, default=90)
    ap.add_argument('--max-lines', type=int, default=900)
    args = ap.parse_args()
    set_family(args.family)

    if args.command == 'verify':
        sys.exit(1 if verify(args.base, args.files) else 0)

    protos = load_prototypes()
    if args.command == 'reshard':
        made = reshard(os.path.dirname(args.files[0]), args.max_lines)
        print('shared skeletons in %d files: %s' % (len(made), ', '.join(made)))
        return
    if args.command == 'dedup':
        folder = os.path.dirname(args.files[0])
        sharedp = sorted(glob.glob(os.path.join(folder, FAMILY['shared'] + '_*.c')))
        h, e = dedup(args.files, sharedp, os.path.join(folder, FAMILY['shared'] + '.h'))
        d = dedup_shared(folder)
        print('%d skeletons promoted to the shared files, %d copies removed, '
              '%d shared duplicates collapsed' % (h, e, d))
        return
    if args.command == 'gfold':
        h, e = gfold(args.files, protos, args.min_members, args.max_params)
        print('%d shared skeletons, %d pattern functions folded' % (h, e))
        return
    for path in args.files:
        if args.command == 'families':
            for sk, members, vary in find_families(open(path).read(), protos,
                                                   args.min_members, args.max_params):
                callees = []
                for _, callee, _ in members[0][3]:
                    if not callees or callees[-1] != callee:
                        callees.append(callee)
                print('%3d members %d args  %s' % (len(members), len(vary), '+'.join(callees)))
        elif args.command == 'split':
            n = split(path, args.max_funcs, args.max_lines)
            print('%s: split into %d files' % (os.path.basename(path), n))
        elif args.command == 'ffold':
            h, e = ffold(path, protos, args.min_members, args.max_params)
            print('%s: %d skeletons, %d pattern functions folded' % (os.path.basename(path), h, e))
        elif args.command == 'fold':
            h, e = fold(path, protos, args.min_members, args.max_params)
            print('%s: %d skeletons, %d pattern functions folded' % (os.path.basename(path), h, e))
        else:
            h, e = xsplit(path, args.max_cases)
            print('%s: %d dispatch tails split' % (os.path.basename(path), h))


if __name__ == '__main__':
    main()

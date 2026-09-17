# Refactoring Playbook

This is a **closed catalogue**. If a change you want to make is not one of the recipes
below, you are not allowed to make it as part of this campaign. Stop and report instead.

Every recipe here is *behaviour-preserving*: it moves code around without changing what
the program computes. That property is what makes this campaign safe to run at scale,
and it is the only reason a task can be handed to an agent that cannot run the game.

---

## The three rules

1. **One recipe, one function, one commit.** The unit is the *function*: a commit may
   apply the same recipe several times to it - four blocks extracted from one state
   machine is one commit - but it must not mix two different recipes, and it must not
   span two functions. The point is that a playtester can revert exactly one behaviour.
2. **Re-measure after every commit.** If the score improved, keep it. If it did not, run
   `code_health_review` and keep the change only when the function you targeted left a
   smell category or its cyclomatic complexity dropped. Otherwise revert. See
   *A flat score does not mean a failed refactor* below.
3. **When in doubt, stop and report.** An unfinished task is fine. A silently broken
   fighting-game engine is not.

---

## Absolutely forbidden

These will change behaviour and must never appear in a campaign commit:

- Changing any numeric literal, string literal, or enum value. **`case` labels are
  literals.** Renumbering states - rewriting `case 3:` as `case 0:` and subtracting an
  offset from the switch expression - is a literal change, however tempting it looks when
  two state machines differ only by their numbering.
- Changing arithmetic (`+ - * / %`), bit operations, or shifts.
- Changing a comparison operator, including `<` to `<=`.
- Reordering statements that have side effects (assignment, I/O, function calls).
- Changing types, including signedness (`s8` to `u8`) and width (`s16` to `s32`).
- "Fixing" a bug you spot. **Report it, leave it.** A faithful port reproduces original
  behaviour; what looks like a bug is often arcade-accurate.
- Deleting code that looks dead. It may be reachable through the replay/rollback path.
- Touching a `const` data table (the large static arrays). They are out of scope entirely.
- Renaming an existing function that is referenced from another file.

---

## Recipe E - Extract Function

**Use when:** CodeScene reports *Bumpy Road Ahead* (a function with several separate
blocks of nested logic). Each "bump" is a missing function.

**How:**

1. Find one nested block. It usually already has a comment or a blank line around it.
2. Cut it into a new `static` function directly above the current one.
3. Pass in every variable it reads as a parameter. Return the single value it produces.
4. If the block writes to more than one outer **local** variable, **skip it** and move to
   the next bump. Do not invent an out-parameter struct to carry results back.

   Writing many *fields* through a struct pointer the block already has - `ewk`, `wk`,
   `mwk` - is not what this rule is about. That is safe, and it is what most extractions
   in this codebase do. The rule exists to stop you inventing a way to return two values.

**Before:**

```c
void update_player(Player *p) {
    // ... other work ...
    if (p->state == STATE_HIT) {
        if (p->hit_timer > 0) {
            p->hit_timer--;
            if (p->hit_timer == 0) {
                p->state = STATE_IDLE;
            }
        }
    }
}
```

**After:**

```c
static void tick_hit_recovery(Player *p) {
    if (p->state != STATE_HIT) {
        return;
    }
    if (p->hit_timer <= 0) {
        return;
    }
    p->hit_timer--;
    if (p->hit_timer == 0) {
        p->state = STATE_IDLE;
    }
}

void update_player(Player *p) {
    // ... other work ...
    tick_hit_recovery(p);
}
```

Name the new function for **what it does**, not where it came from. `tick_hit_recovery`,
not `update_player_part2`.

---

## Recipe G - Guard Clauses

**Use when:** CodeScene reports *Deep, Nested Complexity* (nesting depth 4 or more).

**How:** invert the outermost condition and return early. Repeat until the main body sits
at one level of indentation. This is the highest-value recipe in the campaign - it is
mechanical, and it directly attacks the smell that correlates most strongly with defects.

**Before:**

```c
if (player != NULL) {
    if (player->active) {
        if (player->health > 0) {
            do_the_real_work(player);
        }
    }
}
```

**After:**

```c
if (player == NULL) {
    return;
}
if (!player->active) {
    return;
}
if (player->health <= 0) {
    return;
}
do_the_real_work(player);
```

**Careful:** if the function returns a value, every guard must return the *same* value the
original code would have produced by falling through. Read the end of the function before
you start. If you cannot tell what it falls through to, skip the function and report it.

---

## Recipe P - Named Predicate

**Use when:** CodeScene reports *Complex Conditional* or *Complex Method* driven by
compound boolean expressions.

**How:** move the expression into a `static` function whose name states the intent. This
usually needs no other change, and it is the safest recipe in the catalogue.

**Before:**

```c
if ((p->flags & FLAG_AIR) && p->vy < 0 && !(p->flags & FLAG_STUN) && p->combo < 3) {
    ...
}
```

**After:**

```c
static bool can_air_combo(const Player *p) {
    return (p->flags & FLAG_AIR) && p->vy < 0 && !(p->flags & FLAG_STUN) && p->combo < 3;
}

if (can_air_combo(p)) {
    ...
}
```

Copy the expression **character for character**. Do not "simplify" the boolean algebra,
and do not reorder the operands - `&&` short-circuits, so reordering can change which
side effects run and can introduce null dereferences.

---

## Recipe D - Deduplicate

**Use when:** CodeScene reports *Code Duplication*.

**How:** only when the blocks are **identical or differ by a single value**. Extract one
`static` helper and pass the differing value as a parameter. If they differ in two or more
places, leave them alone - forcing a shared abstraction over near-miss duplicates is how
faithful ports get broken.

---

## Recipe C - Extract Common Part

**Use when:** CodeScene reports *Code Duplication* and the blocks share a **contiguous
identical run** - a prefix, a suffix, or a middle - but differ elsewhere, so Recipe D does
not apply.

This is the mirror image of Recipe D, and it is safer. Recipe D moves the *difference*
into the helper as a parameter. Recipe C moves only the *identical* part, and every
difference stays at the call site where it is still visible and still unchanged.

**How:**

1. Find the longest run of lines that is identical in both blocks, character for
   character. Whitespace may differ; nothing else may.
2. Move exactly that run into a `static` helper. Do not tidy it on the way.
3. Leave everything else at the call sites, in its original order.
4. If the shared run ends inside control flow - the callers need to know whether to carry
   on - the helper returns `0` or `1` and each caller branches on it. Return nothing else.

**Before:**

```c
static void advance_4000(Work *w) {
    if (!paused()) {
        move(w);
        if (--w->timer <= 0) {
            w->state++;
            set_anim(w, 29);          /* <- differs */
        }
    }
}

static void advance_11000(Work *w, Work *parent) {
    if (!paused()) {
        move(w);
        if (--w->timer <= 0) {
            w->state++;
            parent->cmd = 9;          /* <- differs */
        }
    }
}
```

**After:**

```c
static s32 approach_complete(Work *w) {
    if (!paused()) {
        move(w);
        if (--w->timer <= 0) {
            w->state++;
            return 1;
        }
    }
    return 0;
}

static void advance_4000(Work *w) {
    if (approach_complete(w)) {
        set_anim(w, 29);
    }
}

static void advance_11000(Work *w, Work *parent) {
    if (approach_complete(w)) {
        parent->cmd = 9;
    }
}
```

**Careful:** this does **not** license merging near-miss blocks by parameterising two or
more differences. That is exactly what Recipe D forbids, and it stays forbidden. If you
cannot point at a contiguous run that is identical, there is nothing here to extract.

`refactor_guard.py` reports this shape as a WARN - *copies removed, 0/1 returns added* -
because the call sites lose the run's literals while the helper brings its own returns.

---

## Recipe T - Shared Table Scan

**Use when:** a function is built out of the same **threshold-table scan** repeated several
times, and the instances differ only in the table, its row count, and the value scanned -
so Recipe D's single-difference rule refuses them and Recipe C finds no identical run
because the table name sits inside the loop.

This is the idiom `grade.c` is made of:

```c
for (i = 0; i < 23; i++) {
    if (num < grade_t_meichuuritsu2[i + 1][0]) {
        break;
    }
}

point2 = grade_t_meichuuritsu2[i][1];
```

Three things vary, which is why this needed its own entry rather than a relaxation of
Recipe D. It is safe for a different reason than Recipe D is: **all three varying things
are values copied verbatim from the call site**, and the only code that moves is the loop
itself, which moves once and unchanged. Nothing is generalised; the scan is not rewritten
to cope with a new case.

**How:**

1. Name the table's **row type in a header**, next to the tables' own declarations:

   ```c
   /* grade.h */
   typedef const s16 GradeRow[2];
   ```

   This is not cosmetic. Written directly, the parameter is `const s16 table[][2]`, and
   that `2` is a literal new to the `.c` file; against the literals the merge removes,
   `refactor_guard.py` reads the pair as *a constant was substituted* and FAILs. With the
   row type named, the `.c` gains no literal and the fingerprint reads the deduplication
   it actually is.

2. Extract **one** `static` helper holding the scan, character for character as it stands:

   ```c
   static s16 table_points(const GradeRow* table, s16 count, s16 value) {
       s16 i;

       for (i = 0; i < count; i++) {
           if (value < table[i + 1][0]) {
               break;
           }
       }

       return table[i][1];
   }
   ```

3. Each call site passes **its own** table by name, **its own** bound, and **its own**
   scanned expression, each written exactly as it appears today. The bound stays a literal
   at the call site - it is not derived with `sizeof`, and it is not moved into the table.

4. The helper returns the row's score and does nothing else. It does not accumulate into
   `point`, clamp, or take a second table.

**Preconditions, all of them:**

- The loop body is identical across every instance apart from the table name, the bound,
  and the scanned value. If the comparison operator, the `+ 1`, or the column indices
  differ anywhere, those instances are not one family - leave them.
- Every table has the same row type. A family scanning `const s16 t[N][2]` and one
  scanning `const s16 t[N][3]` are two families, and merging them would be a type change.
- The tables themselves are never touched, reordered, or re-declared. Recipe T reads them
  through a pointer; the `const` arrays stay exactly as they are.
- **The bounds and the scanned values are still there afterwards.** The expected literal
  fingerprint is the deduplication WARN - *copies removed, every value still present* -
  and the counts say exactly what was collapsed: eight scans sharing one loop read as
  `removed x14 num 0` and `removed x14 num 1`, sixteen of each in the loops against two of
  each kept in the helper. If a *bound* leaves the fingerprint, it did not travel to the
  call site and the merge is wrong. The header's own diff is the legal "literals added,
  none removed".

**Group the scans by what they score.** Nine scans behind one helper is fine; three
helpers of three scans each read as duplicates of one another and the score falls.

**The same argument covers a table *selection*, not just a scan.** Where several arms
choose between the same two tables the same way and differ only in which pair they name,
one helper can take the pair:

```c
static const u16* select_waza_table(const PLW* wk, s16 kos, AsstblCharRows* arcade, AsstblCharRows* ps2) {
    if (ArcadeBalance_IsEnabled()) {
        return arcade[CHAR_3SX_TO_ARCADE(wk->player_number)][kos];
    }

    return ps2[wk->player_number][kos];
}
```

and each arm passes its own two by name:

```c
    wst = select_waza_table(wk, kos, asstbl_lv_2000_arcade, _asstbl_lv_2000);
```

This is two differing values, which Recipe D refuses, and it is safe for Recipe T's reason
rather than Recipe D's: **both tables are written out verbatim at the call site, and the
helper does nothing with them but the two subscripts it already performed.** The moment it
tests one, picks between them on anything but the flag that was already there, or computes
an index from a parameter, that argument is gone and this is Recipe D's forbidden case.

The preconditions are Recipe T's, plus one:

- The selection rule is identical in every arm - the same flag, the same subscripts on
  each side. If one arm indexes the arcade table differently, the arms are not one family.
- Tables of different first extent are fine and are the usual case: `[20][6][2]` and
  `[21][6][2]` both decay to the same parameter type. **The extents that remain must be
  named in a header**, for the reason the array-typed-parameter rule below gives - written
  out, the `[6][2]` is two literals new to the `.c` and the guard reads the pair as a
  substitution.

Measured on `pls03.c`'s `waza_select`, whose five arms each chose between an arcade table
and the PS2 one: **8.08 -> 8.19**, cc 20 -> 15, and five copies of the `ArcadeBalance`
branch collapsed into one.

---

## Recipe F - Action Parameter

**Use when:** two or more functions - or two or more arms of one `switch` - share a control
skeleton that is identical character for character **except for the name of one function
being called**. Recipe D allows a differing *value*; this is the same shape with a differing
*callee*, which no existing recipe reaches.

**Before:**

```c
static s32 comm_pa_x(PLW* wk, CTC* ctc) {
    switch (ctc->koc) {
    case 0:  add_script_x_offset(wk, ctc);  break;
    case 2:  add_script_x_offset(wk, ctc);  /* fallthrough */
    default: emwk = (WORK*)wk->target_adrs; add_script_x_offset(emwk, ctc); break;
    }
    return 1;
}

static s32 comm_pa_y(PLW* wk, CTC* ctc) {
    switch (ctc->koc) {
    case 0:  add_script_y_offset(wk, ctc);  break;
    case 2:  add_script_y_offset(wk, ctc);  /* fallthrough */
    default: emwk = (WORK*)wk->target_adrs; add_script_y_offset(emwk, ctc); break;
    }
    return 1;
}
```

**After:**

```c
static s32 dispatch_by_koc(PLW* wk, CTC* ctc, void (*action)(PLW*, CTC*)) {
    switch (ctc->koc) {
    case 0:  action(wk, ctc);  break;
    case 2:  action(wk, ctc);  /* fallthrough */
    default: emwk = (WORK*)wk->target_adrs; action(emwk, ctc); break;
    }
    return 1;
}

static s32 comm_pa_x(PLW* wk, CTC* ctc) { return dispatch_by_koc(wk, ctc, add_script_x_offset); }
static s32 comm_pa_y(PLW* wk, CTC* ctc) { return dispatch_by_koc(wk, ctc, add_script_y_offset); }
```

**Preconditions, all of them:**

- **Exactly one call differs, and nothing else.** If the arms differ in a second callee or
  in a statement, Recipe F does not apply - that is the near-miss case Recipe D forbids,
  and it stays forbidden.
- **The call may differ whole** - the callee *and* the arguments it is given. The nine
  attack stances in `pls03.c` differ in which selector they call and at which level, and
  both travel into the helper's argument list at each arm:

  ```c
  case 20:
      return start_nm_attack(wk, kos, 3, select_nm_attack_level_3010);
  ```

  This is not "parameterise two differences". It is safe for the reason Recipe T is safe:
  every varying part is written out verbatim at its own call site, so no arm's pair can be
  mis-mapped, and the helper **does nothing with those parameters except pass them
  straight to the call**. It must not test them, index with them, or compute from them -
  the moment it does, the differences have been generalised and this is Recipe D's
  forbidden case again.
- **The callees' signatures are identical**, parameter for parameter, return type included.
  The parameter is written with that exact prototype. Casting a function pointer to make
  two signatures fit is a type change and is forbidden outright - it is also undefined
  behaviour.
- **The arguments at the call are unchanged**, in the same order, with the same
  expressions. `action(emwk, ctc)` is legal because `add_script_x_offset(emwk, ctc)` was
  what stood there.
- **Nothing becomes non-`static` to be pointed at.** Taking the address of a file-local
  function is fine; widening linkage so a helper in another file can be named here is not,
  and is not what this recipe is for.
- The pointer is passed **as a bare name at the call site**, never stored in a table or a
  struct field, never chosen at run time. Recipe F replaces a duplicated skeleton; it does
  not introduce dispatch the program did not have.

**The guard needs telling.** `--calls` counts a name as a call only when a `(` follows it,
so a callee now passed by pointer reads as a vanished call and FAILs. Declare each one:

```bash
python tools/refactor_guard.py --calls --fnptr add_script_x_offset \
                               --fnptr add_script_y_offset src/.../charset_position.c
```

The declaration is yours, not a guess by the tool: the counts still have to balance
afterwards, and the expected signature is Recipe C/D's - each shared callee drops by the
copies removed, and the new helper is `+1` per call site plus `+1` for its definition.

**This is control flow.** Every application of Recipe F reroutes a call through a pointer,
so it belongs in the *genuinely high risk* tier of the verification loop: run
`tools/replay_verify.sh` on it, not just the build and the guards.

---

## Recipe X - Split Dispatch

**Use when:** CodeScene reports *Complex Method* on a function whose complexity is mostly
its own `switch` - a state machine with more arms than the threshold allows. Recipe E does
not help: there is no bump of nested logic to lift out, only arms.

**How:**

1. Choose a contiguous group of later arms that belong together - the airborne half of a
   jump, the teardown of an effect.
2. Move them to a `static` helper that switches on **the same expression**.
3. Reach the helper from the original `default:` arm.

**The binding constraint: case labels are never renumbered.** The helper keeps the
original labels, so a state still reads as the number the rest of the engine uses. If you
find yourself writing `switch (index - 2)` with labels `0..4`, stop - that is a literal
change, and it is forbidden.

**Before:**

```c
void term(Work *w, s16 a) {
    switch (w->index) {
    case 0: begin(w); break;
    case 1: launch(w); break;
    case 2: rise(w); break;
    case 3: climb(w, a); break;
    case 4: strike(w, a); break;
    default: land(w); break;
    }
}
```

**After:**

```c
/* The airborne half: everything from the rise onwards. The case labels are the
 * original ones, so the states still read as the same numbers. */
static void term_airborne(Work *w, s16 a) {
    switch (w->index) {
    case 2: rise(w); break;
    case 3: climb(w, a); break;
    case 4: strike(w, a); break;
    default: land(w); break;
    }
}

void term(Work *w, s16 a) {
    switch (w->index) {
    case 0: begin(w); break;
    case 1: launch(w); break;
    default: term_airborne(w, a); break;
    }
}
```

Anything that ran after the switch - a trailing lever merge, a sort request - stays in the
caller, after its switch, where it ran before.

**The group may also be chosen by what two switches agree on.** When two sibling state
machines are a Code Duplication pair because several of their arms are identical, the same
move breaks the pair: put the arms they share in one helper, and let *both* switches reach
it from a new `default`. Measured on `plpat06.c`'s run and throw markers, which agree on
arms 20, 30 and 40 and disagree on the rest: 8.81 -> 9.38, and the finding went.

The safety argument is the one above plus one more step. Case labels are mutually
exclusive, so a `cg_type` of 20 that used to match the caller's own arm now falls to the
`default` and matches the same label in the helper. A value matching none of the labels did
nothing before, and still does nothing - **provided neither caller had a `default` of its
own and the helper does not add one.** If either switch already has a `default`, this
variant does not apply: the values that used to reach it would now reach the helper first.

---

## Recipe A - Parameter Object

**Use when:** CodeScene reports *Excess Number of Function Arguments* (more than four for
C). No other recipe reaches this smell, and on the Term families it was the only thing
standing between the file and a clean review.

**Authorised by the project owner on 2026-09-15**, including on public signatures, in the
knowledge that it can mean rewriting thousands of call sites in the character scripts.
That is mechanical, but it is not small: `Command_Attack` alone has 1,658 call sites.

**How:**

1. Declare a `struct` whose fields are the function's parameters, **in the same order**
   and with **exactly the same types**. Name it for the family, not the function.
2. Change the function to take one `const Struct*`, and read each parameter as a field.
   Pass the same pointer on to any helper that only ever ran on those values.
3. Rewrite every call site to pass its **original argument list** through a compound
   literal: `f(wk, &(Args){8, 0, 0x30, -1});`. Do not reorder, reformat or "clean up" the
   arguments while you are in there.
4. Declare the struct in the header the callers already include, beside the function.

**Preconditions - all of them:**

- Two functions share a struct **only** if their parameter lists match in order *and* in
  type. One signed-versus-unsigned field is enough to require two structs; merging them
  would be a type change, which is forbidden.
- Where two families need the same subset of values - approach gates, a landing step -
  give that subset its own small struct and build it at the call site from fields the
  caller already holds. Do not widen one family's struct to serve another.
- Check that the function is never referenced other than as a direct call. A name that
  appears in a function-pointer table cannot take a new signature.
- `refactor_guard.py` must report **OK** on every changed file - not WARN. The argument
  lists are copied verbatim, so the literal fingerprint cannot move. A WARN here means the
  rewrite touched something it should not have.

**Scriptable, and worth scripting.** A regex over `name(wk, <args>);` that wraps the
argument list is reliable *if* it also asserts, per file, that the number of matches
equals the number of occurrences of `name(wk,` - that catches a call split over two lines,
which the regex would silently miss - and that every call has the expected argument count.

---

## Recipe S - Split File

**Use when:** CodeScene reports *Lines of Code in a Single File*.

**This is the last recipe you apply, never the first.** Reduce function complexity first;
the file often drops out of the Red band without needing a split at all.

**How:**

1. Group functions that share a `static` variable or an obvious theme.
2. Move that group to a new `.c` file beside the original.
3. Add a matching `.h` **only** for functions used from outside the new file.
4. The build globs sources, so no CMake edit is needed - but re-run `cmake --build build`
   and confirm the link step succeeds.

Do not split a file if it would require making a `static` function non-`static`. Widening
linkage is a behaviour change in the sense that matters here: it changes what the rest of
the program can reach.

---

## Recipe R - Resolve a Goto Chain

**Use when:** CodeScene reports *Complex Method* on a function whose complexity is mostly
`goto`. CodeScene counts each `goto` as a branch, so a function built from a chain of
`if (...) goto label;` scores roughly twice its apparent complexity and no other recipe in
this catalogue reaches it - E, G, X and P all leave the jumps exactly where they were.

This shape is common in the decompiled code: the Ghidra output for a chain of early
returns comes back as a chain of jumps to labels that each return.

**Added 2026-09-17** under the project owner's standing authorisation to write new recipes
when a transformation is behaviour-preserving by construction.

**How:**

1. Check the preconditions below. If any of them fails, stop - this is not the shape.
2. Replace each `goto L;` with a **verbatim copy** of the `return` statement that stands
   under `L`. Character for character: the same expression, the same subscripts, the same
   casts.
3. Delete every label that is now unreferenced, together with the return beneath it.
4. The one label that was also reachable by falling off the end of the code above keeps
   its return, unlabelled, as the function's tail.

**Preconditions, all of them:**

- **Every label in the chain ends in an unconditional transfer** - a `return`, or a
  `break` that leaves the enclosing loop or switch - and holds nothing after it. The
  common case is a label holding exactly one `return`; a label holding a short fixed
  sequence that *ends* in the transfer is the same shape and is allowed, but keep the
  sequence to one or two statements, or extract it (Recipe E) before applying this one so
  the copies do not become a duplication finding of their own.
- **No label can be reached by falling into it.** Each label is entered only by its
  `goto`s, or by falling off the end of the code above into the *first* of them. If
  control can fall from one label's body into the next, the chain encodes an order and
  deleting it changes behaviour.
- **Every `goto` jumps forward, and stays inside the same construct.** A backward jump is
  a loop and is out of scope; a jump that leaves a loop or switch is not a transfer this
  recipe can copy.
- **The conditions are not touched.** The `if`s keep their operators, their operands and
  their order; only the jump becomes the statements it jumped to.
- **Nothing else in the function references the labels.**

**Before:**

```c
u8 pick_row(Work *w, s16 ix) {
    if (w->missed)      goto miss;
    if (w->flags & 3)   goto hit;
    if (w->flags & 0xC0) goto block;
miss:
    return miss_table[w->id][ix];
hit:
    return hit_table[w->id][ix];
block:
    return block_table[w->id][ix];
}
```

**After:**

```c
u8 pick_row(Work *w, s16 ix) {
    if (w->missed) {
        return miss_table[w->id][ix];
    }

    if (w->flags & 3) {
        return hit_table[w->id][ix];
    }

    if (w->flags & 0xC0) {
        return block_table[w->id][ix];
    }

    return miss_table[w->id][ix];
}
```

Note the tail: the original fell off the last `if` into `miss:`, so the miss row is
returned twice and that repetition is correct. Do not "tidy" it by reordering the tests to
avoid it - that would reorder the conditions, which is forbidden.

**What the guard shows.** Literals are **added, none removed** - the copied return brings
its own subscripts - and `--calls` is unchanged, because a table subscript is not a call.
A removed literal here means a return was rewritten rather than copied, and that is a FAIL.

**This is control flow.** Like Recipe F, a Recipe R commit belongs in the genuinely
high-risk tier of the verification loop: run `tools/replay_verify.sh` on it.

---

## Known plateaus

A plateau is a result, not a failure: the point where no legal recipe raises the score
further. Record it in the task report with the reason, so the next agent does not spend a
session rediscovering it.

Every plateau found so far has the same cause - duplication between sibling state machines
that differ in more than one value, or only in their state numbering, which Recipe D and
Recipe X both refuse to merge.

| File | Plateau | Why |
| --- | --- | --- |
| `com_sub_air_term.c` | 9.68 | `ORO_JA_Term` at cc 9; clearing it makes a twin of `ORO_HJA_Term_Airborne` and costs 0.87 |
| `com_sub_attack.c` | 9.09 | the two normal-attack wind-ups differ in two statements |
| `com_sub_command_term.c` | 9.09 | two pairs of airborne twins, one state number apart |
| `eff09.c` | 8.54 | see *The eff09 family* below |
| `eff09_endgame.c`, `eff09_late.c` | 9.38 | see *The eff09 family* below |
| `eff02.c` | 9.06 | near-miss siblings |
| `eff55.c` | 9.42 | the rise and the fall differ in three values; splitting the states exposes it, -0.33 |
| `eff68.c` | 9.09 | five waypoint steps differing in their timers and targets; sharing their identical runs leaves the smell unmoved |
| `eff78.c` | 9.55 | `crow_flap` and `crow_take_off` differ in five values; splitting `crow_fuss_move` exposes it, -0.17 |
| `grade.c` | **10.00** | *was 8.67.* The table-scan idiom below, cleared by Recipe T: the seventeen scans share one loop and each call site keeps its own table, bound and value. The last finding, `makeup_spp_frdat`, was an ordinary Recipe E |
| `pls03.c` | 8.92 | *was 8.08.* Recipe T twice, Recipe E on the leap and catch tests, then two shared runs for the mean. `decode_wst_data`'s twelve encodings and `waza_select`'s eleven case labels are what remain, and neither loses a branch without renumbering states |
| `cmd_main_checks.c` | 7.50 | The hardest file left. Its mean is 4.34 over 64 functions and needs **thirteen** more, which is far more than the duplicate web can absorb - every arm lifted joins one of three families. Sharing the runs was tried too (`load_waza_command_header`, `command_terminator_reached`) and measured flat, because the findings here are five Bumpy Roads and three Complex Methods rather than the mean alone |
| `pls00_normal_states.c` | 8.03 | *was 7.55.* Five shared runs - the two end-of-animation markers, the entry-frame guard, and the two jump hand-overs - cleared Overall Code Complexity. What is left is a Code Duplication web between the state machines themselves, which no run reaches: sharing the two arms `jumping_cg_type_low_pat` and `jumping_cg_type_high_pat` agree on (Recipe X's variant) measured flat, and the gate chains differ in their members and their order |
| `plpnm.c` | 7.52 | what is left of the 28-function group are state machines differing in two or more values; the two parry states keep Duff-style `case` arms that cannot be split |
| `pls03_super_arts.c` | 9.92 | *was 7.61.* Recipe C on the full-gauge guards and the EX strength launch, Recipe D on the super-art launch tail, Recipe F on the EX strength scan, then the airborne EX guard chain - an extraction this table previously recorded at -0.23, which pays now that Recipe F removed the twin behind it. The last finding is `try_grounded_dc_strengths`' Bumpy Road: giving the direct cancels the same Recipe F treatment was measured at 9.92 -> 9.09, because unlike the EX selectors the two DC ones carry the ex4th latch as well and so read as duplicates |
| `manage.c` | 9.92 | `Game_Manage_7_3`'s two identical test arms; clearing the bump means deleting the dead condition, which the catalogue forbids |
| `plcnt3.c` | **10.00** | *was 9.50.* Recipe D on the two push-out requests and Recipe P on the two both-players waits, measured as a set |
| `plmain2.c` | **10.00** | *was 9.68.* Recipe E on the bonus-game placement. Extracting the other candidate block instead measures 9.38 - it twins with `plmv_b_1010` |
| `stun.c` | **10.00** | *was 9.53.* Three extractions measured as a set (Recipe E twice, Recipe D once), then one more Recipe D for the redraw |
| `eff93.c` | 9.38 | the two slide-outs differ only in a comparison operator, which may not be parameterised |
| `effa2.c` | 9.34 | every state returns past a shared tail, so no state can move to a helper without a 0/1 protocol per arm |
| `effa9.c` | 9.16 | near-miss siblings |
| `effb5.c` | 9.58 | `goto case_1` jumps from state 2 into the middle of state 1 |
| `effd1.c` | 9.92 | `fall_data_set`'s two mirrored aiming arms - see the rule below |
| `effe9.c` | 9.92 | `effe9_panel_shape`'s two mirrored arms - see the rule below |
| `eff09_init.c` | 9.68 | ten case labels left after one split; a second split makes three dispatchers that read as duplicates, -0.30 |
| `eff11.c` | 9.13 | near-miss siblings |
| `effg6.c`, `eff00.c` | 9.21-9.22 | near-miss siblings |
| `effect.c` | 9.38 | four functions whose two arms walk the same list in mirror - see the rule below |
| `efff6.c` | 9.09 | near-miss siblings |
| `effm2.c` | 9.53 | the two cat routines' dispatchers read as duplicates once their states are named |
| `plcnt.c` | **10.00** | *was 9.47.* Recipe X on the victory pause and Recipe P on the hit-state pair cleared both Complex Methods and pushed the file over 1000 lines; Recipe S split the per-player setup into `plcnt_setup.c` |
| `plcnt_setup.c` | 9.38 | split from `plcnt.c`. Recipe C took the run both super-art setups open with; `remake_sa_store_max` and `remake_sa_gauge_len` remain, differing in the table, the index, a multiplier, the clamp bounds - and in `<=` against `<`, which may never be parameterised |
| `pls02.c` | 9.31 | **Recipe R** resolved `check_body_touch2`'s goto chain, which the previous note said was what blocked its nesting - and with the jumps gone Recipe E lifted the whole bonus-car block out, clearing Deep Nested Complexity there. The score did not move: `set_field_hosei_flag` still carries it, and splitting its two wall sides was measured at 9.31 -> 9.02 because the file already holds two latent twin pairs and a third tips it |
| `charset_position.c` | **10.00** | *was 9.09.* Recipe F merged the two `pa` axes through the `koc` skeleton; naming `comm_ps_x`'s one-line position set (Recipe E) is what stopped it reading as a copy of `set_other_y`, and the `rv` pair fell out with it |
| `plpdm_states.c` | **10.00** | *was 9.38.* Six shared runs, no arm lifted - see *Against a twin family, share what they agree on* |
| `plpdm_states_late.c` | **10.00** | *was 9.38.* Three shared runs and one ordinary extraction |
| `caldir.c` | 8.81 | Two findings, both immovable from inside the engine folder. **Excess Number of Function Arguments**: `cal_all_speed_data` and `cal_delta_speed` take 6 each, and Recipe A would clear it - but their call sites are spread across the effect and animation folders, not just the engine one, so the rewrite leaves this branch's scope. (An earlier note blamed a single call site in `plpat00.c`; that was too narrow - the real count is 62 across three folders.) **Code Duplication**: three x/y mirror pairs, each differing in five field names and a callee, which no recipe reaches - see *Two mirrored arms are cheaper left together* |
| `charset.c` | 9.68 | `set_char_move_init2` takes 5 arguments; same shape as `caldir.c` and the same reason, measured properly this time: **155** call sites, 111 of them in the effect folder and 13 in animation. Recipe A here is an effect-folder change wearing an engine-folder hat |
| `plpat.c` | **10.00** | *was 6.15.* Five extractions and three dedups cleared every function, then Recipe S moved the jump-attack dummy-RTNM group to `plpat_ja.c` to bring the 42-function mean down |
| `plpat_ja.c` | **10.00** | split from `plpat.c`. `get_cjdR`'s goto chain was the last Complex Method and the reason **Recipe R** exists; the nine rno-mapping arms then went behind one Recipe D helper, whose cc of 1 is what took the mean under the threshold |
| `plpat19.c` | **10.00** | *was 7.14.* Three Recipe D/C passes over the shared flight and marker blocks, then arm extractions, then three more for the mean |
| `plpatuni.c` | **10.00** | *was 7.37.* See *Choose which arms to extract so no two dispatchers end up bare* - the seven-extraction set scored 9.09, the same set minus two scored 10.00 |
| `plpat09.c` | 9.84 | *was 8.36.* `set_tenguiwa`'s two rock placements were **shared** rather than split - Recipe T, four call-site values - which is what every earlier attempt had got backwards; Recipe A then took the helper's five arguments. Two shared runs and two arms cleared the mean. What is left is `place_tenguiwa_set`'s two loops, whose inner guard cannot be inverted without changing `<` to `>=`, and `homing_target_x`'s mirrored arms |
| `plpat17.c` | **10.00** | *was 8.17.* Recipe D on AT1's repeated markers, then all six of its arms, then the taunt's and finally Recipe P on the bonus-car test |
| `plpat14.c` | **10.00** | *was 8.75.* Arm extractions on all four attacks; the twin AT3 exposed was closed by Recipe D on the tail the union leg and the regrab share |
| `plpat07.c` | **10.00** | *was 9.38.* One shared opening changed the count of arms needed from five to four, and four could be chosen to miss the twins |
| `plpat20.c` | **10.00** | *was 8.93.* AT1 and AT3 turned out to share two arms outright, not as near misses; after Recipe D on those, arm extractions cleared the rest |
| `plpat06.c` | **10.00** | *was 9.11.* The run and throw marker switches are the case Recipe X's shared-arm variant was written for - see the recipe |
| `plmain.c` | **10.00** | *was 9.38.* Three Recipe S splits, then the extractions that had measured flat before them - see *A file can be too big for its own mean* below. 1430 lines and 65 functions became 606 and 35, plus `plmain_arts.c`, `plmain_ps2_arts.c` and `plmain_vital.c`, all at 10.00 |
| `plmain_arts.c` | **10.00** | split from `plmain.c`. Almost any pair of helpers named out of its gauge state machines reads as a duplicate: naming `mpg_union`'s arms twins it with `eag_union`, and naming `spend_max_gauge`'s firing arm twins it with `spend_and_disarm_ex`, both -0.57. What paid was Recipe C, which removes a run instead of naming an arm |
| `hitplpl.c` | 8.59 | `player_at_vs_player_dm` is one `while (1)` whose arms leave through `break` and `goto two`; no arm can move to a helper without a numeric verdict protocol |
| `cmd_main.c` | 9.39 | `latch_sw_lvbt_bit_0x80` and `_0x800` differ only in their four case labels and two masks; splitting each in two trades their Complex Method for a Code Duplication pair at no net gain |
| `cmb_win.c` | **10.00** | *was 9.92.* Recipe F: two of the three passes over the players differed only in what they called |
| `plpdm.c` | **10.00** | *was 9.61.* Recipe X on the rumble suppression list, then Recipe E on the death conversion |
| `bbbscom.c` | **10.00** | *was 9.38.* Overall Code Complexity only, and two Recipe E extractions cleared it - the file has 15 functions, so the mean moves at once. Compare `plmain.c` above, where 65 functions make the same move worthless |
| `manage_result.c` | **10.00** | *was 9.38.* One Recipe D on `BGM_Control`'s two waits, for the same reason |

---

### The table-scan idiom, and how `grade.c` was unblocked

`grade.c` was the first plateau in this campaign that was **not** duplication between
sibling state machines. Its six big functions are built almost entirely out of one idiom, repeated
about twenty times:

```c
for (i = 0; i < 23; i++) {
    if (num < grade_t_meichuuritsu2[i + 1][0]) {
        break;
    }
}

point2 = grade_t_meichuuritsu2[i][1];
```

Find the first row of a threshold table the value falls under, then take that row's score.
Each occurrence differs in **three** things: the table, its row count, and the value being
compared. Recipe D allows one differing value, and the prohibition on parameterising more
than one is the point of that rule, so the idiom cannot be shared. Recipe C does not reach
it either - the runs are not identical. Recipe E does not apply, because the loops are not
nested: they sit at depth 1, one after another.

That left `get_offence_total`, `get_defence_total`, `get_ex_point_total`,
`makeup_final_grade`, `grade_makeup_stage_parameter` and `get_tech_pts_total` all flagged,
with splitting any of them leaving both halves over the threshold, for the arithmetic
reason recorded above.

**This is now Recipe T.** The project owner authorised the narrow extension the paragraph
above asked for: a helper may take a *table, its length, and the value to look up* when the
extracted body is character-for-character identical across every call site and each call
site passes its own table verbatim. It is mechanically checkable, and it is strictly
narrower than Recipe A. See *Recipe T - Shared Table Scan* above for the preconditions.

Applied, it took `grade.c` from **8.67 to 10.00** in eleven commits, one per function: the
Complex Method went with the first, and each remaining Bumpy Road cleared as its own
function's scans collapsed. The one finding Recipe T did not reach, `makeup_spp_frdat`,
was three unrelated loops and wanted an ordinary Recipe E.

## The verification loop

After **every** commit:

```bash
cmake --build build
```

`cmake` is not on PATH by default on the Windows dev machine. Prepend the toolchain:

```bash
export PATH="/c/msys64/mingw64/bin:$PATH"
```

A single-file change rebuilds and links in roughly 30 seconds.

Then check that no constant moved:

```bash
python tools/refactor_guard.py <file>
```

This extracts every numeric, string, and character literal from the file before and after
your change and compares the counts:

- **FAIL** - a value vanished from the file, or a count dropped while another rose. That
  is a substituted constant (`30` became `31`) or deleted logic. Revert.
- **WARN** - counts only dropped and every value is still present. That is what
  deduplication looks like. Recipes **D** and **P** both produce it legitimately: pulling
  a repeated condition into one named predicate removes copies of its literals without
  changing any of them.
- **WARN** - counts only rose. A new guard clause brings its own `return 0`.
- **WARN** - counts dropped and only `0`/`1` rose. That is extract-to-predicate: a
  duplicated block became a named boolean helper, so the call sites lost the block's
  literals and the helper brought its own returns. Confirm the helper *is* the block
  rather than a rewrite of it. (Recognised since 2026-09-15, on the project owner's
  directive - before that the guard scored this legal Recipe D shape as a substitution
  and blocked it.)
- **OK** - nothing changed.

A `WARN` is not a pass mark, it is a request for a second look. The tool cannot tell
deduplication apart from *deleting* one copy of a duplicated block, so a human confirms
those before they land. `--strict` turns warnings into failures.

Then check that no call went missing:

```bash
python tools/refactor_guard.py --calls <file>
```

This compares the multiset of **called function names** before and after, so a call
dropped or duplicated inside a rewritten branch shows up even when every literal still
matches. It is the check that makes Recipes C and X cheap to trust; run it whenever you
extract or split.

Two shapes are expected, both measured against real campaign commits:

| Recipe | Signature |
| --- | --- |
| E, G, X (extract, flatten, split) | **nothing removed**; each new helper `+2` - its definition and the one call that replaced the block |
| C, D (deduplicate) | the shared callees drop by the number of copies removed; the helper is `+1` per call site and `+1` for its definition |

A name that disappears from the file entirely is a **FAIL**: something was deleted, not
moved. Anything else that does not match the two rows above wants explaining before it
lands.

**After a Recipe S split, check the group rather than each file.** A split moves whole
functions, so each file on its own reads as calls vanishing or appearing; only the union
is meant to be unchanged:

```bash
python tools/refactor_guard.py --calls --combined <old-file> <new-file>
```

`OK combined group (N call sites unchanged)` is the result a clean split gives, and it is
strong evidence: it says every call in the original is still made, from one of the two
files, the same number of times.

**Renaming a helper you extracted earlier reads as a vanished call**, because the tool
sees only that the old name is gone. Renaming a `static` that no other file can see is
legal - the prohibition is on renaming functions across files - so declare it and run
again:

```bash
python tools/refactor_guard.py --calls --renamed old_name=new_name <file>
```

The declaration rewrites the old name in the *before* fingerprint and nothing else, so
the counts still have to balance: a call genuinely dropped in the same commit still
FAILs. Do not reach for it to silence a name you did not rename.

It does not see *reordering*. Nothing mechanical in this repo does, which is why the
prohibition on reordering side effects is absolute rather than advisory.

Then re-measure with the CodeScene MCP server:

```
code_health_score(file_path="<absolute path to the file>")
```

| Result | Action |
| --- | --- |
| Build fails | Fix it, or revert. Never commit a broken build. |
| Guard FAILs | `git checkout -- <file>`. No exceptions. |
| Score up | Keep it. Commit. Next function. |
| Score flat or down | **Do not revert yet.** Check the review first - see below. |

### A flat score does not mean a failed refactor

The file score is a single aggregate over every function in the file. In a 5,000-line
module with 111 functions, fixing one function can move it by less than the score's
resolution. Reverting on a flat score would throw away good work.

So when the score does not move, run the detailed review and ask a sharper question:
**did the smell I targeted actually go away?**

```
code_health_review(file_path="<absolute path>")
```

- The function **left** a category it was in (Deep Nested Complexity, Bumpy Road, Large
  Method), or its cyclomatic complexity dropped: **keep the change and commit.**
- The function is still listed with the same numbers: **revert.** The transformation did
  not do what you intended.

A real example from this campaign. Extracting one nested switch out of `Menu_Select` left
the file score at 2.25, unchanged. The review showed the function had dropped out of three
categories and its complexity had fallen from 19 to 12. Keeping it was correct.

Score is the campaign-level signal. Per-step, the review is the signal.

### When the score falls but the review improves

Sometimes a correct transformation *lowers* the score. This is real, it is measurable, and
it happened three times in the September 2026 sessions.

The cause is always the same: decomposing one state machine makes it look like its
sibling, and the duplication detector prices that resemblance above the complexity you
removed. Splitting `ORO_JA_Term`'s airborne half cost 0.25, and a larger version of the
same split cost 0.87. Fixing `Command_Attack`'s complexity cost 0.57 - until the parameter
structs made its two siblings stop reading alike, after which the identical change was
free and took the file to 10.00.

**Duplication-by-shape between sibling state machines is an artifact of decomposition, not
a defect introduced by it.** So:

- Record both numbers in the commit message, and say which smell went.
- Prefer the variant with the better *review*, not the better score, and say so.
- Where the cost is large and the smell is small - one point of cyclomatic complexity
  against 0.87 of score - keep the higher score and record the file as plateaued.
- Never "fix" sibling similarity by merging two state machines that differ only in their
  state numbering. That needs a literal change and is forbidden.

### The eff09 family: separate the shape from the substance

`eff09.c` and its three split files all sat at 8.54-8.81 on Code Duplication, and the
first reading was that the whole family was immovable. That was wrong, and the way it was
wrong is the lesson.

Their duplication is two different things wearing the same label:

- **Substance - identical runs that really are shared.** A frame gate repeated seven
  times, a four-line show-and-init entry repeated three times, a speed-table load, a
  placement block. All of it is ordinary Recipe C and P work and all of it pays.
- **Shape - dispatchers that differ only in which functions they call.** `eff09_4000`
  against `eff09_27000`, `eff09_17000` against `eff09_18000`: same switch, same labels,
  different callees. Merging them means passing function pointers, which is not a recipe
  in this catalogue.

Clearing the substance moved three of the four files:

| File | Before | After |
| --- | --- | --- |
| `eff09_animation.c` | 8.81 | **10.00** |
| `eff09_endgame.c` | 8.81 | 9.38 |
| `eff09_late.c` | 8.81 | 9.38 |
| `eff09.c` | 8.54 | 8.54 |

`eff09.c` is the one that does not move. The same extractions were applied to it twice -
the gate, the entry, the ball style, and the 18 copies of the draw pair, 36 lines into 18
- and the score did not shift, because the file is 1,200 lines carrying **five
independent near-miss pairs**, and removing shared runs from around them leaves every
pair standing. Two of those pairs are `adjust_sean_ball_left`/`_right`, which differ in
five values and contain an arcade-accurate bug that must stay.

**The rule to take away:** when a file reports Code Duplication, separate the shared runs
from the shared shape before calling it a plateau. Extract the runs, measure, and only
then judge. A file can look identical to its neighbour in the review and still have most
of a point in it.

### Between two twin arms, extract from one of them only

When a function's two arms are structural twins that Recipe D may not merge - they differ
in a comparison operator, an offset, or a constant - extracting the *same* helper from both
creates a new twin pair, and the duplication the metric then sees costs more than the
complexity removed. Extracting from **one arm only** keeps the win and creates no twin.

Measured on `pls03_super_arts.c`'s `try_grounded_ex_super` / `try_airborne_ex_super`, whose
gate chains differ only in a DIP constant and `>` against `<`:

| What was done | Score |
| --- | --- |
| baseline | 5.32 |
| gate chain extracted from **both** arms | 5.29 |
| gate chain extracted from the **grounded arm only** | **5.46** |

**Unless extracting both brings the parent under the threshold.** That is the deciding
question, and it is worth checking before settling for one arm. Measured on the same
file's `check_super_arts_attack_dc`, cc 29 with a strength loop in each arm:

| What was done | Score |
| --- | --- |
| baseline | 5.67 |
| loop extracted from the **grounded arm only** | 5.71 |
| loop extracted from **both** arms | **5.87** |

Here both wins, because with both loops gone the parent drops from cc 29 to under 9 and
loses its Complex Method *and* Large Method findings - more than the new twin pair costs.
In the gate-chain case above, extracting both left the parent flagged anyway, so the twin
penalty was all that changed.

So the rule is not "always one arm". It is: **extract both only if that clears the
parent's findings; otherwise extract one.** Same arithmetic as *Do not extract an arm that
is still too big*, applied to the parent rather than the piece.

**And weigh each arm against the twin family it would join, not just its own twin.** The
cost of a new near-twin grows with how many near-twins the file already has, so an arm that
would be the third or fourth member of a family is dearer than one that would be the
second - even when the parent clears either way.

Measured on `plmain.c`'s `sag_union_1`, cc 17 with two liftable states:

| What was done | Score |
| --- | --- |
| baseline | 6.94 |
| states 2 and 4 both lifted | 6.69 |
| state 4 only | **7.09** |

The parent cleared its finding in both of the last two. The difference is that state 2's
helper would have been the *third* `spend_or_abandon`-shaped function in the file, while
state 4's has no close relative. So: before lifting an arm, look at what is already in the
file, not only at what the arm is paired with.

Where one arm is the answer, the asymmetry reads slightly odd, so say in the commit message
that the other arm was left inline deliberately and why.

This is the same force behind *Two mirrored arms are cheaper left together* below; the
difference is that here one arm can still be improved for free.

### Grouping case labels duplicates them, and the guard will say so

A switch whose arms share an opening guard looks like a Recipe C candidate: group the
labels into one arm, write the guard once, and move what differs into a helper that
switches on the same expression again. It is behaviour-preserving, and it is not legal.

Each label ends up written **twice** - once in the grouped arm and once in the helper - and
`refactor_guard.py` reports it as a substituted constant:

```
FAIL  a constant was substituted
      removed x8  num 0
      added   x1  num 14 ... 30   (all nine labels)
```

The rule that catches this is the same one that forbids renumbering states, and the hazard
is real rather than bureaucratic: a dispatch written in two places will eventually be
changed in one of them. `pls03.c`'s `check_nm_attack` was refactored this way, measured
-0.04, tripped the guard, and was reverted.

**A FAIL is a stop, including when the replay is clean and the reasoning looks sound.** The
replay agreed with that change; it was still wrong.

### Retry a rejected extraction once the file has improved

A measurement is only valid for the file as it stood. The cost of a new
near-twin is weighed against everything else the file is already carrying, so an
extraction that measured negative early can measure strongly positive later,
with no change to the extraction itself.

Measured on `plpdm.c`'s `set_dm_hos_flag_sky`. Naming its two distance checks -
which cannot be merged, since both limits differ - was tried twice, the same
edit both times:

| When | File score before | After |
| --- | --- | --- |
| with five other findings still open | 7.15 | 6.99 |
| after those were cleared | 8.66 | **9.53** |

The second time it was worth +0.87. Nothing about the edit changed; what changed
is that it had become the file's last real finding, so clearing it was worth far
more than the twin pair costs.

So: keep a note of what you rejected and why, and come back to it when the file
is close to done. The rejections worth revisiting are the ones refused for
duplication cost rather than for a rule.

### Clear the functions just over the threshold first

A finding is worth score only while it exists. Taking a function from cc 25 to cc 20 keeps
the Complex Method finding and pays almost nothing; taking one from cc 10 to cc 6 removes
the finding outright and pays properly. So work the list from the **bottom** - the
functions barely over the threshold - not the top.

Measured on `pls03.c` in one run, all of them one small Recipe E or P each:

| Function | cc before -> after | Score |
| --- | --- | --- |
| `check_leap_attack` | 10 -> 6 | +0.14 |
| `check_chouhatsu` | 10 -> 8 | +0.15 |
| `ex_slot_is_allowed` | 11 -> 5 | +0.17 |
| `meoshi_cancel_gate` | 10 -> 6 | +0.16 |

Together **+0.62**, from four extractions of a dozen lines each. The same file's
`check_special_attack_airborne`, a much larger piece of work on a cc 23 function, was worth
+0.13 on its own.

This does not mean never touch the big ones - a big function that can be brought *under*
the threshold is worth more than any of these. It means: when a big one cannot be brought
under, spend the time on the small ones instead.

### For a Complex Conditional, name the whole condition, not its parts

CodeScene counts the logical operators in the expression **at the branch**. Replacing the
operands with named predicates leaves the operator count unchanged, so the finding stays.

Measured on `pls03.c`'s `decode_wst_data`. Its 0xA000 arm tested three things at once:

```c
if ((wk->wu.mvxy.a[1].sp > 0) && (lever == (wk->cp->sw_new & 0xF)) &&
    cmd_ex_check(wk->wu.xyz[1].disp.pos, cmd_ex)) {
```

Naming the lever test and the height test separately measured **flat** and left both
findings in place - the arm still joined three terms with `&&`. Replacing the whole
condition with one predicate, `rising_with_lever_at_height(wk, lever, cmd_ex)`, cleared
both findings and was worth **+0.06**.

So: one name for the whole `if`, even when the parts have good names of their own.

**But the opposite is true for Complex Method.** That finding counts every `&&` and `||`
in the function wherever they sit, so moving terms into named helpers is exactly what
reduces it. `grade.c`'s `guard_was_not_deliberate` was five lines at cc 9 - a seven-term
conjunction and nothing else - and splitting it into three named halves took it to cc 3 and
cleared the finding, worth +0.13.

So check which finding you are looking at before choosing:

| Finding | What helps |
| --- | --- |
| Complex Conditional | one name for the **whole** condition at the branch |
| Complex Method driven by a long boolean | names for the **parts**, moving the operators out |

### Recipe X pays only if both halves come in under the threshold

The same arithmetic as *Do not extract an arm that is still too big*, applied to a split
dispatch. Splitting a switch in two leaves each half with roughly half the arms - and a
switch's cyclomatic complexity is driven by its arm count, so a switch far above the
threshold does not get both halves below it.

Measured on `decode_wst_data`, a twelve-arm switch at cc 29. Splitting it six and six
measured **-0.03** (5.35 -> 5.32): the two halves came out at about cc 15 and cc 14, so the
file gained a second Complex Method instead of losing one, and the two halves then read as
near-twins into the bargain. Reverted.

Before splitting a dispatch, divide: if arms/2 is still over 9, the split will not pay.
`decode_wst_data` is recorded as a plateau for this reason - its complexity is its twelve
command encodings, and the only way to reduce the arm count is to renumber or merge
encodings, which is a literal change.

### Do not extract an arm that is still too big

Recipe E on a `switch` arm pays only if the piece you lift out comes in **under the
thresholds** - cyclomatic complexity 9 and 70 lines. If it does not, the file trades one
flagged function for two and the score falls.

Measured on `plmain.c`'s `sag_union_ps2`. Lifting out case 2's whole `gt2` dispatch was
worth **+0.30** (4.38 -> 4.68): the piece landed at cc 16 and the parent dropped from
cc 44. Going one level further and lifting the `gt2 == 1` arm out of *that* cost
**-0.14** (4.68 -> 4.54): the new helper was 74 lines at cc 22, so the file gained a
second Complex Method *and* a second Large Method while the parent only fell to cc 16.
Reverted.

Check the arm's own size before extracting it. When an arm is too big to help as a
function but too complex to leave, name a **pure predicate inside it** instead - that
lowers the parent's complexity without creating a second flagged function.
`vital_drain_is_paused` in `check_omop_vital` is the worked example, +0.05 where an arm
extraction would have cost.

### Never apply the same split across an already-duplicated family

The rule above is about two arms inside one function. This one is about several
functions, and it costs more.

When CodeScene already reports a group of functions as duplicates of each other, applying
the *same* extraction to each of them multiplies the duplication instead of reducing it.
Three near-identical functions split the same way become six near-identical halves, and
the detector prices the larger group.

Measured in `pls03.c`: `check_full_gauge_attack`, `check_full_gauge_attack2` and
`check_super_arts_attack_dc` were one duplication group of three. Splitting each on its
grounded/airborne seam - the identical split that had just paid on `check_special_attack`,
which was *not* in a duplication group - took the file 2.26 -> 2.12 and was reverted.

**So check the duplication groups in the review before extracting.** A split that pays on
a lone function will usually cost on a member of a duplicate family. Fix the family first,
if a legal recipe can, or leave it alone.

### Two mirrored arms are cheaper left together

The commonest remaining smell in this codebase is a Bumpy Road whose two bumps are the
arms of one `if (rl_flag)` - the same statements written once for each facing, differing
in a sign, an offset or a comparison. Extracting each arm into its own helper clears the
Bumpy Road and is perfectly legal. It also **costs between 0.2 and 0.6**, every time,
because the two helpers are then near-identical functions and the duplication detector
prices that above the nesting removed.

Measured this way: `effd1.c` 9.92 -> 9.38, `effe9.c` 9.92 -> 9.38, `effl7.c` 9.61 -> 9.09,
`effect.c` 9.38 -> 9.16.

So: leave a mirrored pair inline and record the file as plateaued. The move is still worth
trying once on a file where the two arms are *not* mirrors - where they do genuinely
different work - and there it usually pays.

### Overall Code Complexity is a whole-file average

`Overall Code Complexity` is the mean cyclomatic complexity over every function in the
file, so it moves when you add a function, not only when you change one. A single
extraction can clear `Complex Method` on the function you targeted and trip
`Overall Code Complexity` for the file, leaving the score lower than before - `effl7.c`
measured 9.61, then 9.31 after one extraction, then 10.00 after three.

When a file shows `Overall Code Complexity`, measure the *set* of extractions together
rather than reverting the first one that looks like a loss. Say so in the commit message
when that is why several extractions share a commit.

---

### What the guard does not cover

The guard checks constants, not control flow. An inverted condition with no literal change
passes it cleanly - so it reduces risk, it does not remove it. That is why Track B stays
closed until statcheck runs.

Use `code_health_review` (not just the score) when you need to see *which* smells remain -
the score alone will not tell you whether you hit the right problem.

---

### Recipe S first, then the duplication it exposes

`charset_commands.c` was the clearest case measured so far. At 132 functions it carried a
*Number of Functions in a Single Module* finding that no extraction could touch, and a Code
Duplication web of 34 functions in 19 groups spanning the whole file.

Moving the 21 position-and-speed opcodes into `charset_position.c` (Recipe S) took the
original from 7.00 to 7.96 and gave the new file 7.27. That is the smaller half of the win.
The larger half is what the split made possible: with each duplication family now whole
inside one file, five ordinary Recipe C and Recipe D merges took `charset_commands.c` to
10.00 and `charset_position.c` to 9.09.

The rule that made the split work is the one already written down for Recipe S - **move a
duplication family whole** - and the way to check it is to run the review first and list
which functions share a group. A split that cuts through a group leaves both halves with a
finding neither file can clear on its own.

### A duplicated call site is worth extracting even when the score does not move

`plmain.c` had eight places that cleared the same three fields in the same order. CodeScene
reported none of them: three lines is below its duplication threshold. Extracting them into
`abandon_super_art` moved the score from 8.03 to 9.09 anyway, because the *near-miss*
groups it did report - three `spend_or_abandon_*` functions that differed in two or three
statements each - stopped reading as near-misses once their shared tail was a call.

So when the review reports a duplication group that Recipe D refuses (too many
differences), look for a shorter identical run **inside** the group and extract that
instead. It is Recipe C, it is always legal, and it can dissolve a group that could not be
merged.

### An arm that ends in `break` inside a `while (1)` is not a plateau

`pls02.c`'s `set_field_hosei_flag` and `plcnt.c`'s `settle_check` both used the
`while (1) { ... break; ... }` idiom to mean "retry" or "fall out to the tail". Two
different attempts to remove it were measured:

- `settle_check`: extracting the arm's body into `settle_double_ko` and leaving the loop
  alone, +0.55.
- `set_field_hosei_flag`: rewriting the loop as two functions and a flag, -0.70 at 8.65 and
  -0.77 again at 9.31 after the file had improved.

The difference is the twin. `settle_double_ko` has no sibling; the two wall sides of
`set_field_hosei_flag` differ in three places and become a duplication group the moment
they are separate functions. **Extract the body, keep the loop** is the move that pays.

---

### Break the twin first, then split the survivor

`cmd_main_checks.c` held two dash commands, `check_10` and `check_12`, that
CodeScene reported as a duplication pair and that were both Complex Methods at
cc 18 and 19. Splitting either one alone had been the obvious move; splitting
both was the obvious trap. What actually worked was a sequence:

1. **Recipe X on one of them.** States 2 and up of `check_10` moved behind
   `default:` into a helper. +0.28, and `check_10` left the duplication group -
   it no longer looks like its twin.
2. **Then read the twin again.** With `check_10` reshaped, `check_12`'s states
   2, 3 and 4 turned out to be byte-for-byte identical to the helper that had
   just been extracted. It became a **Recipe D** call to the same helper, not a
   second split. +0.36, fifty duplicated lines gone, and `check_12` left the
   group too.

The lesson generalises: when two functions are reported as a duplication pair
and both are too complex, do not split them symmetrically. Split one, then
re-read the other against the helper that came out. Often the second function
can *call* the first's helper, which is a merge rather than a second split -
and a merge never creates a new twin.

### The guard's literal fingerprint blocks array-typed parameters

Recipe D on a family of table scans wants a parameter of array type:

```c
static s16 sa_stock_points(s16 ix, const s16 table[][2]);
```

That `2` is a literal new to the file. Against the literals the merge removes -
four `0`s, four `1`s and two `5`s from the three loops it collapses -
`refactor_guard.py` reads the combination as *a constant was substituted* and
**FAILs**. The refactor is legal; the fingerprint cannot tell it from a
substitution.

The fix is to name the row type in a header:

```c
/* grade.h */
typedef const s16 GradeRow[2];
```

and write the parameter as `const GradeRow* table`. The `.c` file then gains no
literal at all and the guard reads the expected deduplication signature; the
header's own run is the legal "literals added, none removed". This is not a way
around the guard - the transformation is the same one either way - it is a way
to write the type where types belong so the fingerprint stays readable. It is a
precondition of **Recipe T**, not an optional tidying.

### The table-scan idiom, measured

`grade.c` is built from this shape, seventeen times:

```c
    for (i = 0; i < N; i++) {
        if (VALUE < TABLE[i + 1][0]) {
            break;
        }
    }

    point += TABLE[i][1];
```

Three things differ between instances: the table, its length, and the value
scanned. That is more than one, so **Recipe D does not apply**; the
`grade_table_points(table, count, value)` helper the code obviously wants is
**Recipe T**, added to the catalogue afterwards. The rules below are what was
legal before Recipe T existed, and they still hold for a family Recipe T's
preconditions refuse:

- **Group the scans by what they score**, not one function per scan. Nine scans
  in one helper is fine; three helpers of three scans each read as duplicates of
  one another and cost 0.09.
- **Recipe D still applies where only the table differs.** The super-art score
  scanned the same length with the same value from three tables; that is one
  difference and it merged.
- **Extract the non-scan work.** The ratio calculations in `get_offence_total`
  and `get_defence_total`, and the all-clear bonus in `makeup_final_grade`, are
  ordinary Recipe E extractions and were worth 0.20, 0.15 and 0.11.

### Recipe F pays only once the arms are already one statement

`pls03.c`'s `check_nm_attack` is the case that shows the ordering. Its nine stance arms
each read:

```c
case 20:
    if (is_blocked_by_hikusugi(wk)) {
        return 0;
    }

    select_nm_attack_level_3010(wk, kos, 3);
    break;
```

Applying Recipe F straight to that buys **nothing**: the arm keeps its `if`, so each arm
still costs two decisions and the cyclomatic complexity does not move. The guard cannot be
hoisted out of the switch either - `default:` does not have it.

What worked was three commits in this order, +0.48 between them:

1. **Recipe E first.** The whole switch answers one question, so it becomes
   `begin_nm_attack(wk, kos)` returning 0/1 and the caller keeps its early return and its
   tail. That clears Large Method, and - the point - it makes the arms' `return 0` and
   their fall-through into two values of one return.
2. **Then Recipe F.** Now each arm collapses to
   `return start_nm_attack(wk, kos, 3, select_nm_attack_level_3010);` - one decision, the
   `case` itself. cc 21 -> 12.
3. **Then Recipe X.** Twelve is still over the threshold, but nine one-line arms split
   cleanly: the three jumping stances and the standing fallback move behind `default:`,
   labels unchanged, and both halves come in under it. cc 12 -> cleared.

The general rule: **Recipe F removes the duplicated skeleton, not the branching inside
it.** If an arm still contains a conditional after the shared call is factored out, find
the recipe that turns that conditional into a return value first - usually Recipe E on the
whole dispatch - and apply Recipe F to what is left.

### What a Recipe F commit looks like to the guard

Measured on `charset_position.c` and `pls03.c`:

| Check | Reads |
| --- | --- |
| literal | the deduplication WARN - the duplicate `case` labels and guard returns are gone, every other value still present |
| `--calls` | the shared callee drops by the copies removed; the new helper is `+1` per call site and `+1` for its definition |
| `--calls` noise | `+1 void` and `+N` for the *parameter's* own name - a declarator and a pointer call both scan as calls. Expected, and it appears on every Recipe F |

Without `--fnptr`, each callee now passed by pointer reads as **a call vanished** and the
guard FAILs a legal refactor. Declaring it is not a way around the check: the counts still
have to balance, and a callee that really did vanish would show up as an unexplained
removal.

### A split on a duplicate-family member can still pay, if it clears two smells

*Never apply the same split across an already-duplicated family* stands, but it is a rule
about cost, not a prohibition, and `pls00_normal_states.c` is the case where the cost was
worth paying.

`nm_16000` and `nm_17000` were already one duplication group, and both were Complex Method
at cc 10. Each opens by reading the settled lever direction and picking one of three
landing states, differing only in which three. Extracting that block from both makes
exactly the twin pair the rule warns about - `enter_jump_from_16000` and
`enter_jump_from_17000` are duplicates of each other - and the score still went **7.14 ->
7.55**, because two Complex Methods left the review and only one duplication pair arrived,
into a group that already existed.

The test to apply before making the split:

- **How many smells does it clear?** Two Complex Methods is worth a duplication pair; one
  Bumpy Road usually is not.
- **Was the group already there?** Adding a pair to a group of fifteen functions moves the
  duplication finding hardly at all. Creating the *first* duplication group in a clean file
  is what costs.

Measure it either way - the two cases differ by less than half a point and neither is
predictable from reading the code.

---

### Choose which arms to extract so no two dispatchers end up bare

*Overall Code Complexity is a whole-file average* says to extract until the mean
drops. `plpatuni.c` shows the constraint that comes with it: **an extraction
shrinks its caller, and a state machine shrunk to nothing but `case N: helper(wk);
break;` looks exactly like every other state machine shrunk the same way.**

Measured, three ways, on the same file:

| Set | Extractions | Score | What held it |
| --- | --- | --- | --- |
| none | - | 9.38 | Overall Code Complexity |
| all seven bumpy arms | 7 | **9.09** | Code Duplication across five bare dispatchers |
| the same set minus two | 4 | **10.00** | nothing |

The seven-arm set cleared the mean and lost more than it gained. Two of its
extractions emptied `Att_SHOURYUUKEN` and `Att_SENPUUKYAKU`, which then matched
three dispatchers that were already skeletons, and CodeScene raised a five-function
duplicate group. Leaving those two arms inline cost nothing - four extractions
were already enough for the mean - and no family formed.

So when a file is a set of sibling state machines and the finding is the mean:

1. Count how many extractions the mean actually needs before doing any. It is
   usually fewer than the number of arms that could be extracted.
2. Spend them on the machines that are **least** like their siblings - a distinct
   label set, a distinct opening - and leave the near-twins holding their arms.
3. If the duplicate family forms anyway, check whether Recipe S can separate the
   twins **before** reverting. In `plpatuni.c` it could not: the openings the
   siblings share, `begin_uni_attack` and `begin_uni_attack_at_row`, are `static`
   and called from every part of the file, so no cut avoids widening a `static`.
   That is forbidden outright, which made the smaller extraction set the only way
   through.

---

### Against a twin family, share what they agree on before you split what they do not

*Choose which arms to extract so no two dispatchers end up bare* says to spend the
mean's extractions on the machines least like their siblings. There is a better move to
try first, and it broke three plateaus this campaign had written off.

**A twin family has two kinds of material in it.** The arms are near misses - they differ
in a state number, a table, an operator - and lifting one makes another twin. But the
*runs inside those arms* are very often identical, character for character, across the
whole family: an opening, a marker, a launch tail. Those cost nothing to share, because
the helper has exactly one definition and so has nothing to twin with, and each one still
counts in the denominator of the mean.

So the order is: **Recipe C and Recipe D over the shared runs first, and only then Recipe E
over whatever arms the mean still needs.** Every shared run you take is one fewer arm you
have to risk.

Measured on three files whose notes all said the smells could not both be cleared:

| File | Was | Now | What the runs were |
| --- | --- | --- | --- |
| `plpdm_states_late.c` | **10.00** | *was 9.38.* Three shared runs and one ordinary extraction |
| `plpdm_states.c` | **10.00** | *was 9.38.* Six shared runs, no arm lifted - see *Against a twin family, share what they agree on* |
| `plpat07.c` | **10.00** | *was 9.38.* One shared opening changed the count of arms needed from five to four, and four could be chosen to miss the twins |

`plpat07.c` is the clearest statement of why it works. Three separate five-extraction sets
had been measured there, each clearing the mean and each landing back on 9.38 with a new
duplicate pair. One shared opening changed the count needed from five to four, and four
was few enough to take only from the machines with no close relative.

**Finding the runs is mechanical.** Search the file for repeated contiguous line
sequences - two lines up to six or so, indented, inside function bodies - and rank them by
how many copies each has. Anything with two or more copies and no differences is a Recipe C
candidate; anything differing in one value is Recipe D's.

A caveat worth stating: where a run is shared across a **Recipe S split**, each file needs
its own `static` copy, because widening a `static` to bridge the two is forbidden.
`plpdm_states.c` and `plpdm_states_late.c` carry three such pairs. That is real duplication
that the metric does not see, and it is the price of the split rather than a reason to
avoid it.

### A file can be too big for its own mean

`plmain.c` is the case that shows what *Overall Code Complexity is a whole-file average*
means in practice. At 1430 lines and 65 functions its mean was 4.4 against a threshold near
3.8, and **one extraction moves a 65-function mean by about 0.01**. Three good extractions
measured flat. Two more measured **8.03**, because in a file that size there is always
something for a new helper to twin with, and past 1400 lines *Lines of Code in a Single
File* is waiting as well.

Three Recipe S splits fixed it, in this order, and none of them moved the score on its own:

| Split | plmain.c after | What it took to clear the new file |
| --- | --- | --- |
| the port's super-art states -> `plmain_ps2_arts.c` | 1203 lines, 9.38 | 4 extractions, 9.38 -> 10.00 |
| the vitality drain -> `plmain_vital.c` | 1051 lines, 9.38 | 2 extractions, 9.38 -> 10.00 |
| the gauges and CPS3 arts -> `plmain_arts.c` | 606 lines, 9.38 | 4 extractions, 9.38 -> 10.00 |

Then the three extractions that had measured flat in the 65-function file took what was
left of `plmain.c` from 9.38 to **10.00** unchanged. Nothing about them got better; the
denominator got smaller.

**So when a file shows only Overall Code Complexity, count its functions before extracting
anything.** Under about twenty, two extractions will clear it - `bbbscom.c` and
`manage_result.c` each took one commit. Over about forty, extraction is the wrong tool and
the file needs splitting first.

One more thing a split does, worth knowing because it flatters the score: **a duplication
pair in different files is not a duplication finding.** `sag_union_0` and
`sag_union_ps2_active` are two dispatchers that share a shape and agree on nothing else;
the first split put them in different files and the finding went away without a line of
either changing. Say so in the commit message when it happens - it is a real improvement in
how the code is organised, but it is not the detector being satisfied by better code.

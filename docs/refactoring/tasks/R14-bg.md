# R14 - refactor `src/sf33rd/Source/Game/stage/bg.c`

| Field | Value |
| --- | --- |
| Baseline Code Health | **3.62** / 10 (Red) |
| Exit target | **plateau** - no legal recipe raises the score further; record it |
| Hand-off point | **4.00** leaves the Red band. A small model stops there and hands on |
| Stretch target | 5.15 (CodeScene industry-average scenario) |
| File size | 1430 lines |
| Git churn | 11 commits |
| Risk tier | **MEDIUM** - Background/stage rendering; limited gameplay coupling. |
| Track | **A** (start now) |
| Recipes needed | P, G, E |
| Suitable for | strongest available agent |

Recipe P is a copy-paste transformation and is safe for a small model. Recipe E needs
data-flow reasoning about which variables cross the extracted boundary - that is where
a weaker model fails silently while the build still succeeds. If this task is assigned
to a smaller model, restrict it to the Recipe P steps and leave the rest.

> [!NOTE]
> **Track A: you may start now.** This file sits outside the in-round simulation, so a
> compile-verified mechanical refactor is acceptable before statcheck exists. You are
> still limited to the transformations in the PLAYBOOK - no logic changes.

## 1. Read these first

- [`../PLAYBOOK.md`](../PLAYBOOK.md) - the only transformations you are allowed to apply.
- [`../README.md`](../README.md) - campaign rules and the definition of done.

## 2. Record the baseline before touching anything

Call both CodeScene MCP tools and keep the output for your report:

```
code_health_review(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/stage/bg.c")
code_health_score(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/stage/bg.c")
```

The score must read 3.62. If it does not, this file changed after the task
was written - stop and report that instead of proceeding.

## 3. What CodeScene flags here

| Smell | Severity (1-3) |
| --- | --- |
| Bumpy Road Ahead | 3 |
| Deep, Nested Complexity | 3 |
| Lines of Code in a Single File | 2 |
| Overall Code Complexity | 2 |
| Complex Method | 2 |
| Complex Conditional | 2 |
| Excess Number of Function Arguments | 2 |

This file also trips **Lines of Code in a Single File**. Splitting it is a *later* step.
Do not start by moving code between files - reduce function-level complexity first.

## 4. Target functions, highest leverage first

| # | Function | Line | Cyclomatic | Nesting | Bumps |
| --- | --- | --- | --- | --- | --- |
| 1 | `scr_trans` | 542 | 109 | 9 | 16 |
| 2 | `Bg_Texture_Load_Ending` | 422 | 16 | 4 | 3 |
| 3 | `bgDrawOneScreen` | 1059 | 9 | 6 | - |
| 4 | `Bg_Texture_Load_EX` | 235 | 19 | - | 6 |
| 5 | `Bg_Kakikae_Set` | 72 | 12 | - | 2 |
| 6 | `Ed_Kakikae_Set` | 166 | 9 | - | 2 |

Thresholds for C: cyclomatic complexity under 9, nesting depth under 4.

## 5. Steps

Work **one function at a time, in the order above**. After each function run the
verification in section 6. Do not batch several functions into one change.

#### Step 1: `scr_trans` (line 542)

- **Recipe G (guard clauses)** - nesting is 9, target is under 4.
- **Recipe E (extract function)** - 16 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(bg): simplify scr_trans`

#### Step 2: `Bg_Texture_Load_Ending` (line 422)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe E (extract function)** - 3 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(bg): simplify Bg_Texture_Load_Ending`

#### Step 3: `bgDrawOneScreen` (line 1059)

- **Recipe G (guard clauses)** - nesting is 6, target is under 4.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(bg): simplify bgDrawOneScreen`

#### Step 4: `Bg_Texture_Load_EX` (line 235)

- **Recipe E (extract function)** - 6 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(bg): simplify Bg_Texture_Load_EX`

#### Step 5: `Bg_Kakikae_Set` (line 72)

- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(bg): simplify Bg_Kakikae_Set`

#### Step 6: `Ed_Kakikae_Set` (line 166)

- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(bg): simplify Ed_Kakikae_Set`

### When to stop

At 4.00 the file has left the Red band. If you are a small model, **stop there** and
hand the file on - the transformations that remain need data-flow reasoning. If you are
not, keep going to the plateau, the point where no legal recipe raises the score
further, and record that figure and its reason in your report.

## 6. Verification - run all three after EVERY step

**Step 1 - it still builds.**

```bash
export PATH="/c/msys64/mingw64/bin:$PATH"
cmake --build build
```

**Step 2 - no constant was removed or altered.**

```bash
python tools/refactor_guard.py src/sf33rd/Source/Game/stage/bg.c
```

A `FAIL` here means a literal disappeared or changed value - that is not a legal
refactor. Revert immediately. A `WARN` about *added* literals is normally fine
(a new guard clause brings its own `return 0`).

**Step 3 - the metric improved.**

```
code_health_score(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/stage/bg.c")
```

- Score went **up**, build passed, guard passed: commit and continue.
- Guard **failed**: revert that step, no exceptions.
- Score **flat or down**: do *not* revert yet. Run the review and check whether the
  function you targeted left a category or dropped in complexity:

```
code_health_review(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/stage/bg.c")
```

  This file has 1430 lines, so one function can move the aggregate score by
  less than its resolution. If the targeted function left Deep Nested Complexity,
  Bumpy Road, or Large Method, or its cyclomatic complexity fell: **keep and commit.**
  If it is still listed with the same numbers: revert with
  `git checkout -- src/sf33rd/Source/Game/stage/bg.c` and move on.

## 7. Definition of done

Either of these is a successful outcome:

- [ ] `code_health_score` >= 4.00, **or**
- [ ] every function in section 4 has left at least one smell category, with the
      before/after review pasted into the report
- [ ] `cmake --build build` succeeds with no new warnings
- [ ] `python tools/refactor_guard.py src/sf33rd/Source/Game/stage/bg.c` reports no removed literals
- [ ] Each commit covers one function and uses the message format above
- [ ] No behaviour change: no edits to literals, arithmetic, comparison operators, or the order of side effects
- [ ] Final `code_health_review` captured in the report

## 8. Why this file is worth the effort

CodeScene projects that lifting this file to 5.15 yields, as a 90% confidence interval:

- **14% to 34%** fewer defects
- **2% to 14%** less development time

Model-based projections, not guarantees.

## 9. Report back

Reply with exactly this, filled in:

```
Task: R14 (src/sf33rd/Source/Game/stage/bg.c)
Baseline score: 3.62
Final score:    <x.xx>
Steps completed: <n> of 6 in this wave
Smells cleared:  <function: which category it left, or 'none'>
Steps reverted:  <n>   (list which, and why)
Build: PASS / FAIL
Commits: <sha list>
Unsure about: <free text, or NONE>
```

---

## Report - 2026-09-18 (the stage-folder wave)

```
Task: R14 (src/sf33rd/Source/Game/stage/bg.c), widened to the whole stage folder
Baseline score: 7.32 for bg.c at the start of this wave (3.62 at campaign start)
Final score:    9.09, plateaued
Steps completed: 20 commits across 8 files
Smells cleared:  bg.c        - Excess Number of Function Arguments (14 functions),
                               Lines of Code in a Single File (1351 -> 730)
                 bg_sub.c    - Bumpy Road Ahead (3 functions),
                               Lines of Code in a Single File (1106 -> 690)
                 ta_sub.c    - Code Duplication, all of it
                 bg090.c     - Overall Code Complexity
                 bonus_bg.c  - Code Duplication
                 bg_textures.c - Overall Code Complexity
Steps reverted:  5 (see below)
Build: PASS
Replay: per-commit gate on every control-flow and rollback-state change,
        plus the wide gate, 30 seeds x 3600 frames against the branch point -
        108,000 saved states, all identical
Unsure about: NONE
```

### Where the folder stands

| File | Before | After |
| --- | --- | --- |
| `bg.c` | 7.32 | **9.09** - plateau |
| `bg_textures.c` | - | **10.00** - split out of `bg.c` |
| `bg_sub.c` | 7.38 | **9.09** - plateau |
| `bg_zoom.c` | - | **8.54** - split out of `bg_sub.c`, plateau |
| `ta_sub.c` | 9.38 | **10.00** |
| `bg090.c` | 9.38 | **10.00** |
| `bonus_bg.c` | 9.38 | **10.00** |
| `bg000.c` | 9.92 | 9.92 - plateau, unchanged |
| the other 20 files | 10.00 | 10.00 |

### What moved `bg.c`

| Move | Score |
| --- | --- |
| Recipe A x14: the draw context into the whole `draw_*` family, `TextureSource` for the loaders, `ChipRect` and `ScreenDraw` for the two primitives | 7.32 -> **7.55** on the fourteenth |
| Recipe F: one body for the three remapped tile passes | flat, three functions left the web |
| Recipe D: one rewrite-texture loader for stage and ending | flat, three left the web |
| Recipe D: one reload for a rewrite slot, three call sites | flat, two left the web |
| Recipe D: one zoom offset for both frame axes | -> **7.78** |
| **Recipe S: the texture loading -> `bg_textures.c`** | -> **8.28**, 1287 -> 758 lines |
| Recipe N: one ending chip remap over a range | flat, a pair became one |
| Recipe N: one ending `g_kakikae` remap over a range | -> **8.81** |
| Recipe D: one chip draw and list restore | -> **9.09** |

The Excess Number of Function Arguments finding is the clearest illustration in this
campaign of why a flat score is not a failed step: thirteen consecutive commits measured
7.32, and the fourteenth measured 7.55, because the finding is on the file and does not
clear until the last function carrying it leaves.

### What moved `bg_sub.c`

| Move | Score |
| --- | --- |
| Recipe C: one chase step per axis, two commits | 7.38 -> **7.49** |
| Recipe E: one function per x step direction | -> **7.55**, last Bumpy Road gone |
| Recipe D: one family position set for all four setters | flat, two left the web |
| Recipe D: one horizontal zoom request, 11 copies | flat, the cross-axis group dissolved |
| Recipe D: one vertical zoom request, 15 copies | flat, a group went |
| **Recipe S: the zoom requests -> `bg_zoom.c`** | -> **8.81**, 1090 -> 794 lines |
| Recipe D: the family loops call the appoint functions | -> **9.09** |
| Recipe D: one test for a released zoom request | flat, the pair shrank by a third |

### Plateaus, and why

- **`bg.c`, 9.09.** One finding: four chip-remap scanners that no recipe merges.
  `remap_stage19_default_chip` and `remap_stage03_background_chip` differ in their limit
  *and* in a `*vtxColor = *rw3col_ptr` write that happens only on a match, so it cannot
  be hoisted to the call site and a helper reporting both a remapped index and whether it
  matched is two results. `remap_stage03_player_chip` differs from the others in its key
  *and* its value expression, both indexed by the loop variable, so neither travels to a
  call site. `advance_stage19_state` against `advance_stage03_state` differ in shape, not
  in a value. A second Recipe S split was considered and is blocked: every seam between
  the ending and stage halves runs through `draw_remapped_tiles`, `bgDrawOneScreen` or
  `bgRWWorkUpdate`, all `static` with callers on both sides.
- **`bg_sub.c`, 9.09.** Three mirrored x/y pairs. `scr_11_22`/`scr_12_21` swap the player
  indices in three places; `scr_11_21`/`scr_12_22` differ in `<` against `>` and `-`
  against `+`; the two chase start checks differ in five names and two callees.
- **`bg_zoom.c`, 8.54.** The zoom selector chain. See the note below.
- **`bg000.c`, 9.92.** `advance_bg0000_demo_position`'s two arms differ in `+=` against
  `-=` and `>` against `<`, which may never be parameterised.

### Reverted, and why

- **Recipe D on the rewrite-list selection in `bg.c`, six call sites.** 8.81 -> 8.54.
  Legal and the block was genuinely identical, but with the tail gone all eight scanners
  became the same ten-line loop and two functions that had left Code Duplication rejoined
  it. The clearest case in this wave of decomposition manufacturing resemblance.
- **Recipe X on five more `bg_zoom.c` selectors** (the full set, on top of the one that
  was kept). 8.54 -> 8.28. Every dispatcher in that chain has the same shape, so any two
  left bare twin each other. One split pays; six do not. This is
  `plpatuni.c`'s lesson measured again.
- **Recipe E on `bg000.c`'s two demo arms.** 9.92 -> 9.38 - the arms twin.
- **Recipe D on `bg000.c`'s settle block.** Flat, and `advance_bg0000_demo_position` kept
  bumps = 2. Ten duplicated lines went, but nothing measurable moved, so it went back.
- **Recipe P on `check_cg_zoom`'s merge condition, and Recipe E on the per-fighter
  position update, in `bg_zoom.c`.** Both flat, review byte-identical before and after.
- **Recipe C on the run `scr_11_22` and `scr_12_21` share in `bg_sub.c`.** The middle
  five lines - halve the satse difference, take the fighters' midpoint, subtract the ideal
  screen position - are identical character for character, and sharing them took both
  functions from 19 lines to 13. Flat, and both stayed in the group with the same numbers:
  what makes them a pair is the swapped player indices in the two `micchaku_flag` tests,
  which no recipe reaches, and the run was never the reason. Reverted under rule 2.

### Two things worth carrying forward

**Share the run before splitting the shape - and the converse.** `bg_textures.c` needed
both, in order: extracting `Bg_Kakikae_Set`'s two arms *first* measured 9.38 -> 8.81,
because each arm carried its own copy of the six-line slot load and the two helpers read
as duplicates. Sharing that load first, then extracting, measured 9.38 -> **10.00**. The
two commits are flat on their own and worth 0.62 as a set.

**A two-instance family is not automatically blocked.** `bonus_bg.c`'s two bonus inits
differ in two literals, which Recipe D refuses and Recipe V refuses for having only two
instances - the open question the playbook records. It did not need the rule relaxed:
the two closing lines are a contiguous identical run, and Recipe C on them took the file
from 9.38 to 10.00.

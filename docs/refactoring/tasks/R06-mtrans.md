# R06 - refactor `src/sf33rd/Source/Game/rendering/mtrans.c`

| Field | Value |
| --- | --- |
| Baseline Code Health | **2.57** / 10 (Red) |
| Exit target | **>= 4.00** (leaves the Red band) |
| Stretch target | 5.15 (CodeScene industry-average scenario) |
| File size | 2224 lines |
| Git churn | 10 commits |
| Risk tier | **MEDIUM** - Rendering transforms; no gameplay state. |
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
code_health_review(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/rendering/mtrans.c")
code_health_score(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/rendering/mtrans.c")
```

The score must read 2.57. If it does not, this file changed after the task
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
| Code Duplication | 2 |
| Excess Number of Function Arguments | 2 |

This file also trips **Lines of Code in a Single File**. Splitting it is a *later* step.
Do not start by moving code between files - reduce function-level complexity first.

**Code Duplication** is flagged. When two blocks are identical, extract one shared helper
rather than editing both copies.

## 4. Target functions, highest leverage first

| # | Function | Line | Cyclomatic | Nesting | Bumps |
| --- | --- | --- | --- | --- | --- |
| 1 | `mlt_obj_trans_rgb_ext` | 1130 | 22 | 4 | 4 |
| 2 | `mlt_obj_trans_cp3_ext` | 748 | 22 | 4 | 4 |
| 3 | `mlt_obj_trans_ext` | 382 | 22 | 4 | 4 |
| 4 | `makeup_tpu_free` | 1873 | 11 | 5 | 2 |
| 5 | `mlt_obj_melt2` | 2143 | 9 | 4 | 2 |
| 6 | `mlt_obj_trans_rgb` | 1366 | 15 | - | 3 |

Thresholds for C: cyclomatic complexity under 9, nesting depth under 4.

## 5. Steps

Work **one function at a time, in the order above**. After each function run the
verification in section 6. Do not batch several functions into one change.

### Step 1: `mlt_obj_trans_rgb_ext` (line 1130)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe E (extract function)** - 4 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(mtrans): simplify mlt_obj_trans_rgb_ext`

### Step 2: `mlt_obj_trans_cp3_ext` (line 748)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe E (extract function)** - 4 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(mtrans): simplify mlt_obj_trans_cp3_ext`

### Step 3: `mlt_obj_trans_ext` (line 382)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe E (extract function)** - 4 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(mtrans): simplify mlt_obj_trans_ext`

### Step 4: `makeup_tpu_free` (line 1873)

- **Recipe G (guard clauses)** - nesting is 5, target is under 4.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(mtrans): simplify makeup_tpu_free`

### Step 5: `mlt_obj_melt2` (line 2143)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(mtrans): simplify mlt_obj_melt2`

### Final step: re-measure, then stop

Once the score reaches 4.00, **stop**. Do not keep pushing for a higher number.

## 6. Verification - run all three after EVERY step

**Step 1 - it still builds.**

```bash
export PATH="/c/msys64/mingw64/bin:$PATH"
cmake --build build
```

**Step 2 - no constant was removed or altered.**

```bash
python tools/refactor_guard.py src/sf33rd/Source/Game/rendering/mtrans.c
```

A `FAIL` here means a literal disappeared or changed value - that is not a legal
refactor. Revert immediately. A `WARN` about *added* literals is normally fine
(a new guard clause brings its own `return 0`).

**Step 3 - the metric improved.**

```
code_health_score(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/rendering/mtrans.c")
```

- Score went **up**, build passed, guard passed: commit and continue.
- Score went **down or stayed flat**: revert that step with
  `git checkout -- src/sf33rd/Source/Game/rendering/mtrans.c` and move to the next function. Do not force it.
- Guard **failed**: revert that step, no exceptions.

## 7. Definition of done

- [ ] `code_health_score` >= 4.00
- [ ] `cmake --build build` succeeds with no new warnings
- [ ] `python tools/refactor_guard.py src/sf33rd/Source/Game/rendering/mtrans.c` reports no removed literals
- [ ] Each commit covers one function and uses the message format above
- [ ] No behaviour change: no edits to literals, arithmetic, comparison operators, or the order of side effects
- [ ] Final `code_health_review` captured in the report

## 8. Why this file is worth the effort

CodeScene projects that lifting this file to 5.15 yields, as a 90% confidence interval:

- **30% to 56%** fewer defects
- **4% to 24%** less development time

Model-based projections, not guarantees.

## 9. Report back

Reply with exactly this, filled in:

```
Task: R06 (src/sf33rd/Source/Game/rendering/mtrans.c)
Baseline score: 2.57
Final score:    <x.xx>
Steps completed: <n> of 5
Steps reverted:  <n>   (list which, and why)
Build: PASS / FAIL
Commits: <sha list>
Unsure about: <free text, or NONE>
```

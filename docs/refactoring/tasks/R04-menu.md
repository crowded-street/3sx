# R04 - refactor `src/sf33rd/Source/Game/menu/menu.c`

| Field | Value |
| --- | --- |
| Baseline Code Health | **1.97** / 10 (Red) |
| Exit target | **>= 4.00** (leaves the Red band) |
| Stretch target | 5.15 (CodeScene industry-average scenario) |
| File size | 5365 lines |
| Git churn | 58 commits |
| Risk tier | **LOW** - Menu/UI flow; not part of in-round simulation. |
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
code_health_review(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/menu/menu.c")
code_health_score(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/menu/menu.c")
```

The score must read 1.97. If it does not, this file changed after the task
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
| Large Method | 2 |

This file also trips **Lines of Code in a Single File**. Splitting it is a *later* step.
Do not start by moving code between files - reduce function-level complexity first.

**Code Duplication** is flagged. When two blocks are identical, extract one shared helper
rather than editing both copies.

## 4. Target functions, highest leverage first

| # | Function | Line | Cyclomatic | Nesting | Bumps |
| --- | --- | --- | --- | --- | --- |
| 1 | `Direction_Menu` | 897 | 27 | 5 | 4 |
| 2 | `Extra_Option` | 5039 | 28 | 4 | 4 |
| 3 | `Menu_Select` | 2635 | 19 | 5 | 2 |
| 4 | `Normal_Training` | 4005 | 24 | 4 | 2 |
| 5 | `Blocking_Training` | 4508 | 20 | 4 | 2 |
| 6 | `toSelectGame` | 469 | 16 | 4 | 3 |

Thresholds for C: cyclomatic complexity under 9, nesting depth under 4.

## 5. Steps

Work **one function at a time, in the order above**. After each function run the
verification in section 6. Do not batch several functions into one change.

### Step 1: `Direction_Menu` (line 897)

- **Recipe G (guard clauses)** - nesting is 5, target is under 4.
- **Recipe E (extract function)** - 4 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(menu): simplify Direction_Menu`

### Step 2: `Extra_Option` (line 5039)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe E (extract function)** - 4 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(menu): simplify Extra_Option`

### Step 3: `Menu_Select` (line 2635)

- **Recipe G (guard clauses)** - nesting is 5, target is under 4.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(menu): simplify Menu_Select`

### Step 4: `Normal_Training` (line 4005)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(menu): simplify Normal_Training`

### Step 5: `Blocking_Training` (line 4508)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(menu): simplify Blocking_Training`

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
python tools/refactor_guard.py src/sf33rd/Source/Game/menu/menu.c
```

A `FAIL` here means a literal disappeared or changed value - that is not a legal
refactor. Revert immediately. A `WARN` about *added* literals is normally fine
(a new guard clause brings its own `return 0`).

**Step 3 - the metric improved.**

```
code_health_score(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/menu/menu.c")
```

- Score went **up**, build passed, guard passed: commit and continue.
- Score went **down or stayed flat**: revert that step with
  `git checkout -- src/sf33rd/Source/Game/menu/menu.c` and move to the next function. Do not force it.
- Guard **failed**: revert that step, no exceptions.

## 7. Definition of done

- [ ] `code_health_score` >= 4.00
- [ ] `cmake --build build` succeeds with no new warnings
- [ ] `python tools/refactor_guard.py src/sf33rd/Source/Game/menu/menu.c` reports no removed literals
- [ ] Each commit covers one function and uses the message format above
- [ ] No behaviour change: no edits to literals, arithmetic, comparison operators, or the order of side effects
- [ ] Final `code_health_review` captured in the report

## 8. Why this file is worth the effort

CodeScene projects that lifting this file to 5.15 yields, as a 90% confidence interval:

- **40% to 66%** fewer defects
- **6% to 29%** less development time

Model-based projections, not guarantees.

## 9. Report back

Reply with exactly this, filled in:

```
Task: R04 (src/sf33rd/Source/Game/menu/menu.c)
Baseline score: 1.97
Final score:    <x.xx>
Steps completed: <n> of 5
Steps reverted:  <n>   (list which, and why)
Build: PASS / FAIL
Commits: <sha list>
Unsure about: <free text, or NONE>
```

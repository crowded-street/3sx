# R14 - refactor `src/sf33rd/Source/Game/stage/bg.c`

| Field | Value |
| --- | --- |
| Baseline Code Health | **3.62** / 10 (Red) |
| Exit target | **>= 4.00** (leaves the Red band) |
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

### Step 1: `scr_trans` (line 542)

- **Recipe G (guard clauses)** - nesting is 9, target is under 4.
- **Recipe E (extract function)** - 16 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(bg): simplify scr_trans`

### Step 2: `Bg_Texture_Load_Ending` (line 422)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe E (extract function)** - 3 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(bg): simplify Bg_Texture_Load_Ending`

### Step 3: `bgDrawOneScreen` (line 1059)

- **Recipe G (guard clauses)** - nesting is 6, target is under 4.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(bg): simplify bgDrawOneScreen`

### Step 4: `Bg_Texture_Load_EX` (line 235)

- **Recipe E (extract function)** - 6 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(bg): simplify Bg_Texture_Load_EX`

### Step 5: `Bg_Kakikae_Set` (line 72)

- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(bg): simplify Bg_Kakikae_Set`

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
Steps completed: <n> of 5
Steps reverted:  <n>   (list which, and why)
Build: PASS / FAIL
Commits: <sha list>
Unsure about: <free text, or NONE>
```

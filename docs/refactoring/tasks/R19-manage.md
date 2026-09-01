# R19 - refactor `src/sf33rd/Source/Game/engine/manage.c`

| Field | Value |
| --- | --- |
| Baseline Code Health | **3.92** / 10 (Red) |
| Exit target | **>= 4.00** (leaves the Red band) |
| Stretch target | 5.15 (CodeScene industry-average scenario) |
| File size | 2518 lines |
| Git churn | 30 commits |
| Risk tier | **CRITICAL** - Core gameplay engine; rollback-netcode determinism depends on it. |
| Track | **B** (BLOCKED - see Gate) |
| Recipes needed | P, G, E |
| Suitable for | strongest available agent |

Recipe P is a copy-paste transformation and is safe for a small model. Recipe E needs
data-flow reasoning about which variables cross the extracted boundary - that is where
a weaker model fails silently while the build still succeeds. If this task is assigned
to a smaller model, restrict it to the Recipe P steps and leave the rest.

> [!WARNING]
> **Gate: do not start this task until statcheck is green in CI.**
> This file is CRITICAL risk. A silent behaviour change here corrupts gameplay or
> breaks rollback determinism, and nothing in the build will catch it. If statcheck is
> not yet running, pick a Track A task instead and leave this one alone.

## 1. Read these first

- [`../PLAYBOOK.md`](../PLAYBOOK.md) - the only transformations you are allowed to apply.
- [`../README.md`](../README.md) - campaign rules and the definition of done.

## 2. Record the baseline before touching anything

Call both CodeScene MCP tools and keep the output for your report:

```
code_health_review(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/engine/manage.c")
code_health_score(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/engine/manage.c")
```

The score must read 3.92. If it does not, this file changed after the task
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

This file also trips **Lines of Code in a Single File**. Splitting it is a *later* step.
Do not start by moving code between files - reduce function-level complexity first.

## 4. Target functions, highest leverage first

| # | Function | Line | Cyclomatic | Nesting | Bumps |
| --- | --- | --- | --- | --- | --- |
| 1 | `Game_Manage_12_4` | 2054 | 32 | 4 | 5 |
| 2 | `Wait_Seek_Time` | 285 | 12 | 4 | 2 |
| 3 | `Game_Manage_12_8` | 2231 | 25 | - | 3 |
| 4 | `Game_Manage_2_4` | 436 | 16 | - | - |
| 5 | `Game_Manage_9th` | 1057 | 12 | - | - |
| 6 | `Check_Break_Into_CPU` | 1755 | 10 | - | - |

Thresholds for C: cyclomatic complexity under 9, nesting depth under 4.

## 5. Steps

Work **one function at a time, in the order above**. After each function run the
verification in section 6. Do not batch several functions into one change.

### Step 1: `Game_Manage_12_4` (line 2054)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe E (extract function)** - 5 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(manage): simplify Game_Manage_12_4`

### Step 2: `Wait_Seek_Time` (line 285)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(manage): simplify Wait_Seek_Time`

### Step 3: `Game_Manage_12_8` (line 2231)

- **Recipe E (extract function)** - 3 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(manage): simplify Game_Manage_12_8`

### Step 4: `Game_Manage_2_4` (line 436)

- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(manage): simplify Game_Manage_2_4`

### Step 5: `Game_Manage_9th` (line 1057)

- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(manage): simplify Game_Manage_9th`

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
python tools/refactor_guard.py src/sf33rd/Source/Game/engine/manage.c
```

A `FAIL` here means a literal disappeared or changed value - that is not a legal
refactor. Revert immediately. A `WARN` about *added* literals is normally fine
(a new guard clause brings its own `return 0`).

**Step 3 - the metric improved.**

```
code_health_score(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/engine/manage.c")
```

- Score went **up**, build passed, guard passed: commit and continue.
- Score went **down or stayed flat**: revert that step with
  `git checkout -- src/sf33rd/Source/Game/engine/manage.c` and move to the next function. Do not force it.
- Guard **failed**: revert that step, no exceptions.

## 7. Definition of done

- [ ] `code_health_score` >= 4.00
- [ ] `cmake --build build` succeeds with no new warnings
- [ ] `python tools/refactor_guard.py src/sf33rd/Source/Game/engine/manage.c` reports no removed literals
- [ ] Each commit covers one function and uses the message format above
- [ ] No behaviour change: no edits to literals, arithmetic, comparison operators, or the order of side effects
- [ ] Final `code_health_review` captured in the report
- [ ] statcheck runs clean across the replay corpus

## 8. Why this file is worth the effort

CodeScene projects that lifting this file to 5.15 yields, as a 90% confidence interval:

- **10% to 27%** fewer defects
- **2% to 12%** less development time

Model-based projections, not guarantees.

## 9. Report back

Reply with exactly this, filled in:

```
Task: R19 (src/sf33rd/Source/Game/engine/manage.c)
Baseline score: 3.92
Final score:    <x.xx>
Steps completed: <n> of 5
Steps reverted:  <n>   (list which, and why)
Build: PASS / FAIL
Commits: <sha list>
Unsure about: <free text, or NONE>
```

# R16 - refactor `src/sf33rd/Source/Game/com/ck_pass.c`

| Field | Value |
| --- | --- |
| Baseline Code Health | **3.75** / 10 (Red) |
| Exit target | **>= 4.00** (leaves the Red band) |
| Stretch target | 5.15 (CodeScene industry-average scenario) |
| File size | 3126 lines |
| Git churn | 5 commits |
| Risk tier | **CRITICAL** - CPU-opponent logic driving gameplay state. |
| Track | **B** (BLOCKED - see Gate) |
| Recipes needed | P, E |
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
code_health_review(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/com/ck_pass.c")
code_health_score(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/com/ck_pass.c")
```

The score must read 3.75. If it does not, this file changed after the task
was written - stop and report that instead of proceeding.

## 3. What CodeScene flags here

| Smell | Severity (1-3) |
| --- | --- |
| Bumpy Road Ahead | 3 |
| Number of Functions in a Single Module | 2 |
| Complex Method | 2 |
| Complex Conditional | 2 |
| Code Duplication | 2 |
| Excess Number of Function Arguments | 2 |

**Code Duplication** is flagged. When two blocks are identical, extract one shared helper
rather than editing both copies.

## 4. Target functions, highest leverage first

| # | Function | Line | Cyclomatic | Nesting | Bumps |
| --- | --- | --- | --- | --- | --- |
| 1 | `KEN_vs` | 31 | 41 | - | 8 |
| 2 | `GILL_vs` | 319 | 37 | - | 7 |
| 3 | `HUGO_vs` | 190 | 33 | - | 7 |
| 4 | `Check_Special_Technique` | 464 | 14 | - | - |
| 5 | `Check_After_Attack` | 843 | 12 | - | - |
| 6 | `Check_Limited_Attack` | 618 | 12 | - | - |

Thresholds for C: cyclomatic complexity under 9, nesting depth under 4.

## 5. Steps

Work **one function at a time, in the order above**. After each function run the
verification in section 6. Do not batch several functions into one change.

### Step 1: `KEN_vs` (line 31)

- **Recipe E (extract function)** - 8 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(ck_pass): simplify KEN_vs`

### Step 2: `GILL_vs` (line 319)

- **Recipe E (extract function)** - 7 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(ck_pass): simplify GILL_vs`

### Step 3: `HUGO_vs` (line 190)

- **Recipe E (extract function)** - 7 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(ck_pass): simplify HUGO_vs`

### Step 4: `Check_Special_Technique` (line 464)

- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(ck_pass): simplify Check_Special_Technique`

### Step 5: `Check_After_Attack` (line 843)

- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(ck_pass): simplify Check_After_Attack`

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
python tools/refactor_guard.py src/sf33rd/Source/Game/com/ck_pass.c
```

A `FAIL` here means a literal disappeared or changed value - that is not a legal
refactor. Revert immediately. A `WARN` about *added* literals is normally fine
(a new guard clause brings its own `return 0`).

**Step 3 - the metric improved.**

```
code_health_score(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/com/ck_pass.c")
```

- Score went **up**, build passed, guard passed: commit and continue.
- Score went **down or stayed flat**: revert that step with
  `git checkout -- src/sf33rd/Source/Game/com/ck_pass.c` and move to the next function. Do not force it.
- Guard **failed**: revert that step, no exceptions.

## 7. Definition of done

- [ ] `code_health_score` >= 4.00
- [ ] `cmake --build build` succeeds with no new warnings
- [ ] `python tools/refactor_guard.py src/sf33rd/Source/Game/com/ck_pass.c` reports no removed literals
- [ ] Each commit covers one function and uses the message format above
- [ ] No behaviour change: no edits to literals, arithmetic, comparison operators, or the order of side effects
- [ ] Final `code_health_review` captured in the report
- [ ] statcheck runs clean across the replay corpus

## 8. Why this file is worth the effort

CodeScene projects that lifting this file to 5.15 yields, as a 90% confidence interval:

- **12% to 31%** fewer defects
- **2% to 13%** less development time

Model-based projections, not guarantees.

## 9. Report back

Reply with exactly this, filled in:

```
Task: R16 (src/sf33rd/Source/Game/com/ck_pass.c)
Baseline score: 3.75
Final score:    <x.xx>
Steps completed: <n> of 5
Steps reverted:  <n>   (list which, and why)
Build: PASS / FAIL
Commits: <sha list>
Unsure about: <free text, or NONE>
```

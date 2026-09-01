# R04 - refactor `src/sf33rd/Source/Game/menu/menu.c`

| Field | Value |
| --- | --- |
| Baseline Code Health | **2.25** / 10 (Red) |
| Exit target | **>= 4.00** (leaves the Red band) |
| Stretch target | 5.15 (CodeScene industry-average scenario) |
| File size | 5374 lines |
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

The score must read 2.25. If it does not, this file changed after the task
was written - stop and report that instead of proceeding.

## 3. What CodeScene flags here

| Smell | Severity (1-3) |
| --- | --- |
| Bumpy Road Ahead | 3 |
| Deep, Nested Complexity | 3 |
| Number of Functions in a Single Module | 2 |
| Overall Code Complexity | 2 |
| Complex Method | 2 |
| Complex Conditional | 2 |
| Code Duplication | 2 |
| Large Method | 2 |

**Code Duplication** is flagged. When two blocks are identical, extract one shared helper
rather than editing both copies.

## 4. Target functions, highest leverage first

| # | Function | Line | Cyclomatic | Nesting | Bumps |
| --- | --- | --- | --- | --- | --- |
| 1 | `Extra_Option` | 5048 | 28 | 4 | 4 |
| 2 | `Direction_Menu` | 919 | 25 | 5 | 3 |
| 3 | `Normal_Training` | 4012 | 24 | 4 | 2 |
| 4 | `Blocking_Training` | 4517 | 20 | 4 | 2 |
| 5 | `toSelectGame` | 469 | 16 | 4 | 3 |
| 6 | `End_Replay_Menu` | 5292 | 13 | 4 | 2 |
| 7 | `Mode_Select` | 285 | 29 | - | 4 |
| 8 | `VS_Result` | 3060 | 26 | - | 5 |
| 9 | `Character_Change` | 4764 | 9 | 4 | 2 |
| 10 | `Ex_Move_Sub_LR` | 5205 | 23 | - | 3 |
| 11 | `After_Replay` | 4854 | 25 | - | 2 |
| 12 | `Dir_Move_Sub_LR` | 1133 | 15 | - | 5 |

Thresholds for C: cyclomatic complexity under 9, nesting depth under 4.

## 5. Steps

Work **one function at a time, in the order above**. After each function run the
verification in section 6. Do not batch several functions into one change.

The 12 targets are grouped into **3 waves**. A wave is one sitting
and one pull request: do its steps, re-measure, report, open the PR, stop. Pick the
next wave up afterwards. Do not attempt every wave in one go - this file is
5374 lines and the review gets stale underneath you as you change it.

Re-run `code_health_review` at the start of each wave: line numbers below shift as
earlier waves land.

### Wave 1 - start here (one PR)

#### Step 1: `Extra_Option` (line 5048)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe E (extract function)** - 4 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(menu): simplify Extra_Option`

#### Step 2: `Direction_Menu` (line 919)

- **Recipe G (guard clauses)** - nesting is 5, target is under 4.
- **Recipe E (extract function)** - 3 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(menu): simplify Direction_Menu`

#### Step 3: `Normal_Training` (line 4012)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(menu): simplify Normal_Training`

#### Step 4: `Blocking_Training` (line 4517)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(menu): simplify Blocking_Training`

### Wave 2 (one PR)

#### Step 5: `toSelectGame` (line 469)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe E (extract function)** - 3 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(menu): simplify toSelectGame`

#### Step 6: `End_Replay_Menu` (line 5292)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(menu): simplify End_Replay_Menu`

#### Step 7: `Mode_Select` (line 285)

- **Recipe E (extract function)** - 4 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(menu): simplify Mode_Select`

#### Step 8: `VS_Result` (line 3060)

- **Recipe E (extract function)** - 5 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(menu): simplify VS_Result`

### Wave 3 (one PR)

#### Step 9: `Character_Change` (line 4764)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(menu): simplify Character_Change`

#### Step 10: `Ex_Move_Sub_LR` (line 5205)

- **Recipe E (extract function)** - 3 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(menu): simplify Ex_Move_Sub_LR`

#### Step 11: `After_Replay` (line 4854)

- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(menu): simplify After_Replay`

#### Step 12: `Dir_Move_Sub_LR` (line 1133)

- **Recipe E (extract function)** - 5 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(menu): simplify Dir_Move_Sub_LR`

### When to stop

Stop at the end of your wave, even if you feel you could keep going. Also stop
early if the score reaches 4.00.

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
- Guard **failed**: revert that step, no exceptions.
- Score **flat or down**: do *not* revert yet. Run the review and check whether the
  function you targeted left a category or dropped in complexity:

```
code_health_review(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/menu/menu.c")
```

  This file has 5374 lines, so one function can move the aggregate score by
  less than its resolution. If the targeted function left Deep Nested Complexity,
  Bumpy Road, or Large Method, or its cyclomatic complexity fell: **keep and commit.**
  If it is still listed with the same numbers: revert with
  `git checkout -- src/sf33rd/Source/Game/menu/menu.c` and move on.

## 7. Definition of done

Either of these is a successful outcome:

- [ ] `code_health_score` >= 4.00, **or**
- [ ] every function in section 4 has left at least one smell category, with the
      before/after review pasted into the report
- [ ] `cmake --build build` succeeds with no new warnings
- [ ] `python tools/refactor_guard.py src/sf33rd/Source/Game/menu/menu.c` reports no removed literals
- [ ] Each commit covers one function and uses the message format above
- [ ] No behaviour change: no edits to literals, arithmetic, comparison operators, or the order of side effects
- [ ] Final `code_health_review` captured in the report

## 8. Why this file is worth the effort

CodeScene projects that lifting this file to 5.15 yields, as a 90% confidence interval:

- **35% to 62%** fewer defects
- **5% to 27%** less development time

Model-based projections, not guarantees.

## 9. Report back

Reply with exactly this, filled in:

```
Task: R04 (src/sf33rd/Source/Game/menu/menu.c)  wave <n> of 3
Baseline score: 2.25
Final score:    <x.xx>
Steps completed: <n> of 4 in this wave
Smells cleared:  <function: which category it left, or 'none'>
Steps reverted:  <n>   (list which, and why)
Build: PASS / FAIL
Commits: <sha list>
Unsure about: <free text, or NONE>
```

# R05 - refactor `src/sf33rd/Source/Game/engine/cmd_main.c`

| Field | Value |
| --- | --- |
| Baseline Code Health | **2.31** / 10 (Red) |
| Exit target | **>= 4.00** (leaves the Red band) |
| Stretch target | 5.15 (CodeScene industry-average scenario) |
| File size | 1876 lines |
| Git churn | 12 commits |
| Risk tier | **CRITICAL** - Core gameplay engine; rollback-netcode determinism depends on it. |
| Track | **B** (manual playtesting required - see note) |
| Recipes needed | P, G, E |
| Suitable for | strongest available agent |

Recipe P is a copy-paste transformation and is safe for a small model. Recipe E needs
data-flow reasoning about which variables cross the extracted boundary - that is where
a weaker model fails silently while the build still succeeds. If this task is assigned
to a smaller model, restrict it to the Recipe P steps and leave the rest.

> [!NOTE]
> **Gate relaxed 2026-09-04 (project owner directive).** Statcheck cannot be stood up -
> a legitimate CPS3 ROM dump requires owning genuine CPS3 arcade hardware, which this
> project's no-piracy policy won't work around (see `docs/statcheck.md`, `AGENTS.md`).
> This file is CRITICAL risk - a silent behaviour change corrupts gameplay or breaks rollback
> determinism and the build will not catch it. Verify with manual playtesting instead of
> statcheck: follow the recipes exactly, one recipe per commit, build + `refactor_guard.py`
> after every commit, and call out what changed so playtesting can be targeted.

## 1. Read these first

- [`../PLAYBOOK.md`](../PLAYBOOK.md) - the only transformations you are allowed to apply.
- [`../README.md`](../README.md) - campaign rules and the definition of done.

## 2. Record the baseline before touching anything

Call both CodeScene MCP tools and keep the output for your report:

```
code_health_review(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/engine/cmd_main.c")
code_health_score(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/engine/cmd_main.c")
```

The score must read 2.31. If it does not, this file changed after the task
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

This file also trips **Lines of Code in a Single File**. Splitting it is a *later* step.
Do not start by moving code between files - reduce function-level complexity first.

**Code Duplication** is flagged. When two blocks are identical, extract one shared helper
rather than editing both copies.

## 4. Target functions, highest leverage first

| # | Function | Line | Cyclomatic | Nesting | Bumps |
| --- | --- | --- | --- | --- | --- |
| 1 | `check_10` | 622 | 48 | 5 | 3 |
| 2 | `check_12` | 840 | 44 | 5 | 3 |
| 3 | `check_21` | 1266 | 16 | 5 | 4 |
| 4 | `check_1` | 209 | 15 | 4 | 6 |
| 5 | `check_15` | 1102 | 15 | 5 | 3 |
| 6 | `check_9` | 554 | 16 | 4 | 4 |
| 7 | `check_14` | 1045 | 13 | 4 | 4 |
| 8 | `check_25` | 1470 | 11 | 4 | 3 |
| 9 | `check_24` | 1425 | 11 | 4 | 3 |
| 10 | `pl_lvr_set` | 1564 | 29 | - | 4 |
| 11 | `check_19` | 1230 | 10 | 4 | 2 |
| 12 | `check_18` | 1196 | 10 | 4 | 2 |

Thresholds for C: cyclomatic complexity under 9, nesting depth under 4.

## 5. Steps

Work **one function at a time, in the order above**. After each function run the
verification in section 6. Do not batch several functions into one change.

The 12 targets are grouped into **3 waves**. A wave is one sitting
and one pull request: do its steps, re-measure, report, open the PR, stop. Pick the
next wave up afterwards. Do not attempt every wave in one go - this file is
1876 lines and the review gets stale underneath you as you change it.

Re-run `code_health_review` at the start of each wave: line numbers below shift as
earlier waves land.

### Wave 1 - start here (one PR)

#### Step 1: `check_10` (line 622)

- **Recipe G (guard clauses)** - nesting is 5, target is under 4.
- **Recipe E (extract function)** - 3 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(cmd_main): simplify check_10`

#### Step 2: `check_12` (line 840)

- **Recipe G (guard clauses)** - nesting is 5, target is under 4.
- **Recipe E (extract function)** - 3 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(cmd_main): simplify check_12`

#### Step 3: `check_21` (line 1266)

- **Recipe G (guard clauses)** - nesting is 5, target is under 4.
- **Recipe E (extract function)** - 4 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(cmd_main): simplify check_21`

#### Step 4: `check_1` (line 209)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe E (extract function)** - 6 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(cmd_main): simplify check_1`

### Wave 2 (one PR)

#### Step 5: `check_15` (line 1102)

- **Recipe G (guard clauses)** - nesting is 5, target is under 4.
- **Recipe E (extract function)** - 3 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(cmd_main): simplify check_15`

#### Step 6: `check_9` (line 554)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe E (extract function)** - 4 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(cmd_main): simplify check_9`

#### Step 7: `check_14` (line 1045)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe E (extract function)** - 4 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(cmd_main): simplify check_14`

#### Step 8: `check_25` (line 1470)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe E (extract function)** - 3 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(cmd_main): simplify check_25`

### Wave 3 (one PR)

#### Step 9: `check_24` (line 1425)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe E (extract function)** - 3 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(cmd_main): simplify check_24`

#### Step 10: `pl_lvr_set` (line 1564)

- **Recipe E (extract function)** - 4 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(cmd_main): simplify pl_lvr_set`

#### Step 11: `check_19` (line 1230)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(cmd_main): simplify check_19`

#### Step 12: `check_18` (line 1196)

- **Recipe G (guard clauses)** - nesting is 4, target is under 4.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(cmd_main): simplify check_18`

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
python tools/refactor_guard.py src/sf33rd/Source/Game/engine/cmd_main.c
```

A `FAIL` here means a literal disappeared or changed value - that is not a legal
refactor. Revert immediately. A `WARN` about *added* literals is normally fine
(a new guard clause brings its own `return 0`).

**Step 3 - the metric improved.**

```
code_health_score(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/engine/cmd_main.c")
```

- Score went **up**, build passed, guard passed: commit and continue.
- Guard **failed**: revert that step, no exceptions.
- Score **flat or down**: do *not* revert yet. Run the review and check whether the
  function you targeted left a category or dropped in complexity:

```
code_health_review(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/engine/cmd_main.c")
```

  This file has 1876 lines, so one function can move the aggregate score by
  less than its resolution. If the targeted function left Deep Nested Complexity,
  Bumpy Road, or Large Method, or its cyclomatic complexity fell: **keep and commit.**
  If it is still listed with the same numbers: revert with
  `git checkout -- src/sf33rd/Source/Game/engine/cmd_main.c` and move on.

## 7. Definition of done

Either of these is a successful outcome:

- [ ] `code_health_score` >= 4.00, **or**
- [ ] every function in section 4 has left at least one smell category, with the
      before/after review pasted into the report
- [ ] `cmake --build build` succeeds with no new warnings
- [ ] `python tools/refactor_guard.py src/sf33rd/Source/Game/engine/cmd_main.c` reports no removed literals
- [ ] Each commit covers one function and uses the message format above
- [ ] No behaviour change: no edits to literals, arithmetic, comparison operators, or the order of side effects
- [ ] Final `code_health_review` captured in the report
- [ ] statcheck runs clean across the replay corpus

## 8. Why this file is worth the effort

CodeScene projects that lifting this file to 5.15 yields, as a 90% confidence interval:

- **35% to 61%** fewer defects
- **5% to 26%** less development time

Model-based projections, not guarantees.

## 9. Report back

Reply with exactly this, filled in:

```
Task: R05 (src/sf33rd/Source/Game/engine/cmd_main.c)  wave <n> of 3
Baseline score: 2.31
Final score:    <x.xx>
Steps completed: <n> of 4 in this wave
Smells cleared:  <function: which category it left, or 'none'>
Steps reverted:  <n>   (list which, and why)
Build: PASS / FAIL
Commits: <sha list>
Unsure about: <free text, or NONE>
```

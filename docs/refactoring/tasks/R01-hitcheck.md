# R01 - refactor `src/sf33rd/Source/Game/engine/hitcheck.c`

| Field | Value |
| --- | --- |
| Baseline Code Health | **1.42** / 10 (Red) |
| Exit target | **>= 4.00** (leaves the Red band) |
| Stretch target | 5.15 (CodeScene industry-average scenario) |
| File size | 2308 lines |
| Git churn | 17 commits |
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
code_health_review(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/engine/hitcheck.c")
code_health_score(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/engine/hitcheck.c")
```

The score must read 1.42. If it does not, this file changed after the task
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
| 1 | `attack_hit_check` | 1815 | 67 | 8 | 8 |
| 2 | `defense_ground_ps2` | 1353 | 60 | 6 | 8 |
| 3 | `defense_ground_cps3` | 1201 | 41 | 5 | 7 |
| 4 | `catch_hit_check` | 1729 | 34 | 7 | 4 |
| 5 | `hit_pattern_extdat_check` | 727 | 39 | 5 | 6 |
| 6 | `plef_at_vs_player_damage_union` | 455 | 39 | 5 | 6 |

Thresholds for C: cyclomatic complexity under 9, nesting depth under 4.

## 5. Steps

Work **one function at a time, in the order above**. After each function run the
verification in section 6. Do not batch several functions into one change.

### Step 1: `attack_hit_check` (line 1815)

- **Recipe G (guard clauses)** - nesting is 8, target is under 4.
- **Recipe E (extract function)** - 8 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(hitcheck): simplify attack_hit_check`

### Step 2: `defense_ground_ps2` (line 1353)

- **Recipe G (guard clauses)** - nesting is 6, target is under 4.
- **Recipe E (extract function)** - 8 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(hitcheck): simplify defense_ground_ps2`

### Step 3: `defense_ground_cps3` (line 1201)

- **Recipe G (guard clauses)** - nesting is 5, target is under 4.
- **Recipe E (extract function)** - 7 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(hitcheck): simplify defense_ground_cps3`

### Step 4: `catch_hit_check` (line 1729)

- **Recipe G (guard clauses)** - nesting is 7, target is under 4.
- **Recipe E (extract function)** - 4 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(hitcheck): simplify catch_hit_check`

### Step 5: `hit_pattern_extdat_check` (line 727)

- **Recipe G (guard clauses)** - nesting is 5, target is under 4.
- **Recipe E (extract function)** - 6 nested blocks; each bump is a missing function.
- **Recipe P (named predicate)** - move compound conditions into named boolean helpers.

Commit message: `refactor(hitcheck): simplify hit_pattern_extdat_check`

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
python tools/refactor_guard.py src/sf33rd/Source/Game/engine/hitcheck.c
```

A `FAIL` here means a literal disappeared or changed value - that is not a legal
refactor. Revert immediately. A `WARN` about *added* literals is normally fine
(a new guard clause brings its own `return 0`).

**Step 3 - the metric improved.**

```
code_health_score(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/engine/hitcheck.c")
```

- Score went **up**, build passed, guard passed: commit and continue.
- Guard **failed**: revert that step, no exceptions.
- Score **flat or down**: do *not* revert yet. Run the review and check whether the
  function you targeted left a category or dropped in complexity:

```
code_health_review(file_path="E:/SynologyDrive/research/_agentic_refactoring/Street Fighter/git_win10/3sx/src/sf33rd/Source/Game/engine/hitcheck.c")
```

  This file has 2308 lines, so one function can move the aggregate score by
  less than its resolution. If the targeted function left Deep Nested Complexity,
  Bumpy Road, or Large Method, or its cyclomatic complexity fell: **keep and commit.**
  If it is still listed with the same numbers: revert with
  `git checkout -- src/sf33rd/Source/Game/engine/hitcheck.c` and move on.

## 7. Definition of done

Either of these is a successful outcome:

- [ ] `code_health_score` >= 4.00, **or**
- [ ] every function in section 4 has left at least one smell category, with the
      before/after review pasted into the report
- [ ] `cmake --build build` succeeds with no new warnings
- [ ] `python tools/refactor_guard.py src/sf33rd/Source/Game/engine/hitcheck.c` reports no removed literals
- [ ] Each commit covers one function and uses the message format above
- [ ] No behaviour change: no edits to literals, arithmetic, comparison operators, or the order of side effects
- [ ] Final `code_health_review` captured in the report
- [ ] statcheck runs clean across the replay corpus

## 8. Why this file is worth the effort

CodeScene projects that lifting this file to 5.15 yields, as a 90% confidence interval:

- **48% to 73%** fewer defects
- **7% to 35%** less development time

Model-based projections, not guarantees.

## 9. Report back

Reply with exactly this, filled in:

```
Task: R01 (src/sf33rd/Source/Game/engine/hitcheck.c)
Baseline score: 1.42
Final score:    <x.xx>
Steps completed: <n> of 5
Steps reverted:  <n>   (list which, and why)
Build: PASS / FAIL
Commits: <sha list>
Unsure about: <free text, or NONE>
```

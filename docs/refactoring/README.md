# Code Health Refactoring Campaign

A structured, measurable campaign to reduce technical debt in the 3SX codebase, designed
so that the work can be distributed across many agents - including agents that are not
capable enough to reason about the whole system.

Every task is scoped to **one file**, bounded by a **closed catalogue of transformations**,
and judged by an **external metric** (CodeScene Code Health) rather than by taste.

---

## Baseline

Measured with the CodeScene MCP server (cs-mcp 1.4.7) across every first-party `.c` and
`.cpp` file. Vendored code (`src/imgui`, `src/stb`, `src/argparse`) and generated tables
(`src/bin2obj`) are excluded.

| Band | Score | Files | Share |
| --- | --- | --- | --- |
| **Red** - severe debt | 1.0 - 3.9 | **19** | 3.9% |
| **Yellow** - problematic debt | 4.0 - 8.9 | **207** | 42.9% |
| Green | 9.0 - 9.9 | 158 | 32.8% |
| Optimal | 10.0 | 98 | 20.3% |
| **Total scored** | | **482** | |

A further **142 files** could not be scored. All of them are pure `const` data tables with
zero function definitions - 54,551 lines of static arrays. They are **out of scope** and
must not be touched.

The debt is concentrated: **19 files carry the severe debt**, and they are almost all in
`src/sf33rd/Source/Game/`. Across those 19 files CodeScene finds **330 complex methods**,
**267 bumpy roads**, and **95 deeply-nested functions**.

Every one of the 19 Red files trips both *Bumpy Road Ahead* and *Complex Method*; 18 of 19
trip *Deep, Nested Complexity*. The campaign therefore leans almost entirely on three
recipes: **extract function**, **guard clauses**, and **named predicates**.

### The extremes

| Function | File | Cyclomatic | Nesting |
| --- | --- | --- | --- |
| `scr_trans` | `stage/bg.c:542` | **109** | **9** |
| `check_special_attack` | `engine/pls03.c:676` | 99 | 8 |
| `jumping_cg_type_check` | `engine/pls00.c:788` | 92 | - |
| `attack_hit_check` | `engine/hitcheck.c:1815` | 67 | 8 |

For reference, the recommended threshold for C is **cyclomatic complexity below 9** and
**nesting depth below 4**.

---

## The central risk

3SX is a native port derived from a matching decompilation. Much of this code is a faithful
reproduction of arcade behaviour, and it feeds a **rollback netcode** implementation where
determinism is not negotiable. A refactor that changes behaviour by one frame is a
regression that no compiler warning will catch.

**`statcheck` is the only thing that can prove behaviour was preserved.** It replays real
Fightcade matches frame by frame and compares health, positions, stun, SA meter, round
timer, and round phase against CPS3 RAM dumps.

Three facts about its current state:

- `THREESX_STATCHECK` defaults to **OFF**.
- **No CI workflow runs it.** The five workflows build on Linux, macOS, Windows and PSP,
  and none of them execute a single test.
- **The blocker is not "not yet built."** `fbneo-replay-runner` needs a real CPS3 ROM dump
  to emulate anything, and a legitimate dump requires owning genuine CPS3 arcade hardware -
  this project's no-piracy policy (`README.md`) won't work around that. Treat statcheck as
  indefinitely unavailable.

**2026-09-04 (project owner directive):** given the above, the campaign no longer waits on
statcheck. The Track A / Track B split below stays as a risk label, but Track B files may
now be refactored under the same recipe discipline - verification is manual playtesting
by a human instead of a clean statcheck run.

---

## Two tracks

### Track A - start now (6 files)

Files outside the in-round simulation. A compile-verified mechanical refactor is acceptable
here before statcheck exists, because a mistake degrades menus or rendering rather than
silently corrupting a match.

| Task | File | Score | Risk |
| --- | --- | --- | --- |
| R04 | `Game/menu/menu.c` | 1.97 | LOW |
| R06 | `Game/rendering/mtrans.c` | 2.57 | MEDIUM |
| R10 | `Game/opening/opening.c` | 3.09 | LOW |
| R11 | `Common/PPGFile.c` | 3.49 | MEDIUM |
| R14 | `Game/stage/bg.c` | 3.62 | MEDIUM |
| R17 | `Game/screen/entry.c` | 3.86 | LOW |

`menu.c` is the highest-churn file in the entire Red band (58 commits) and scores 1.97 -
it is the single best place to start. `bg.c` contains `scr_trans`, the worst function in
the codebase.

### Track B - manual playtesting required (13 files)

Core engine, CPU logic, animation and effects. Statcheck cannot verify these (see above),
so **verify with manual playtesting instead of waiting on CI.** Each task file's warning
banner has been updated to reflect this.

These are the files where refactoring pays the most and costs the most if it goes wrong.

---

## Phases

**Phase 0 - Stand up the safety net** *(indefinitely blocked - see above; not a prerequisite
for Phase 2 anymore)*

1. Obtain the PS2 assets and a Fightcade replay corpus (see [`../statcheck.md`](../statcheck.md)).
2. Build `fbneo-replay-runner`, produce SCRD archives, confirm `tools/statcheck_runner.py`
   runs clean **on unmodified `main`**. A red baseline means the harness is wrong, not the code.
3. Add a CI workflow that builds with `-DTHREESX_STATCHECK=ON` and runs the corpus.
4. Record how long a full run takes - that number sets the batch size for Phase 2.

This requires a legitimately-dumped CPS3 ROM, which requires owning genuine CPS3 arcade
hardware. Revisit this phase only if that changes.

**Phase 1 - Track A** *(can start immediately)*

Work R04, R06, R10, R11, R14, R17 in that order. Each file exits when it scores >= 4.00.
This phase also serves as the pilot: it tells you whether the task format actually works
for the agents you plan to use, on files where being wrong is cheap.

**Phase 2 - Track B** *(can start now - Phase 0 is not a prerequisite; verify with manual
playtesting instead)*

The remaining 13 files, hardest first: R01 `hitcheck.c`, R02 `com_sub.c`, R03 `pls03.c`.
Every commit should still be called out clearly (file, function, what changed) so a human
playtester knows what to focus on.

**Phase 3 - The Yellow band** *(not yet specified)*

207 files. Do not generate these tasks until Phases 1 and 2 have shown the pipeline works
and the per-file cost is known.

---

## Rules for every agent

1. Apply **only** the recipes in [`PLAYBOOK.md`](PLAYBOOK.md). Nothing else.
2. **One recipe, one function, one commit.**
3. Re-measure after every commit. Score did not improve, revert the commit.
4. Never change literals, arithmetic, comparisons, types, or the order of side effects.
5. Found a bug? **Report it, do not fix it.** Arcade-accurate is not the same as correct.
6. Stop at 4.00. Do not gold-plate.
7. Blocked, confused, or the baseline score does not match? **Stop and report.**
   An unfinished task is a fine outcome.

---

## Definition of done, per file

- `code_health_score` >= 4.00
- `cmake --build build` succeeds with no new warnings
- One function per commit, message format `refactor(<file>): simplify <function>`
- Track B only: statcheck clean across the corpus, when statcheck exists. Until then
  (indefinitely - see the note above), manual playtesting stands in for it.
- Final `code_health_review` included in the report

---

## Tracking progress

Re-run the full sweep at any time to see campaign-level movement:

```bash
python tools/codehealth_sweep.py
```

It writes `codehealth-baseline.json` and prints the band distribution. Compare against the
committed baseline to see what the campaign has actually bought.

The sweep must run in **chunks**. Firing all 624 files at the MCP server at once makes the
CodeScene CLI die with `java.lang.OutOfMemoryError`; the script handles this for you.

---

## Files in this directory

| Path | Purpose |
| --- | --- |
| [`README.md`](README.md) | This charter |
| [`PLAYBOOK.md`](PLAYBOOK.md) | The closed catalogue of allowed transformations |
| [`BACKLOG.md`](BACKLOG.md) | All 19 Red tasks, ranked, with baselines |
| [`AGENT-SETUP.md`](AGENT-SETUP.md) | Per-harness MCP config, work routing, calibration protocol |
| [`codehealth-baseline.json`](codehealth-baseline.json) | The committed baseline sweep |
| `tasks/R01..R19` | One self-contained task file per Red file |

Repo-root files that agents read: [`../../AGENTS.md`](../../AGENTS.md) (all harnesses) and
[`../../CLAUDE.md`](../../CLAUDE.md) (Claude Code).

## The mechanical guard

Because statcheck cannot yet cover this work, [`../../tools/refactor_guard.py`](../../tools/refactor_guard.py)
provides a cheap partial substitute. It compares every literal in a file before and after a
change; a removed or altered constant fails. It catches the single most dangerous class of
mistake - a changed magic number - with essentially no false positives on legal refactors.

It does **not** check control flow. An inverted condition passes it. It reduces risk for
Track A; on Track B it is combined with manual playtesting rather than statcheck, which
is indefinitely unavailable (see above).

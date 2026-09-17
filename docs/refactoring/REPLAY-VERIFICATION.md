# Replay-trace verification

The campaign has been running on the belief that a refactor can only be checked by
playing the game, because `statcheck` cannot be stood up. That is true of `statcheck`,
which compares against a CPS3 ROM. It is not true of behaviour checking in general.

`tools/compare_stress_replays.py` feeds **identical generated inputs to two builds of
3SX** and compares their per-frame rollback-state hashes. Baseline against candidate.
No ROM is involved.

```
tools/replay_verify.sh origin/main 12 1800
```

A divergence is reported by seed and frame, so it is reproducible.

## Setup, and the three things that make it look broken

1. **It needs a Debug build.** `--stress` sits behind `NETPLAY_ENABLED`, and only
   `CMAKE_BUILD_TYPE=Debug` defines it. A Release binary answers
   `error: unknown option --stress`, which reads like the feature is missing.
2. **It needs Python 3.10 or newer.** The tools use `X | None` annotations. The system
   `python3` on macOS is 3.9 and fails with a `TypeError` inside `stress_desync.py`
   before printing any usage.
3. **A fresh worktree has no `third_party`.** Those prebuilt dependencies are untracked,
   so a baseline worktree cannot configure until they are symlinked from the main
   checkout.

`tools/replay_verify.sh` handles all three.

## What it covers

Reading `src/platform/netplay/netplay_stress.c`:

- It taps START through the boot screens, enters **versus mode**, and picks characters
  with random pad input.
- The session starts **at the fight**. Character select and stage loading are
  deliberately outside the rollback window - too expensive to re-simulate.
- Inputs are one of eight directions, with a one-in-three chance of one of six attack
  buttons, held for 0-5 frames.

So it exercises the fight loop: movement, normals, guard, hit detection, damage, stun,
gauges, throws, and round end.

## What it costs, and how often to run it

Measured on an 8-core M-series Mac, with the baseline worktree and build cached
(`replay_verify.sh` only rebuilds the baseline when the ref actually moves):

| Run | Wall clock | Saved states compared |
| --- | --- | --- |
| 8 seeds x 600 frames | **22 s** | ~10,100 |
| 8 seeds x 1200 frames (default) | **~35 s** | ~15,000 |
| 12 seeds x 1800 frames | **91 s** | ~29,000 |
| 30 seeds x 3600 frames | **~8 min** | ~126,000 |

`compare_stress_replays.py` runs the seeds concurrently (`--jobs`, defaulting to one
per CPU). The runs are independent processes writing to separate directories, so this
changes wall-clock time only, never the traces. Before this, the same work ran serially
and 12 x 1800 took six minutes rather than ninety seconds.

**Widen the seeds, not the frames.** Seeds parallelise; frames do not. Doubling the
seed count is close to free until the cores run out, while doubling the frames doubles
the critical path. Seed diversity is also the better buy for finding a refactoring bug:
a mis-extracted branch either gets exercised in the first few hundred frames or is not
reached at all, whereas a longer run mostly revisits states it has already covered.

The resulting cadence (project owner directive, 2026-09-16):

- **Every commit: build plus `tools/refactor_guard.py`, including `--calls`.** This
  remains the routine gate. It is what catches the mistake class no build error will,
  and it costs seconds.
- **A replay run when the change is genuinely high risk.** Removing a `goto` into a
  `switch`, restructuring control flow where an early return has to be argued to reach
  the same exit, anything touching state the rollback system saves. The default
  `tools/replay_verify.sh origin/main` (8 x 1200, ~35 s) is the right size here. Not
  needed for an ordinary Recipe P predicate or a straightforward deduplication.
- **Before pushing a branch: one wide run,
  `tools/replay_verify.sh origin/main 30 3600`** (~8 min).

Judging which changes are risky is part of the work. Running the wide gate on everything
is not a substitute for that judgement, and it is not what the time is for.

If a pre-push run ever does diverge, bisect with the same tool: the seed and frame are
reported, and every commit is reachable as a worktree.

## The baseline has to be your branch point

`origin/main` is the obvious baseline and it is the wrong one as soon as main moves. The
checksum this harness records has already changed shape once - `fix(replay): stabilize
verification on Windows` moved `Stress_RecordState` out of `save_state` and into
`process_events`, where it re-gathers the state - so a build from one side of that commit
cannot be compared with a build from the other at all. Every seed diverges, on a branch
that changed nothing.

That happened in this campaign: main gained five commits mid-session, and runs that had
been clean started failing on work that was provably untouched. The branch's own base
verified clean against itself.

So: **compare against the commit your branch actually started from**, not against whatever
`origin/main` points at now.

```bash
tools/replay_verify.sh $(git merge-base origin/main HEAD)
```

`replay_verify.sh` warns when the baseline is not an ancestor of HEAD, and says how many
commits the baseline carries that your tree does not.

## Read the verdict, not the tail

Every run ends with one line, `REPLAY OK` or `REPLAY FAILED`, and the script exits
non-zero on divergence. Both exist because a diverging run looks like a passing one if you
read only the last couple of lines: the per-seed output ends with whichever seed ran last,
and a run where six seeds diverged and two did not will show two reassuring `identical`
lines at the bottom. Check the verdict line or the exit status.

## What it does not cover

This is the part that matters, and it is why replay verification **supplements**
targeted playtesting rather than replacing it.

- **CPU AI is excluded by design.** The stress session forces `operator = 1` for both
  players, because `cpu_algorithm()`'s state is not part of the saved State and would
  diverge on every rollback. Nothing in `com_sub`, `ck_pass`, or the CPU paths of
  `cmd_main` is checked.
- **Only versus mode.** Arcade progression, endings, bonus stages, training modes and
  the menus past boot are never reached.
- **Specific moves only by luck.** Random inputs will land some specials; a particular
  super motion is unlikely in any given run. A clean result is not evidence that a
  named move still works.
- **The round never ends.** Measured: at 7200 frames - two minutes of game time - the
  session still exits with *frame limit reached*, not *match ended*. Random flailing does
  not deal enough damage for a KO, and `Stress_OnFrameAdvanced` exits the moment the
  fight state ends anyway. So **no KO, round-end, win-pose or settle path is executed**,
  however many seeds you run. `settle_check`, `check_sa_resurrection` and everything
  downstream of them are invisible to this harness.

A file such as `manage.c` (round and continue flow) or `plmain2.c` (bonus stages) is
barely touched by this harness even when the run is clean.

## How to read a clean result

A clean run says: *for the inputs tried, the fight loop computes the same state, frame
for frame.* That is far stronger than "it looked fine when I played it", and it is the
one check that catches a one-frame divergence.

It does not say the change is correct everywhere. Name in the commit what the harness
could not reach, and playtest that.

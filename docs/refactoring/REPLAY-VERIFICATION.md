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

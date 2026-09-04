# Agent instructions

Read this before changing anything. It applies to every agent and every harness.

## What this repository is

3SX is a **native C port of Street Fighter III: 3rd Strike**, derived from a *matching
decompilation* of the PS2 build. Large parts of this code are a faithful reproduction of
original arcade behaviour, and the engine feeds a **rollback netcode** implementation.

The practical consequence:

> **Behaviour fidelity outranks code quality, readability, and your own judgement.**
> A change that makes the code cleaner but shifts behaviour by one frame is a defect,
> even if it looks obviously correct.

Code that appears wrong is usually right. Odd constants, redundant checks, and strange
control flow are typically arcade-accurate. **Report them, do not fix them.**

## Build

`cmake` is not on PATH by default on the Windows dev machine. Prepend the toolchain:

```bash
export PATH="/c/msys64/mingw64/bin:$PATH"
cmake --build build
```

- A no-op build takes ~2 seconds; a single-file change rebuilds and links in ~30 seconds.
- The build globs sources, so adding a `.c` file needs no CMake edit.
- Never commit a broken build.

There are **no unit tests**. CI builds on four platforms and runs no tests at all. Do not
assume anything is verified by the pipeline.

## Behavioural verification

`statcheck` replays real Fightcade matches frame by frame and compares engine state
against CPS3 RAM dumps. It is the only mechanism that proves behaviour was preserved.

It is currently **not running**: `THREESX_STATCHECK` defaults OFF, no CI workflow invokes
it, and the replay corpus and ROM are not present. See [`docs/statcheck.md`](docs/statcheck.md).

Until that changes, agents must not modify code inside the in-round simulation. The
refactoring campaign encodes this as a Track A / Track B split - respect it.

### User-authorized local experiments

For Track A code only, the repository owner may explicitly authorize a local experimental
refactoring session whose goal is manual gameplay evaluation. During such a session,
`refactor_guard.py` is advisory: a `WARN` or `FAIL` must be investigated and reported, but
does not require reverting or stopping when the owner has explicitly said to continue.

This exception does not authorize intentional behaviour changes, changes to in-round
simulation code, publishing, pushing, or opening a pull request. Before any experimental
work is committed for review, the normal build, guard, CodeScene, and commit requirements
below apply again unless the owner explicitly requests a separate review workflow.

## The refactoring campaign

If you were given a task ID like `R04`, your instructions are in
[`docs/refactoring/tasks/`](docs/refactoring/tasks/). Read, in order:

1. Your task file, e.g. `docs/refactoring/tasks/R04-menu.md`
2. [`docs/refactoring/PLAYBOOK.md`](docs/refactoring/PLAYBOOK.md) - the closed catalogue of allowed changes
3. [`docs/refactoring/README.md`](docs/refactoring/README.md) - campaign rules

You may apply **only** the five recipes in the playbook. Anything else is out of scope.

## Hard prohibitions

Never, in any commit:

- Change a numeric literal, string literal, character constant, or enum value.
- Change arithmetic, bitwise operations, or shifts.
- Change a comparison operator, including `<` to `<=`.
- Change a type, including signedness (`s8` to `u8`) or width (`s16` to `s32`).
- Reorder statements that have side effects.
- Delete code that looks dead - it may be reachable through the rollback path.
- Modify a `const` data table. The large static arrays are out of scope entirely.
- Fix a bug you noticed. Report it.

## Verification sequence

Run all three after **every** commit, in this order:

```bash
# 1. it still builds
export PATH="/c/msys64/mingw64/bin:$PATH"
cmake --build build

# 2. no constant was removed or altered
python tools/refactor_guard.py <the file you changed>

# 3. the metric improved  -- via the CodeScene MCP server
#    code_health_score(file_path="<absolute path>")
```

`refactor_guard.py` extracts every literal before and after your change and compares the
counts. It reports one of three things:

| Outcome | Meaning |
| --- | --- |
| `OK` | Nothing changed. |
| `FAIL` | A value vanished from the file, or a count dropped while another rose. That is a substituted constant - `30` became `31` - or deleted logic. |
| `WARN` | Counts only dropped, every value still present (deduplication); or counts only rose (a new guard clause bringing its own `return 0`). |

Outside a user-authorized local experiment, if the guard **fails**, revert immediately:

```bash
git checkout -- <file>
```

A `WARN` is not a pass mark, it is a request for a second look. Extracting a repeated
condition into one named predicate (Recipe P) and deduplicating (Recipe D) both remove
copies of literals, and to this tool they are indistinguishable from *deleting* one copy
of a duplicated block. Have a human confirm those before they land. `--strict` turns every
warning into a failure.

## Commits

- One recipe, one function, one commit.
- Message format: `refactor(<file>): simplify <function>`
- Never squash several functions into one commit. The campaign relies on being able to
  revert a single step.

## Git remotes and pull requests

This checkout is a **fork**, and the two remotes are easy to confuse:

| Remote | Repository | Use |
| --- | --- | --- |
| `origin` | `mrksbrg/3sx` | **This is the target for everything.** |
| `upstream` | `crowded-street/3sx` | The public project. Read-only, as far as agents are concerned. |

> [!WARNING]
> **Every pull request must target `mrksbrg/3sx`, base `main`.**
> Never open one against `crowded-street/3sx`.

This is not a hypothetical mistake. GitHub's "Compare & pull request" button and
`gh pr create` both default the base to the **upstream parent** when the repo is a fork,
so the wrong target is the *default* and it has already happened once. Always name the
target explicitly:

```bash
gh pr create --repo mrksbrg/3sx --base main
```

Or set it once per clone, after which the default is safe:

```bash
gh repo set-default mrksbrg/3sx
```

Also: **never `git push upstream`.** That remote has a push URL configured, so the push
would succeed rather than fail safely.

This work is private refactoring research. It is not intended for the public project
unless the maintainer decides otherwise.

## When to stop

Outside a user-authorized local experiment, stop and report, rather than pressing on, whenever:

- The baseline score in your task file does not match what you measure.
- The build fails and the fix is not obvious.
- The guard fails and you do not understand why.
- A function's control flow is too tangled to transform confidently.
- You want to make a change the playbook does not cover.

**An unfinished task is an acceptable outcome. A silently broken fighting-game engine is
not.** Reporting "I completed 2 of 5 steps and stopped because step 3 was unclear" is a
good result.

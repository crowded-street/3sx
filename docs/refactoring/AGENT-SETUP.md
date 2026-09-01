# Agent setup and work routing

How to point each harness at this campaign, how to hand out a task, and which agent should
get which kind of work.

Three harnesses are in use:

| # | Harness | Instruction file it reads | MCP config |
| --- | --- | --- | --- |
| 1 | Claude Code | `CLAUDE.md` (plus `AGENTS.md`) | `.mcp.json` (repo root) |
| 2 | Codex | `AGENTS.md` | `~/.codex/config.toml` |
| 3 | OpenCode | `AGENTS.md` | `opencode.json` (repo root) |

`AGENTS.md` at the repo root is the shared contract. Everything harness-specific lives
here or in `CLAUDE.md`.

---

## 1. Claude Code

Already configured and verified. `.mcp.json` launches `cs-mcp` through Node and reads
`CS_ACCESS_TOKEN` from the environment. Confirm with:

```
verify_installation(git_repository_path="<repo root>")
```

Expect 5/5 checks passing. `.mcp.json` is machine-specific and is not committed.

---

## 2. Codex

Add to `~/.codex/config.toml`. This matches the schema already in use in that file for
other servers:

```toml
[mcp_servers.codescene]
command = 'node'
args = ['C:\Users\<you>\AppData\Roaming\npm\node_modules\@codescene\codehealth-mcp\bin\cs-mcp.js']
startup_timeout_sec = 120

[mcp_servers.codescene.env]
CS_ACCESS_TOKEN = "<your token>"
CS_DISABLE_VERSION_CHECK = "1"
```

Notes:

- Invoke `node` against the wrapper script rather than the `cs-mcp` / `npx` `.cmd` shims.
  Those shims are a common source of MCP spawn failures on Windows.
- The repo's parent directory is already `trust_level = "trusted"` in that config, so no
  additional trust entry is needed.
- Raise `startup_timeout_sec` on first run: the wrapper downloads a ~79 MB binary once.

---

## 3. OpenCode

> [!IMPORTANT]
> **Unverified.** OpenCode is not installed on this machine, so unlike the two above this
> block was not tested. Check it against your installed version's schema before relying
> on it, and correct this file if it differs.

Create `opencode.json` at the repo root:

```json
{
  "$schema": "https://opencode.ai/config.json",
  "mcp": {
    "codescene": {
      "type": "local",
      "command": [
        "node",
        "C:/Users/<you>/AppData/Roaming/npm/node_modules/@codescene/codehealth-mcp/bin/cs-mcp.js"
      ],
      "environment": {
        "CS_ACCESS_TOKEN": "{env:CS_ACCESS_TOKEN}",
        "CS_DISABLE_VERSION_CHECK": "1"
      },
      "enabled": true
    }
  }
}
```

If MCP cannot be made to work there, that harness is still usable - see *Fallback without
MCP* below.

---

## Routing work by recipe, not by file

The recipes in [`PLAYBOOK.md`](PLAYBOOK.md) differ sharply in how much reasoning they need.
Route on that, not on file size:

| Recipe | What the agent must reason about | Needs |
| --- | --- | --- |
| **P** - named predicate | Nothing. Copy an expression into a function, character for character. | Any agent |
| **G** - guard clauses | What the function returns when it falls through. Local, but real. | Mid capability |
| **E** - extract function | Which variables cross the boundary, and which are written. Data flow. | Strongest agent |
| **D** - deduplicate | Whether two blocks are truly identical. **Trips the guard by design.** | Strongest + human review |
| **S** - split file | Module boundaries, linkage, what stays `static`. | Strongest + human review |

A useful rule: **Recipe P is safe to hand to the smallest model you have.** It is a
copy-paste transformation with a mechanical checker behind it. Recipe E is where a weaker
model will silently drop a variable, and where the build may still succeed.

### Suggested starting split

Do not treat this as settled - calibrate first (next section).

- **Claude Code / Sonnet** - Recipe E and S. The tasks needing judgement: R14 `bg.c`
  (`scr_trans`, cyclomatic 109, nesting 9) is the hardest single unit in the campaign.
- **Codex** - Recipe G and E on mid-difficulty Track A files: R06 `mtrans.c`, R11 `PPGFile.c`.
- **OpenCode / smaller model** - Recipe P only, across any Track A file. There are
  plenty of complex-conditional findings to work through, and each one is independently
  verifiable.

---

## Calibrate before you commit to a split

The table above is a hypothesis. Two of these three model/harness pairings have never run
against this codebase, and model capability is not something to assume.

**Run the same probe on all three, then compare:**

1. Give each agent **R17 `entry.c`** (Track A, LOW risk, 1480 lines, baseline 3.86 - the
   cheapest Red file to be wrong about).
2. Restrict every agent to **Recipe P only**, steps 1-3.
3. Compare on four axes:
   - Did the score improve, and by how much?
   - Did `refactor_guard.py` pass?
   - Did it stop when told to, or keep going past the target?
   - Did it stay inside the playbook, or invent a transformation?

Give each a clean branch so the runs do not interfere:

```bash
git checkout -b probe/entry-<agent-name> main
```

The fourth axis matters most. An agent that improves the score while quietly making changes
the playbook forbids is worse than one that stops early, because statcheck is not yet there
to catch it.

Record what you find in [`BACKLOG.md`](BACKLOG.md) and route the real work accordingly.

---

## Dispatching a task

Give the agent exactly this, with the task ID substituted:

```
Work task R17 in this repository.

Read, in this order:
  1. AGENTS.md
  2. docs/refactoring/PLAYBOOK.md
  3. docs/refactoring/tasks/R17-entry.md

Follow the task file exactly. Apply only Recipe P. Do steps 1 through 3, then stop.

After every commit run all three checks:
  export PATH="/c/msys64/mingw64/bin:$PATH" && cmake --build build
  python tools/refactor_guard.py src/sf33rd/Source/Game/screen/entry.c
  code_health_score on the same file

If the guard fails, revert that commit and move on.
Stop and report if anything is unclear. An unfinished task is fine.

Finish with the report block at the end of the task file, filled in.
```

Keep the "stop and report" line. Weaker models will otherwise improvise once they run out
of instructions, and improvisation is the failure mode that matters here.

---

## Fallback without MCP

If a harness cannot reach the CodeScene MCP server, it can still measure via the sweep:

```bash
python tools/codehealth_sweep.py -o /tmp/after.json
```

Then read the file's score out of the JSON. This is much slower than a single MCP call -
it scores all 624 files - so prefer MCP where available. An agent with neither should be
given Recipe P work only, and its output reviewed by a human before merge.

---

## Reviewing agent work

Before merging any campaign branch:

1. `python tools/refactor_guard.py --all --base main` - no removed literals.
2. `git log --oneline main..` - one function per commit, correct message format.
3. `git diff main -- <file>` - scan for anything outside the playbook. Particularly:
   changed comparison operators, changed types, deleted branches.
4. Confirm the reported final score against a fresh `code_health_score` call.

Do not skip step 3 because steps 1 and 2 passed. The guard checks constants, not control
flow - an inverted condition with no literal change passes it cleanly.

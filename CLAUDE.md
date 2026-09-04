# CLAUDE.md

**Read [`AGENTS.md`](AGENTS.md) first.** It is the authoritative contract for all agents
working in this repository, and everything in it applies to you. This file only adds what
is specific to Claude Code.

## Claude Code specifics

**CodeScene MCP** is configured in [`.mcp.json`](.mcp.json) at the repo root. It exposes
`code_health_review`, `code_health_score`, and `pre_commit_code_health_safeguard`. If those
tools are missing, the server did not start - restart Claude Code rather than proceeding
without measurement.

`.mcp.json` reads `CS_ACCESS_TOKEN` from your environment. It contains a machine-specific
absolute path and is intentionally not committed.

**Skills.** [`.claude/skills/guiding-refactoring-with-code-health/`](.claude/skills/guiding-refactoring-with-code-health/)
loads automatically when you refactor. It enforces the measure-first loop: baseline review,
then small structural steps, re-measuring after each. Follow it.

The repo also carries [`.agents/skills/compare-cps3-code/`](.agents/skills/compare-cps3-code/)
for CPS3 parity work against the Ghidra decomp. That is a separate workflow from the
refactoring campaign - do not mix the two in one session.

## Refactoring campaign

Task files live in [`docs/refactoring/tasks/`](docs/refactoring/tasks/). They name the
CodeScene MCP tools directly, so you can execute them as written.

Because you have the MCP server, you are the right harness for the tasks that need
judgement - Recipe E (extract function) and Recipe S (split file), where the agent must
reason about which variables cross the boundary. See
[`docs/refactoring/AGENT-SETUP.md`](docs/refactoring/AGENT-SETUP.md) for how work is routed
across harnesses.

## Reminders that matter here

- Run `python tools/refactor_guard.py <file>` after every commit. It is cheap and it
  catches the one class of mistake that no build error will.
- **Track B gate relaxed 2026-09-04 (project owner directive).** Statcheck cannot be
  stood up: a legitimate CPS3 ROM dump requires owning genuine CPS3 arcade hardware,
  which this project's no-piracy policy won't work around (`docs/statcheck.md`,
  `AGENTS.md`). This is not "not yet" - treat it as indefinite. Track B files may be
  refactored; verification is manual playtesting instead of statcheck. Still follow the
  closed recipe catalogue exactly, one recipe per commit, build + `refactor_guard.py`
  after every commit, and flag CRITICAL-risk files clearly so playtesting is targeted.
- Stop at a Code Health score of 4.00. Do not gold-plate.

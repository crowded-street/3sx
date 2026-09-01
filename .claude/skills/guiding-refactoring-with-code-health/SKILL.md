---
name: guiding-refactoring-with-code-health
description: Use when refactoring unhealthy code and needing Code Health findings to choose small safe steps and verify improvement.
---

# Guiding Refactoring With Code Health

## Overview

Use Code Health as the control signal for refactoring. The agent should first understand why a file is hard to work with, establish a measurable baseline, then improve it in small structural steps and verify that each step helped. The goal is not just cleaner code, but code that is easier for both humans and agents to understand and modify safely.

## When to Use

- A file is hard to read, risky to change, or repeatedly attracts defects.
- The user asks for refactoring help and wants an objective way to measure progress.
- A safeguard or review points to complexity, size, low cohesion, or deep nesting.

Do not use this skill when the task is to rank project-wide priorities. Use `prioritizing-technical-debt` for that.

## Quick Reference

- `code_health_review`: Detailed maintainability findings for a file.
- `code_health_score`: Numeric baseline and trend check across refactoring iterations.

## Implementation

1. Run `code_health_review` on the target file.
2. Record the current `code_health_score` so the refactoring starts from a measurable baseline.
3. Identify the highest-leverage structural problems, such as excessive responsibilities, deep nesting, low cohesion, or hard-to-follow control flow.
4. Propose 3 to 5 small structural refactor steps, not a single rewrite.
5. After each meaningful step, re-run `code_health_review` to see whether the targeted structural problems were reduced.
6. Use `code_health_score` as the compact checkpoint to confirm directional improvement across iterations.
7. Stop only when the targeted structural issues are substantially reduced and the score has measurably improved, or when the user explicitly accepts a partial uplift.

## Common Mistakes

- Refactoring without a baseline review.
- Refactoring without recording the initial score.
- Making a large rewrite that hides whether things improved.
- Counting cosmetic cleanup as meaningful progress when the structural problems remain.
- Using score checks alone without re-running the detailed review.
- Forgetting to re-measure after each meaningful step.
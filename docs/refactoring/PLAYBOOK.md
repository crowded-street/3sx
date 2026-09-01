# Refactoring Playbook

This is a **closed catalogue**. If a change you want to make is not one of the recipes
below, you are not allowed to make it as part of this campaign. Stop and report instead.

Every recipe here is *behaviour-preserving*: it moves code around without changing what
the program computes. That property is what makes this campaign safe to run at scale,
and it is the only reason a task can be handed to an agent that cannot run the game.

---

## The three rules

1. **One recipe, one function, one commit.** Never combine two recipes in one commit.
2. **Re-measure after every commit.** If the Code Health score did not improve, revert.
3. **When in doubt, stop and report.** An unfinished task is fine. A silently broken
   fighting-game engine is not.

---

## Absolutely forbidden

These will change behaviour and must never appear in a campaign commit:

- Changing any numeric literal, string literal, or enum value.
- Changing arithmetic (`+ - * / %`), bit operations, or shifts.
- Changing a comparison operator, including `<` to `<=`.
- Reordering statements that have side effects (assignment, I/O, function calls).
- Changing types, including signedness (`s8` to `u8`) and width (`s16` to `s32`).
- "Fixing" a bug you spot. **Report it, leave it.** A faithful port reproduces original
  behaviour; what looks like a bug is often arcade-accurate.
- Deleting code that looks dead. It may be reachable through the replay/rollback path.
- Touching a `const` data table (the large static arrays). They are out of scope entirely.
- Renaming an existing function that is referenced from another file.

---

## Recipe E - Extract Function

**Use when:** CodeScene reports *Bumpy Road Ahead* (a function with several separate
blocks of nested logic). Each "bump" is a missing function.

**How:**

1. Find one nested block. It usually already has a comment or a blank line around it.
2. Cut it into a new `static` function directly above the current one.
3. Pass in every variable it reads as a parameter. Return the single value it produces.
4. If the block writes to more than one outer variable, **skip it** and move to the next
   bump. Do not invent an out-parameter struct.

**Before:**

```c
void update_player(Player *p) {
    // ... other work ...
    if (p->state == STATE_HIT) {
        if (p->hit_timer > 0) {
            p->hit_timer--;
            if (p->hit_timer == 0) {
                p->state = STATE_IDLE;
            }
        }
    }
}
```

**After:**

```c
static void tick_hit_recovery(Player *p) {
    if (p->state != STATE_HIT) {
        return;
    }
    if (p->hit_timer <= 0) {
        return;
    }
    p->hit_timer--;
    if (p->hit_timer == 0) {
        p->state = STATE_IDLE;
    }
}

void update_player(Player *p) {
    // ... other work ...
    tick_hit_recovery(p);
}
```

Name the new function for **what it does**, not where it came from. `tick_hit_recovery`,
not `update_player_part2`.

---

## Recipe G - Guard Clauses

**Use when:** CodeScene reports *Deep, Nested Complexity* (nesting depth 4 or more).

**How:** invert the outermost condition and return early. Repeat until the main body sits
at one level of indentation. This is the highest-value recipe in the campaign - it is
mechanical, and it directly attacks the smell that correlates most strongly with defects.

**Before:**

```c
if (player != NULL) {
    if (player->active) {
        if (player->health > 0) {
            do_the_real_work(player);
        }
    }
}
```

**After:**

```c
if (player == NULL) {
    return;
}
if (!player->active) {
    return;
}
if (player->health <= 0) {
    return;
}
do_the_real_work(player);
```

**Careful:** if the function returns a value, every guard must return the *same* value the
original code would have produced by falling through. Read the end of the function before
you start. If you cannot tell what it falls through to, skip the function and report it.

---

## Recipe P - Named Predicate

**Use when:** CodeScene reports *Complex Conditional* or *Complex Method* driven by
compound boolean expressions.

**How:** move the expression into a `static` function whose name states the intent. This
usually needs no other change, and it is the safest recipe in the catalogue.

**Before:**

```c
if ((p->flags & FLAG_AIR) && p->vy < 0 && !(p->flags & FLAG_STUN) && p->combo < 3) {
    ...
}
```

**After:**

```c
static bool can_air_combo(const Player *p) {
    return (p->flags & FLAG_AIR) && p->vy < 0 && !(p->flags & FLAG_STUN) && p->combo < 3;
}

if (can_air_combo(p)) {
    ...
}
```

Copy the expression **character for character**. Do not "simplify" the boolean algebra,
and do not reorder the operands - `&&` short-circuits, so reordering can change which
side effects run and can introduce null dereferences.

---

## Recipe D - Deduplicate

**Use when:** CodeScene reports *Code Duplication*.

**How:** only when the blocks are **identical or differ by a single value**. Extract one
`static` helper and pass the differing value as a parameter. If they differ in two or more
places, leave them alone - forcing a shared abstraction over near-miss duplicates is how
faithful ports get broken.

---

## Recipe S - Split File

**Use when:** CodeScene reports *Lines of Code in a Single File*.

**This is the last recipe you apply, never the first.** Reduce function complexity first;
the file often drops out of the Red band without needing a split at all.

**How:**

1. Group functions that share a `static` variable or an obvious theme.
2. Move that group to a new `.c` file beside the original.
3. Add a matching `.h` **only** for functions used from outside the new file.
4. The build globs sources, so no CMake edit is needed - but re-run `cmake --build build`
   and confirm the link step succeeds.

Do not split a file if it would require making a `static` function non-`static`. Widening
linkage is a behaviour change in the sense that matters here: it changes what the rest of
the program can reach.

---

## The verification loop

After **every** commit:

```bash
cmake --build build
```

`cmake` is not on PATH by default on the Windows dev machine. Prepend the toolchain:

```bash
export PATH="/c/msys64/mingw64/bin:$PATH"
```

A single-file change rebuilds and links in roughly 30 seconds.

Then check that no constant moved:

```bash
python tools/refactor_guard.py <file>
```

This extracts every numeric, string, and character literal from the file before and after
your change and compares the counts:

- **FAIL** - a value vanished from the file, or a count dropped while another rose. That
  is a substituted constant (`30` became `31`) or deleted logic. Revert.
- **WARN** - counts only dropped and every value is still present. That is what
  deduplication looks like. Recipes **D** and **P** both produce it legitimately: pulling
  a repeated condition into one named predicate removes copies of its literals without
  changing any of them.
- **WARN** - counts only rose. A new guard clause brings its own `return 0`.
- **OK** - nothing changed.

A `WARN` is not a pass mark, it is a request for a second look. The tool cannot tell
deduplication apart from *deleting* one copy of a duplicated block, so a human confirms
those before they land. `--strict` turns warnings into failures.

Then re-measure with the CodeScene MCP server:

```
code_health_score(file_path="<absolute path to the file>")
```

| Result | Action |
| --- | --- |
| Build fails | Fix it, or revert. Never commit a broken build. |
| Guard FAILs | `git checkout -- <file>`. No exceptions. |
| Score up | Keep it. Commit. Next function. |
| Score flat or down | `git checkout -- <file>`. Next function. |

The guard checks constants, not control flow. An inverted condition with no literal change
passes it cleanly - so it reduces risk, it does not remove it. That is why Track B stays
closed until statcheck runs.

Use `code_health_review` (not just the score) when you need to see *which* smells remain -
the score alone will not tell you whether you hit the right problem.

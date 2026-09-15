# Playbook review - 2026-09-15

A review of the closed catalogue in [`PLAYBOOK.md`](PLAYBOOK.md), written after taking
thirteen files to Code Health 10.00 in one session: the ten `com_sub` files (R02) and six
effect files. It is a **proposal**, not a change. Nothing here is adopted until the
project owner says so.

## The framing that matters

The catalogue is often described as being sized for weak models. That is only half true,
and the half that is false is the important one.

The recipes exist because **`statcheck` cannot run**. They are a substitute for tests. A
transformation is allowed when it is behaviour-preserving *by construction* - so obviously
safe that a reviewer can confirm it by reading a diff, and `refactor_guard.py` can check
its literal fingerprint. That reasoning is about **verification**, not about how clever the
agent is.

So a stronger model does not earn a licence to change literals, reorder side effects, or
"fix" arcade-accurate oddities. The cost of an undetected behaviour change is unchanged:
a one-frame divergence that breaks rollback determinism and that no build error, and no
amount of confidence, will catch.

What a stronger model *does* change is the set of transformations it can carry out
**correctly and legibly**. Several of the catalogue's limits are not safety properties at
all - they are simplifications that keep a small model out of trouble, and they currently
block work that is provably safe. Those are worth revisiting. The semantic prohibitions
are not.

Below: what actually blocked me, with evidence; what is internally inconsistent; and what
I would change.

---

## 1. Gaps that blocked safe work

### 1.1 Excess Number of Function Arguments had no recipe *(already resolved)*

This was the single largest ceiling in the campaign. Seven `com_sub` files sat at
9.38-9.68 with every other smell cleared and no legal move left, because the Term entry
points took 9-13 arguments and no recipe addressed argument count.

The owner authorised a **parameter object** recipe on 2026-09-15, including public
signatures. Result: 21 entry points converted, ~5,500 call sites rewritten across 53
character-script files, seven files reached 10.00.

**Recommendation:** write it into the catalogue as **Recipe A - Parameter Object**, with
the preconditions that made it safe here:

- The struct's field order is the function's old parameter order, and field types are the
  old parameter types exactly. No widening, no reordering, no merging of two fields.
- Call sites pass their original argument list through a compound literal, unchanged. The
  literal fingerprint of every changed file must come back **OK**, not merely WARN - if a
  literal moved or changed, the rewrite was wrong.
- Two functions may share a struct only if their parameter lists are identical in order
  *and* type. `Jump_Term_Args` and `Hi_Jump_Term_Args` stayed separate for one signed
  versus unsigned field; merging them would have been a type change.
- Where two families need the same subset (the approach gates, the landing step), give
  that subset its own small struct and build it at the call site from fields already held.

### 1.2 Recipe D cannot express "share the identical part"

Recipe D allows sharing only when blocks are *identical or differ by a single value*, and
tells you to leave anything else alone. That rule misses a shape that is **strictly safer
than what Recipe D already allows**, and it came up repeatedly:

> Two blocks are byte-identical for a contiguous run, and differ in what comes after.

Recipe D parameterises the difference - the differing value travels into the helper. The
shape below does the opposite: it extracts **only the identical run** and leaves every
difference at the call site, where it stays visible and unchanged. Nothing is
parameterised, so there is less room to get it wrong, not more.

Real cases from this session, all currently illegal or awkward under the letter of the
rule:

| Pair | Shared | Differs |
| --- | --- | --- |
| `advance_eff09_4000` / `advance_eff09_11000` | the whole gate, move and countdown | one final statement |
| `Lever_Attack_Begin` / `Lever_Attack_SP_Begin` | both gates and the dash clear | one extra assignment |
| `Lever_Attack_Wind_Up` / `Lever_Attack_SP_Wind_Up` | gate, countdown, guard lever | one timer decrement |
| `HJA_Term_Begin` / `ORO_HJA_Term_Begin` | three gates and the menu reset | one statement before, one after |

Each merged cleanly, built, passed the guard, and moved the score (+0.26, +0.51, +0.27,
+0.81). I applied them as "Recipe D", which is a stretch of the written rule.

**Recommendation:** add **Recipe C - Extract Common Part**:

- Use when CodeScene reports Code Duplication and the blocks share a contiguous identical
  run - prefix, suffix, or middle.
- Extract **exactly** the identical run, character for character. Everything that differs
  stays at the call sites, in its original order.
- If the run ends inside control flow - the callers need to know whether to carry on -
  the helper returns `0`/`1` and the callers branch on it. Nothing else may be returned.
- This does **not** license merging near-miss blocks by parameterising two or more
  differences. That prohibition is the point of Recipe D and stays.

### 1.3 The dispatch split has no name

I used one move perhaps fifteen times: a `switch` with too many arms is split in two, the
later arms moving to a helper reached through `default:`, **with every case label
unchanged**. It reduces cyclomatic complexity with no change to behaviour or to the state
numbering a reader sees.

It is not really Recipe E - nothing here is a "bump of nested logic"; it is arm
partitioning. It deserves its own entry so that agents apply it deliberately and reviewers
recognise it.

**Recommendation:** add **Recipe X - Split Dispatch**, with the binding constraint stated
loudly: **case labels are never renumbered.** The helper switches on the same expression
and keeps the original labels, so states still read as the numbers the rest of the engine
uses. An offset - `CP_Index - 2` with labels `0..4` - is a literal change and is forbidden.

### 1.4 Recipe E's "more than one outer variable" rule is stricter than intended

Recipe E says to skip a block that "writes to more than one outer variable". Read
literally that rules out almost every extraction in this codebase, since the blocks write
many fields through `ewk`, `wk` or `mwk`.

It is clear from the example that the rule means *local* variables. Writing many fields
through a pointer parameter is safe and is what the whole campaign has been doing.

**Recommendation:** reword to "more than one outer **local** variable". Add: passing a
struct pointer that the block reads and writes is fine and expected.

---

## 2. Internal inconsistencies

### 2.1 Rule 2 contradicts "A flat score does not mean a failed refactor"

Rule 2 says: *"Re-measure after every commit. If the Code Health score did not improve,
revert."* A later section says the opposite, correctly: a flat score with the targeted
smell gone is a keep, and reverting on flat would throw away good work.

I hit this on at least six commits this session. I followed the later section.

**Recommendation:** rewrite rule 2 to point at the review-based test: *re-measure after
every commit; if the score did not improve, run the review and keep the change only if the
targeted smell left its category or its complexity dropped.*

### 2.2 "One recipe, one function, one commit" costs more than it protects

The rule's real value is revert granularity for playtesting, which is worth keeping. But
"never combine two recipes" made me rebuild a pair of commits by hand in
`com_sub_attack.c` for no safety gain - two independent deduplications in one file that
could not be told apart by a playtester anyway.

**Recommendation:** keep one *function* per commit. Allow several applications of the
*same* recipe in one commit when they are the same transformation on that function.
Require the commit message to name the recipe, the before/after score, and what to play -
that is what actually helps the human at the other end.

---

## 3. Where the metric fights the work

Three times this session, a correct transformation **lowered** the score, because
extracting a helper made two sibling state machines look alike and the duplication
detector priced that above the complexity removed:

- `ORO_JA_Term`'s airborne split: -0.25, then -0.87 on a second attempt at a larger split.
- `Command_Attack`'s complexity fix: -0.57 - until the parameter structs made the three
  dispatchers stop reading alike, after which the same fix was free and took the file to
  10.00.

There is a lesson here worth writing down: **duplication-by-shape between sibling state
machines is an artifact of decomposition, not a defect introduced by it.** Chasing it
leads to churn; tolerating it leaves a real ceiling.

**Recommendation:** add a short section, *When the score falls but the review improves*:
record both numbers in the commit message, prefer the variant with the better review, and
do not attempt to "fix" sibling similarity by merging state machines that differ only in
their state numbering.

---

## 4. Plateaus are real and should be named

Some files cannot reach 10.00 under any safe rule, and an agent that does not know this
will burn a session rediscovering it. All of these are duplication between near-miss
siblings:

| File | Plateau | Why |
| --- | --- | --- |
| `com_sub_air_term.c` | 9.68 | `ORO_JA_Term` at cc 9; clearing it creates a twin, -0.87 |
| `com_sub_attack.c` | 9.09 | the two normal-attack wind-ups differ in two statements |
| `com_sub_command_term.c` | 9.09 | two pairs of airborne twins, one state number apart |
| `eff09.c` and its three split files | 8.54-8.81 | five near-miss pairs; e.g. `adjust_sean_ball_left`/`_right` differ in five values |

**Recommendation:** add an *Expected plateau* field to the task template, and record the
figure once a file is worked out, with the reason. A plateau is a result, not a failure.

---

## 5. Strengthening the guard, so relaxations stay cheap

`refactor_guard.py` compares literal fingerprints. It is good at catching a substituted
constant and blind to everything else - it says so itself.

One cheap addition would cover a real risk class, and would make Recipe C and Recipe X
safer than they are today:

**A call-fingerprint mode.** Collect the multiset of called function names per file,
before and after. Extraction adds exactly one name - the new helper - and moves calls
between functions, but the multiset of *callee names* is otherwise invariant. A dropped
call, a duplicated call, or a call that quietly went missing inside a rewritten branch
would show up immediately. It does not catch reordering, but it catches deletion and
duplication, which is what a botched extraction actually does.

This is maybe forty lines in the existing tool, reusing its comment stripper.

---

## 6. What must not change

For the avoidance of doubt, and regardless of which agent is running:

- Numeric, string and enum literals. Case labels included - renumbering states is a
  literal change even when it looks like arithmetic.
- Arithmetic, bit operations, shifts, comparison operators. (Recipe G's inversion of a
  condition is the one sanctioned exception, and only as part of a guard clause.)
- The order of side-effecting statements.
- Types, including signedness and width. This is why two near-identical parameter structs
  stayed separate.
- Bugs you spot. This session found two: the swapped thresholds in `eff09.c`'s
  `adjust_sean_ball_left`/`_right`, and a duplicated `my_col_code` assignment in
  `eff01.c`. Both are reported in comments and left exactly as they were.
- Dead-looking code, `const` data tables, and the linkage of `static` functions.

---

## 7. Summary of proposed changes

| # | Change | Kind |
| --- | --- | --- |
| 1 | Add **Recipe A - Parameter Object** with its preconditions | Write down what is already authorised |
| 2 | Add **Recipe C - Extract Common Part** | New capability, strictly safer than Recipe D |
| 3 | Add **Recipe X - Split Dispatch**, labels never renumbered | Name a move already in wide use |
| 4 | Recipe E: "outer variable" -> "outer **local** variable" | Fix wording |
| 5 | Rule 2: revert-on-flat -> review-based test | Fix contradiction |
| 6 | Rule 1: one *function* per commit; same recipe may repeat | Reduce friction, keep granularity |
| 7 | Add *When the score falls but the review improves* | Guidance for a real, measured case |
| 8 | Add *Expected plateau* to the task template | Stop future churn |
| 9 | Add a call-fingerprint mode to `refactor_guard.py` | Make the above cheaper to trust |

Items 1-8 are documentation. Item 9 is a small tool change.

None of them relax a semantic prohibition, and none of them depend on trusting the agent's
judgement in place of a check.

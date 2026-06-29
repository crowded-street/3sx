---
name: compare-cps3-code
description: Compare and synchronize this codebase with the CPS3 game program in the configured Ghidra project, for individual symbols, selected functions, or whole source files. Use when asked to inspect CPS3 parity, identify behavioral differences or version-exclusive functions, annotate parity status, or implement CPS3-matching behavior behind the arcade balance flag. Treat comparison requests as authorization to add guarded CPS3 behavior unless the user explicitly requests a read-only report.
---

# Compare CPS3 Code

Use the repository's Ghidra query bridge to compare local C code with the CPS3 decomp. Treat decompiler output as evidence to interpret, not source text that must be copied literally.

## Query Ghidra

Run commands from the repository root:

```sh
tools/ghidra/ghidra_query.py --search SYMBOL_FRAGMENT --limit 25
tools/ghidra/ghidra_query.py --target SYMBOL_OR_ADDRESS --decompile
tools/ghidra/ghidra_query.py --target SYMBOL_OR_ADDRESS --decompile --bytes 32
```

Read `tools/ghidra/README.md` if the bridge is not configured or a query fails. Use symbol searches to resolve uncertain names, then decompile exact targets. For whole-file requests, enumerate the local functions first and query their likely CPS3 counterparts systematically.

For numeric data addresses in decompiler output, query the address directly and inspect `addressContext`. Prefer `containingData`, its type, and its `offsetHex` when Ghidra has defined the object; otherwise use `previousSymbol` and its `offsetHex` to reconstruct the base-plus-offset expression. Correlate that offset with local struct and array layouts instead of treating raw constants as unexplained behavior. The following symbol can help bound the inferred object, but do not assume the preceding symbol contains the address without layout evidence.

When Ghidra shows unlabeled data, generated field names, or raw offsets and you can infer a likely local identity, explicitly report that inference to the user. Name the generated label or offset, the suspected local symbol or struct field, and the evidence used, such as matching offsets, call arguments, table layout, or nearby assignments. Keep inference separate from certainty: say when it is only a likely match, and ask for Ghidra labeling if the ambiguity affects the comparison.

If comparison is blocked by likely unlabeled or misleading symbols in Ghidra, stop at that ambiguity and ask the user to label the relevant symbols in Ghidra. Include the addresses, current generated names, and the labels or relationships needed to continue. Do not guess through material symbol ambiguity.

## Compare Semantics

Compare control flow, constants, data accesses, calls, side effects, signedness, and ordering. Account for normal decompiler artifacts such as temporary variables, casts, flattened expressions, and recovered types. State uncertainty when types or symbols prevent a firm conclusion.

Limit work to the symbols or files requested. For a whole file, account for every local function and report CPS3-only functions discovered while resolving its counterparts.

## Mark Function Parity

Add exactly one status comment on the same line as each compared local function's opening brace:

```c
void Example(void) { // 🟢
```

Use:

- 🟢 when the function fully matches CPS3 behavior.
- 🟡 when the function exists in both versions but differs from CPS3 in any behavior.
- 🔴 when the function exists only in this codebase's version.
- 🔵 when the function exists only in CPS3 and a corresponding local stub or declaration with a body exists to annotate.

Do not fabricate a local function merely to place a 🔵 marker. When a CPS3-only function has no local body, list it in the comparison result with its symbol or address.

Replace an existing parity emoji on the opening-brace line rather than adding another. Preserve unrelated comments and formatting where practical.

For every 🟡 function, leave a nearby short code comment explaining the intentional difference or the guarded arcade-balance branch. This is especially important when the final code keeps a port-only side effect that is not visible in CPS3, such as controller vibration, training-mode reporting, diagnostics, save/netplay bookkeeping, or other platform affordances. Keep the comment factual and local to the differing code path.

## Implement CPS3 Behavior

Treat every comparison request as authorization to update the requested local symbols so they can reproduce CPS3 gameplay behavior. Do not stop after identifying a gameplay or balance difference when a safe implementation is possible. Skip edits only when the user explicitly requests comparison without modification, the local symbol is version-exclusive, or unresolved evidence makes a faithful change unsafe.

For each symbol that exists in both versions and differs in gameplay-relevant behavior, implement the smallest faithful CPS3 path. Keep version-specific gameplay behavior gated by `ArcadeBalance_IsEnabled()` from `src/arcade/arcade_balance.h`:

```c
#include "arcade/arcade_balance.h"

if (ArcadeBalance_IsEnabled()) {
    /* CPS3 behavior */
} else {
    /* existing behavior */
}
```

Preserve existing behavior when the flag is disabled. If CPS3 omits behavior that the local version performs, guard the local-only behavior with `!ArcadeBalance_IsEnabled()`. Prefer a narrow conditional around the actual difference instead of duplicating an entire function.

Do not add an `ArcadeBalance_IsEnabled()` gate solely because behavior is controlled by a dipswitch or similar local option. Treat the dipswitch as the gate: compare the option's default value against CPS3 behavior first. If the default local behavior matches CPS3, leave the option-controlled logic as-is and document the version/option difference when the function remains 🟡. If the default local behavior diverges in gameplay-relevant ways, implement the smallest arcade-balance adjustment around the default/config interaction rather than blindly wrapping every dipswitch branch.

Do not gate non-gameplay port affordances behind `ArcadeBalance_IsEnabled()`. Examples include controller vibration and other `pp_pulpara*` feedback, training-mode display/reporting metadata, debug or diagnostics, save-state/netplay bookkeeping, platform I/O, and other user-facing conveniences that do not change match rules, combat state, RNG, timers, scoring, damage, meter, input interpretation, collision, or round flow. Keep those effects unconditional unless there is evidence that they affect gameplay parity.

Assign the parity emoji after making the change, based on the function's final contents. Retain 🟡 when the function contains an arcade-balance branch or any other intentional version-dependent or port-only behavior; its guarded contents or nearby comment must visibly explain the difference. Use 🟢 only when the final function matches CPS3 without qualification. Never leave a 🟡 marker on a safely fixable gameplay difference without also representing the CPS3 behavior in its contents.

Verify edited code with the narrowest relevant formatter, build, or test available. Summarize matched functions, differences, version-exclusive functions, edits, and unresolved symbol-labeling needs.

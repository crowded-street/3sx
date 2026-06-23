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

## Implement CPS3 Behavior

Treat every comparison request as authorization to update the requested local symbols so they can reproduce CPS3 behavior. Do not stop after identifying a difference when a safe implementation is possible. Skip edits only when the user explicitly requests comparison without modification, the local symbol is version-exclusive, or unresolved evidence makes a faithful change unsafe.

For each symbol that exists in both versions and differs, implement the smallest faithful CPS3 path. Keep version-specific behavior gated by `ArcadeBalance_IsEnabled()` from `src/arcade/arcade_balance.h`:

```c
#include "arcade/arcade_balance.h"

if (ArcadeBalance_IsEnabled()) {
    /* CPS3 behavior */
} else {
    /* existing behavior */
}
```

Preserve existing behavior when the flag is disabled. If CPS3 omits behavior that the local version performs, guard the local-only behavior with `!ArcadeBalance_IsEnabled()`. Prefer a narrow conditional around the actual difference instead of duplicating an entire function.

Assign the parity emoji after making the change, based on the function's final contents. Retain 🟡 when the function contains an arcade-balance branch or any other intentional version-dependent behavior; its guarded contents must visibly explain the difference. Use 🟢 only when the final function matches CPS3 without qualification. Never leave a 🟡 marker on a safely fixable function without also representing the CPS3 behavior in its contents.

Verify edited code with the narrowest relevant formatter, build, or test available. Summarize matched functions, differences, version-exclusive functions, edits, and unresolved symbol-labeling needs.

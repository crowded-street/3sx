# Red Band Backlog

The nineteen files that scored below 4.0 in the 2026-09-01 sweep, ranked worst first at
that time. **None of them is still there.**

See [`README.md`](README.md) for the campaign rules and [`PLAYBOOK.md`](PLAYBOOK.md)
for the allowed transformations.

> [!IMPORTANT]
> **Scores here are refreshed from a full sweep; the task files are not.**
> If a task file's stated baseline does not match what you measure, the task file is
> stale - check this page before stopping and reporting a mismatch. The current numbers
> below come from `codehealth-current.json`, swept 2026-09-19.
> `codehealth-baseline.json` preserves the original 2026-09-01 sweep and is not updated.

## Where the whole repository stands

| Band | Score | 2026-09-01 | 2026-09-19 |
| --- | --- | --- | --- |
| **Red** - severe debt | 1.0 - 3.9 | 19 | **0** |
| **Yellow** - problematic debt | 4.0 - 8.9 | 207 | 124 |
| Green | 9.0 - 9.9 | 158 | 84 |
| Optimal | 10.0 | 98 | **444** |
| Total scored | | 482 | 652 |

The file count rises because the campaign splits files. Mean Code Health across every
scorable first-party file is **9.51**.

## The Red band is empty

As of 2026-09-19 **no first-party file scores below 4.0**. The last two out were
`Game/com/ck_pass.c` (3.75 -> 8.03, split six ways) and `Game/opening/opening.c`
(3.97 -> 9.68, split four ways).

Nothing scores below 5.0 either. The worst files were swept on 2026-09-19:

| File | Before | After |
| --- | --- | --- |
| `Game/com/com_pl.c` | 4.77 | **8.88**, split three ways; group mean 8.47 |
| `Game/game.c` | 4.79 | **9.04**, split four ways; group mean 8.35 |
| `AcrSDK/ps2/flps2etc.c` | 5.17 | **6.94** |
| `AcrSDK/ps2/flps2vram.c` | 4.82 | **5.25** |

The lowest in the repository is now `AcrSDK/ps2/flps2vram.c` at 5.25, and what holds it
there is one function: `flPS2LockTexture` at **367 lines and cc 29**, an outer switch on
the lock mode with a format switch inside each of its four arms. Splitting it is the next
piece of work anyone picks up. After that: `platform/netplay/game_state.c` (6.30),
`Game/opening/opening_scenes_late.c` (6.77) and `platform/video/sdl_gpu/sdl_gpu_renderer.c`
(6.82).

## Track A - available now

Outside the in-round simulation. Safe to refactor with compile verification only.

In both tables the **Score** column is the 2026-09-19 sweep and a struck-through task is
at 10.00. **LOC, Churn and the last column are as of 2026-09-01** and are not refreshed -
where a file has been split, its LOC is now spread over several files, and the named worst
function is usually long gone.

| Task | File | Score | LOC | Churn | Risk | Worst function |
| --- | --- | --- | --- | --- | --- | --- |
| ~~[R04](tasks/R04-menu.md)~~ | `Game/menu/menu.c` | **10.00** | 5374 | 58 | LOW | `Extra_Option` (cc 28) |
| ~~[R06](tasks/R06-mtrans.md)~~ | `Game/rendering/mtrans.c` | **7.55** | 2224 | 10 | MEDIUM | plateau 2026-09-18 - split into `mtrans_seqs.c` (10.00) and `mtrans_pool.c` (9.38) |
| ~~[R10](tasks/R10-opening.md)~~ | `Game/opening/opening.c` | **9.68** | 2997 | 17 | LOW | `TITLE_Move` (cc 11) |
| ~~[R11](tasks/R11-PPGFile.md)~~ | `Common/PPGFile.c` | **8.57** | 1511 | 11 | MEDIUM | `ppgRenewDotDataSeqs` (cc -) |
| ~~[R14](tasks/R14-bg.md)~~ | `Game/stage/bg.c` | **9.09** | 1430 | 11 | MEDIUM | plateau 2026-09-18 - split into `bg_textures.c` (10.00); the whole stage folder went with it, see the task report |
| ~~[R17](tasks/R17-entry.md)~~ | `Game/screen/entry.c` | **10.00** | 1480 | 19 | LOW | done 2026-09-18 - left the Red band; split into `entry_break_in.c` |

## Track B - manual playtesting required (statcheck indefinitely unavailable)

Core engine, CPU logic, animation and effects. A silent behaviour change here breaks
gameplay or rollback determinism and the build will not catch it. Statcheck would
normally catch it, but its replay runner needs a real CPS3 ROM dump, and a legitimate
one requires owning genuine CPS3 arcade hardware - this project's no-piracy policy won't
work around that, so treat statcheck as indefinitely unavailable rather than pending.
**2026-09-04 (project owner directive):** these files may be refactored now; verify with
manual playtesting instead of waiting on statcheck.

| Task | File | Score | LOC | Churn | Risk | Worst function |
| --- | --- | --- | --- | --- | --- | --- |
| ~~[R01](tasks/R01-hitcheck.md)~~ | `Game/engine/hitcheck.c` | **10.00** | 2308 | 17 | CRITICAL | `attack_hit_check` (cc 67) |
| ~~[R02](tasks/R02-com_sub.md)~~ | `Game/com/com_sub.c` | **10.00** | 5799 | 20 | CRITICAL | `Reaction_Sub` (cc 46) |
| [R03](tasks/R03-pls03.md) | `Game/engine/pls03.c` | **8.92** | 1794 | 10 | CRITICAL | `check_special_attack` (cc 99) |
| [R05](tasks/R05-cmd_main.md) | `Game/engine/cmd_main.c` | **9.39** | 1876 | 12 | CRITICAL | `check_10` (cc 48) |
| [R07](tasks/R07-eff09.md) | `Game/effect/eff09.c` | **8.54** | 1897 | 12 | HIGH | `eff09_24000` (cc 22) |
| ~~[R08](tasks/R08-eff13.md)~~ | `Game/effect/eff13.c` | **10.00** | 2155 | 7 | HIGH | `kotp_07000` (cc 26) |
| ~~[R09](tasks/R09-pls00.md)~~ | `Game/engine/pls00.c` | **10.00** | 2128 | 3 | CRITICAL | `jumping_cg_type_check` (cc 92) |
| [R12](tasks/R12-appear.md) | `Game/animation/appear.c` | **9.38** | 2040 | 13 | HIGH | `Appear_29000` (cc 23) |
| ~~[R13](tasks/R13-win_pl.md)~~ | `Game/animation/win_pl.c` | **10.00** | 1629 | 18 | HIGH | `Win_10000` (cc 19) |
| ~~[R15](tasks/R15-plpdm.md)~~ | `Game/engine/plpdm.c` | **10.00** | 1699 | 17 | CRITICAL | `subtract_dm_vital` (cc 30) |
| ~~[R16](tasks/R16-ck_pass.md)~~ | `Game/com/ck_pass.c` | **8.03** | 3126 | 5 | CRITICAL | `KEN_vs` (cc 41) |
| [R18](tasks/R18-charset.md) | `Game/engine/charset.c` | **9.68** | 2880 | 14 | CRITICAL | `check_cgd_patdat` (cc 43) |
| [R19](tasks/R19-manage.md) | `Game/engine/manage.c` | **9.92** | 2518 | 30 | CRITICAL | `Game_Manage_12_4` (cc 32) |

## Aggregate

At the 2026-09-01 start: **19 files**, 46,865 lines of code, **330** complex methods,
**267** bumpy roads and **95** deeply-nested functions. Highest churn was `menu/menu.c`
(58 commits), which started at 1.97; the worst single function was `scr_trans` in
`stage/bg.c` (cyclomatic 109, nesting 9).

At 2026-09-19: **two** of the nineteen are still below 4.0, eight are at 10.00, and
`scr_trans`'s file is at 9.09. Mean Code Health across the repository is 9.51.

## Folder campaigns

Work that is not a numbered task, because none of these files were in the Red band.
Recorded here so a later agent can see what has already been swept.

| Folder | Files | Before | After | Notes |
| --- | --- | --- | --- | --- |
| `Game/ui` | 9 | 4.06 - 9.92 | 8.47 - 10.00 | `sc_sub.c` split four ways; *Recipe A clears one finding, not fifteen* |
| `Game/com/passive` | 20 -> 77 | 4.90 - 7.55 | mean **9.03**, 30 at 10.00 | 3488 CPU pattern scripts folded onto shared skeletons; see *Where `Game/com/passive` stopped* in `PLAYBOOK.md` |
| `Game/com/active` | 20 -> 27 | 5.04 - 8.03 | mean **8.64**, 4 at 10.00 | 1621 scripts, the same shape under another name; folded folder-wide first, which is the order the passive folder's notes recommend |

Both COM script folders are now done. `tools/passive_fold.py --family {passive,active}`
carries the transformations and the equivalence check for either.

## Progress

Update as tasks land.

The **Baseline** column is refreshed whenever these files are regenerated, so for a task
already under way it shows the score at last regeneration, not the campaign's starting
point. **Campaign start** preserves the original 2026-09-01 sweep.

| Task | Campaign start | Baseline | Current | Status |
| --- | --- | --- | --- | --- |
| R01 | 1.42 | 1.42 | **10.00** | Done. Split into `hitcheck_collision.c`, `hitcheck_damage.c` and `hitcheck_defense.c` (Recipe S), all four files at 10.00. The `goto` cores that were left untouched in the first wave moved out whole rather than being rewritten. |
| R02 | 1.52 | 1.52 | **10.00** | Done. The `com_sub` family - `com_sub_attack.c`, `com_sub_command.c`, `com_sub_jump.c`, `com_sub_arts.c` and the rest - carries the parameter-object structs the passive and active scripts pass at every call site. |
| R03 | 1.82 | 1.82 | **8.92** | Plateau. Split into `pls03_super_arts.c`, now **9.92**. `decode_wst_data`'s twelve command encodings and `waza_select`'s eleven case labels are what stop the original; neither loses a branch without renumbering a state. The super-arts file stops on its direct-cancel side, which mirrors grounded against airborne at every level. |
| R04 | **1.97** | 2.25 | **10.00** | Done. Split into `extra_option.c`, `direction_menu.c`, `menu_selection.c`, `blocking_training.c` and more; `netplay_menu.c` is the lowest of the family at 8.59. |
| R05 | 2.31 | 2.31 | **9.39** | Plateau. Split into `cmd_main_checks.c`, now **7.50** and the hardest file left in the folder: its mean needs thirteen more functions against a duplicate web that charges for every one. `cmd_main.c` itself stops on the two `latch_sw_lvbt_bit_*` functions, whose cc 11 is almost entirely `case` labels. |
| R06 | 2.57 | 5.24 | **7.55** | Plateau. Split into `mtrans_seqs.c` (**10.00**) and `mtrans_pool.c` (9.38). Recipe A cleared all seventeen Excess Argument findings, a new **Recipe W** collapsed eighteen copies of the chip-queue call, and Recipe F merged the three extended dispatchers into one body. What stops it is Lines of Code at 1205 with no legal cut left - every seam runs through a shared `static` - and a 16/32 near-miss web the catalogue cannot merge |
| R07 | 2.58 | 2.58 | **8.54** | In progress. Code Duplication is the only finding left. |
| R08 | 2.64 | 2.64 | **10.00** | Done. |
| R09 | 2.74 | 2.74 | **10.00** | Done. Split into `pls00_normal_states.c` (now **8.03**) and `pls00_damage_states.c` (10.00); the original is at 10.00. The normal-states file plateaus on a Code Duplication web between the `nm_*` state machines themselves, which no shared run reaches. |
| R10 | 3.09 | 3.09 | **9.68** | Plateau. Split four ways into `opening_scenes.c` (7.42), `opening_bg0.c` (7.54) and `opening_bg1.c` (8.28), then the scenes again into `opening_scenes_late.c` (6.77). What stops the scene files is that the nineteen `op_1NN_move` state machines are near-copies of one another, so chaining one pays and chaining the next costs - `op_103_move` measured 6.91 -> 7.42 and `op_107_move` 7.42 -> 6.50. |
| R11 | 3.49 | 3.49 | **8.57** | In progress. Split four ways into `PPGFile_quads.c` (7.86), `PPGFile_chunks.c` (6.99) and `PPGFile_context.c` (9.44); Recipe A cleared ten Excess Argument findings and Brain Method with them. `PPGFile_chunks.c` still carries seven Bumpy Roads and is where the remaining work is. |
| R12 | 3.56 | 3.56 | **9.38** | Plateau. Split into `appear_late.c`, now also 9.38. Overall Code Complexity is the only finding left in either. |
| R13 | 3.56 | 3.56 | **10.00** | Done. |
| R14 | 3.62 | 3.62 | **9.09** | Plateau. Split into `bg_textures.c` (10.00); the whole stage folder went with it. |
| R15 | 3.68 | 3.68 | **10.00** | Done. `plpdm.c`, `plpdm_states.c` and `plpdm_states_late.c` are all at 10.00. The last two were the campaign's most stubborn plateau until the runs the damage states share were taken instead of their arms - see *Against a twin family, share what they agree on* in `PLAYBOOK.md`. |
| R16 | 3.75 | 3.75 | **8.03** | Plateau. Split six ways: `ck_pass_checks.c` (**10.00**), `ck_pass_units_a.c` (**10.00**), `_b.c` (9.38), `_c.c` (**10.00**), `_d.c` (**10.00**). Group mean 9.57. What stops `ck_pass.c` is Code Duplication between `KEN_vs`, `HUGO_vs` and `GILL_vs` and their area helpers: the three are near-misses, not duplicates - Hugo discards the jump check's answer and has no personal action, Gill adds a squat check - so merging any two is Recipe D's forbidden case. |
| R17 | 3.86 | 3.86 | **10.00** | Done. Split into `entry_break_in.c`. |
| R18 | 3.92 | 3.92 | **9.68** | Plateau. Split twice, into `charset_commands.c` (10.00) and `charset_position.c` (10.00). What stops the original is `set_char_move_init2`'s five arguments. Recipe A would clear it, and the obstacle is scope rather than any one file: **155** call sites, 111 of them in the effect folder and 13 in animation. |
| R19 | 3.92 | 3.92 | **9.92** | Plateau. Split into `manage_bonus.c` (10.00) and `manage_result.c` (9.38). `Game_Manage_7_3`'s dead condition is the last finding and the catalogue forbids deleting it. |

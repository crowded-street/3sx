# Red Band Backlog

All 19 files scoring below 4.0 (severe technical debt), ranked worst first.
Generated from a CodeScene sweep of 482 scorable first-party files.

See [`README.md`](README.md) for the campaign rules and [`PLAYBOOK.md`](PLAYBOOK.md)
for the allowed transformations.

## Track A - available now

Outside the in-round simulation. Safe to refactor with compile verification only.

| Task | File | Score | LOC | Churn | Risk | Worst function |
| --- | --- | --- | --- | --- | --- | --- |
| [R04](tasks/R04-menu.md) | `Game/menu/menu.c` | **2.25** | 5374 | 58 | LOW | `Extra_Option` (cc 28) |
| [R06](tasks/R06-mtrans.md) | `Game/rendering/mtrans.c` | **2.57** | 2224 | 10 | MEDIUM | `mlt_obj_trans_rgb_ext` (cc 22) |
| [R10](tasks/R10-opening.md) | `Game/opening/opening.c` | **3.09** | 2997 | 17 | LOW | `TITLE_Move` (cc 11) |
| [R11](tasks/R11-PPGFile.md) | `Common/PPGFile.c` | **3.49** | 1511 | 11 | MEDIUM | `ppgRenewDotDataSeqs` (cc -) |
| [R14](tasks/R14-bg.md) | `Game/stage/bg.c` | **3.62** | 1430 | 11 | MEDIUM | `scr_trans` (cc 109) |
| [R17](tasks/R17-entry.md) | `Game/screen/entry.c` | **3.86** | 1480 | 19 | LOW | `Entry_Main_Sub` (cc 35) |

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
| [R01](tasks/R01-hitcheck.md) | `Game/engine/hitcheck.c` | **1.42** | 2308 | 17 | CRITICAL | `attack_hit_check` (cc 67) |
| [R02](tasks/R02-com_sub.md) | `Game/com/com_sub.c` | **1.52** | 5799 | 20 | CRITICAL | `Reaction_Sub` (cc 46) |
| [R03](tasks/R03-pls03.md) | `Game/engine/pls03.c` | **1.82** | 1794 | 10 | CRITICAL | `check_special_attack` (cc 99) |
| [R05](tasks/R05-cmd_main.md) | `Game/engine/cmd_main.c` | **2.31** | 1876 | 12 | CRITICAL | `check_10` (cc 48) |
| [R07](tasks/R07-eff09.md) | `Game/effect/eff09.c` | **2.58** | 1897 | 12 | HIGH | `eff09_24000` (cc 22) |
| [R08](tasks/R08-eff13.md) | `Game/effect/eff13.c` | **2.64** | 2155 | 7 | HIGH | `kotp_07000` (cc 26) |
| [R09](tasks/R09-pls00.md) | `Game/engine/pls00.c` | **2.74** | 2128 | 3 | CRITICAL | `jumping_cg_type_check` (cc 92) |
| [R12](tasks/R12-appear.md) | `Game/animation/appear.c` | **3.56** | 2040 | 13 | HIGH | `Appear_29000` (cc 23) |
| [R13](tasks/R13-win_pl.md) | `Game/animation/win_pl.c` | **3.56** | 1629 | 18 | HIGH | `Win_10000` (cc 19) |
| [R15](tasks/R15-plpdm.md) | `Game/engine/plpdm.c` | **3.68** | 1699 | 17 | CRITICAL | `subtract_dm_vital` (cc 30) |
| [R16](tasks/R16-ck_pass.md) | `Game/com/ck_pass.c` | **3.75** | 3126 | 5 | CRITICAL | `KEN_vs` (cc 41) |
| [R18](tasks/R18-charset.md) | `Game/engine/charset.c` | **3.92** | 2880 | 14 | CRITICAL | `check_cgd_patdat` (cc 43) |
| [R19](tasks/R19-manage.md) | `Game/engine/manage.c` | **3.92** | 2518 | 30 | CRITICAL | `Game_Manage_12_4` (cc 32) |

## Aggregate

- **19 files**, 46,865 lines of code
- **330** complex methods, **267** bumpy roads, **95** deeply-nested functions
- Highest churn: `menu/menu.c` (58 commits), which started the campaign at 1.97
- Worst single function: `scr_trans` in `stage/bg.c` (cyclomatic 109, nesting 9)

## Progress

Update as tasks land.

The **Baseline** column is refreshed whenever these files are regenerated, so for a task
already under way it shows the score at last regeneration, not the campaign's starting
point. **Campaign start** preserves the original 2026-09-01 sweep.

| Task | Campaign start | Baseline | Current | Status |
| --- | --- | --- | --- | --- |
| R01 | 1.42 | 1.42 | **10.00** | Done. Split into `hitcheck_collision.c`, `hitcheck_damage.c` and `hitcheck_defense.c` (Recipe S), all four files at 10.00. The `goto` cores that were left untouched in the first wave moved out whole rather than being rewritten. |
| R02 | 1.52 | 1.52 | - | not started |
| R03 | 1.82 | 1.82 | **8.92** | Plateau. Split into `pls03_super_arts.c`, now **9.92**. `decode_wst_data`'s twelve command encodings and `waza_select`'s eleven case labels are what stop the original; neither loses a branch without renumbering a state. The super-arts file stops on its direct-cancel side, which mirrors grounded against airborne at every level. |
| R04 | **1.97** | 2.25 | **2.25** | wave 1 of 3 done (PR #3) |
| R05 | 2.31 | 2.31 | **9.39** | Plateau. Split into `cmd_main_checks.c`, now **7.50** and the hardest file left in the folder: its mean needs thirteen more functions against a duplicate web that charges for every one. `cmd_main.c` itself stops on the two `latch_sw_lvbt_bit_*` functions, whose cc 11 is almost entirely `case` labels. |
| R06 | 2.57 | 2.57 | - | not started |
| R07 | 2.58 | 2.58 | - | not started |
| R08 | 2.64 | 2.64 | - | not started |
| R09 | 2.74 | 2.74 | **10.00** | Done. Split into `pls00_normal_states.c` (now **8.03**) and `pls00_damage_states.c` (10.00); the original is at 10.00. The normal-states file plateaus on a Code Duplication web between the `nm_*` state machines themselves, which no shared run reaches. |
| R10 | 3.09 | 3.09 | - | not started |
| R11 | 3.49 | 3.49 | - | not started |
| R12 | 3.56 | 3.56 | - | not started |
| R13 | 3.56 | 3.56 | - | not started |
| R14 | 3.62 | 3.62 | - | not started |
| R15 | 3.68 | 3.68 | **10.00** | Done. `plpdm.c`, `plpdm_states.c` and `plpdm_states_late.c` are all at 10.00. The last two were the campaign's most stubborn plateau until the runs the damage states share were taken instead of their arms - see *Against a twin family, share what they agree on* in `PLAYBOOK.md`. |
| R16 | 3.75 | 3.75 | - | not started |
| R17 | 3.86 | 3.86 | - | not started |
| R18 | 3.92 | 3.92 | **9.68** | Plateau. Split twice, into `charset_commands.c` (10.00) and `charset_position.c` (10.00). What stops the original is `set_char_move_init2`'s five arguments. Recipe A would clear it, and the obstacle is scope rather than any one file: **155** call sites, 111 of them in the effect folder and 13 in animation. |
| R19 | 3.92 | 3.92 | **9.92** | Plateau. Split into `manage_bonus.c` (10.00) and `manage_result.c` (9.38). `Game_Manage_7_3`'s dead condition is the last finding and the catalogue forbids deleting it. |

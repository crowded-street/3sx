# Red Band Backlog

All 19 files scoring below 4.0 (severe technical debt), ranked worst first.
Generated from a CodeScene sweep of 482 scorable first-party files.

See [`README.md`](README.md) for the campaign rules and [`PLAYBOOK.md`](PLAYBOOK.md)
for the allowed transformations.

## Track A - available now

Outside the in-round simulation. Safe to refactor with compile verification only.

| Task | File | Score | LOC | Churn | Risk | Worst function |
| --- | --- | --- | --- | --- | --- | --- |
| [R04](tasks/R04-menu.md) | `Game/menu/menu.c` | **1.97** | 5365 | 58 | LOW | `Direction_Menu` (cc 27) |
| [R06](tasks/R06-mtrans.md) | `Game/rendering/mtrans.c` | **2.57** | 2224 | 10 | MEDIUM | `mlt_obj_trans_rgb_ext` (cc 22) |
| [R10](tasks/R10-opening.md) | `Game/opening/opening.c` | **3.09** | 2997 | 17 | LOW | `TITLE_Move` (cc 11) |
| [R11](tasks/R11-PPGFile.md) | `Common/PPGFile.c` | **3.49** | 1511 | 11 | MEDIUM | `ppgRenewDotDataSeqs` (cc -) |
| [R14](tasks/R14-bg.md) | `Game/stage/bg.c` | **3.62** | 1430 | 11 | MEDIUM | `scr_trans` (cc 109) |
| [R17](tasks/R17-entry.md) | `Game/screen/entry.c` | **3.86** | 1480 | 19 | LOW | `Entry_Main_Sub` (cc 35) |

## Track B - blocked until statcheck is green in CI

Core engine, CPU logic, animation and effects. A silent behaviour change here breaks
gameplay or rollback determinism and the build will not catch it.

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

- **19 files**, 46,856 lines of code
- **330** complex methods, **267** bumpy roads, **95** deeply-nested functions
- Highest churn: `menu/menu.c` (58 commits) - and it scores 1.97
- Worst single function: `scr_trans` in `stage/bg.c` (cyclomatic 109, nesting 9)

## Progress

Update as tasks land.

| Task | Baseline | Current | Status |
| --- | --- | --- | --- |
| R01 | 1.42 | - | not started |
| R02 | 1.52 | - | not started |
| R03 | 1.82 | - | not started |
| R04 | 1.97 | - | not started |
| R05 | 2.31 | - | not started |
| R06 | 2.57 | - | not started |
| R07 | 2.58 | - | not started |
| R08 | 2.64 | - | not started |
| R09 | 2.74 | - | not started |
| R10 | 3.09 | - | not started |
| R11 | 3.49 | - | not started |
| R12 | 3.56 | - | not started |
| R13 | 3.56 | - | not started |
| R14 | 3.62 | - | not started |
| R15 | 3.68 | - | not started |
| R16 | 3.75 | - | not started |
| R17 | 3.86 | - | not started |
| R18 | 3.92 | - | not started |
| R19 | 3.92 | - | not started |

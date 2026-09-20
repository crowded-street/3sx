# Red Band Backlog

The nineteen files that scored below 4.0 in the 2026-09-01 sweep, ranked worst first at
that time. **None of them is still there.**

See [`README.md`](README.md) for the campaign rules and [`PLAYBOOK.md`](PLAYBOOK.md)
for the allowed transformations.

> [!IMPORTANT]
> **Scores here are refreshed from a full sweep; the task files are not.**
> If a task file's stated baseline does not match what you measure, the task file is
> stale - check this page before stopping and reporting a mismatch. The current numbers
> below come from `codehealth-current.json`, swept 2026-09-19 (seventh sweep of that day,
> after the PPGFile family, the `Game/opening` folder, `mtrans.c`'s split and
> `flps2vram.c`).
> `codehealth-baseline.json` preserves the original 2026-09-01 sweep and is not updated.

## Where the whole repository stands

| Band | Score | 2026-09-01 | 2026-09-19 |
| --- | --- | --- | --- |
| **Red** - severe debt | 1.0 - 3.9 | 19 | **0** |
| **Yellow** - problematic debt | 4.0 - 8.9 | 207 | 93 |
| Green | 9.0 - 9.9 | 158 | 91 |
| Optimal | 10.0 | 98 | **475** |
| Total scored | | 482 | 659 |

The file count rises because the campaign splits files. Mean Code Health across every
scorable first-party file is **9.64**.

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

`AcrSDK/ps2/flps2vram.c` was the lowest at 5.25 and is now **7.36**: the format switch
three lock modes shared was written once, the lock and unlock conversions moved out of
their loops and chained, and the pixel loop gave up its alpha fix-up and its two width
switches. What is left there is the texture/palette twin pairs - `flPS2GetTextureHandle`
against `flPS2GetPaletteHandle` and two more like them - which differ in their type and
so are Recipe D's forbidden case.

The lowest in the repository is now `platform/netplay/game_state.c` (6.30), then
`platform/video/sdl_gpu/sdl_gpu_renderer.c` (6.82), `AcrSDK/ps2/flps2etc.c` (6.94) and
`AcrSDK/ps2/ps2PAD.c` (7.01).

## Track A - available now

Outside the in-round simulation. Safe to refactor with compile verification only.

In both tables the **Score** column is the 2026-09-19 sweep and a struck-through task is
at 10.00. **LOC, Churn and the last column are as of 2026-09-01** and are not refreshed -
where a file has been split, its LOC is now spread over several files, and the named worst
function is usually long gone.

| Task | File | Score | LOC | Churn | Risk | Worst function |
| --- | --- | --- | --- | --- | --- | --- |
| ~~[R04](tasks/R04-menu.md)~~ | `Game/menu/menu.c` | **10.00** | 5374 | 58 | LOW | `Extra_Option` (cc 28) |
| ~~[R06](tasks/R06-mtrans.md)~~ | `Game/rendering/mtrans.c` | **8.03** | 2224 | 10 | MEDIUM | split again 2026-09-19 into `mtrans_buffers.c` (8.28); the file's size finding is cleared |
| ~~[R10](tasks/R10-opening.md)~~ | `Game/opening/opening.c` | **10.00** | 2997 | 17 | LOW | none - `TITLE_Move`'s sequence moved out 2026-09-19 |
| ~~[R11](tasks/R11-PPGFile.md)~~ | `Common/PPGFile.c` | **9.31** | 1511 | 11 | MEDIUM | the palette/texture twin pair; every other file in the family is at 10.00 |
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

At 2026-09-19: **none** of the nineteen is still below 4.0, eight are at 10.00, and
`scr_trans`'s file is at 9.09. Mean Code Health across the repository is 9.64.

## The yellow band, file by file - re-audited 2026-09-20 evening

The audit earlier the same day found forty-six files below 9.0 and called most of them
refusals. **Nine of those rows were wrong**, and working through them took the band from
forty-six files to twenty-six and the repository mean from 9.80 to 9.87. They are listed
here with what actually unblocked them, because the pattern in the mistakes is more useful
than the individual entries.

| File | Then | Now | What the earlier row missed |
| --- | --- | --- | --- |
| `mtrans.c` | 8.03 | **10.00** | "No further legal cut exists", on a call-graph pass showing every seam running through a `static`. The seam was real; the Recipe S exception of 2026-09-18 covers it, because every static on that seam is one this campaign created. Split to `mtrans_tiles.c`, which then folded to 10.00 as well |
| `Lz77Dec.c` | 8.81 | **10.00** | "Every block advances `src`, `dst` and `size` at once." Two of the three do cross as pointers to the caller's pointers; the third does not cross at all, because every arm ended in the same `size -= loop` and the helper can return `loop` instead |
| `memmgr.c` | 8.88 | **10.00** | The twins are real and cutting both does cost more than it gives - 9.84 against 9.31, measured again. What the row missed is that cutting *one* pays, twice over, and that the block-list scan itself folds on a comparison predicate |
| `pls03.c` | 8.92 | **10.00** | Recipe X, four ways on the wst dispatch and twice on the table selector. Nothing blocked it; it had not been tried |
| `keymap.c` | 8.92 | **10.00** | "Every legal split reproduces its own shape as a duplicate - measured at 8.92 -> 8.54." True of every *split*. The switch is a name table, and writing it as one is Recipe L, added to the playbook this session |
| `caldir.c` | 8.81 | 9.68 | The x/y twins do differ in five field names each, and C has no pointer-to-member - but the fields can travel as pointers into the caller's own struct. The argument finding on the two six-parameter entry points stands, at 62 call sites across 30 files |
| `flps2vram.c` | 8.54 | **10.00** | The 16/32 pointer-type twin is one group of three. The other three groups - handle allocation, unlock, direct-colour conversion - differ only in values and calls |
| `end_05.c` | 8.81 | **10.00** | The `Request_Fade(1) != 0` against `Request_Fade(3)` difference is real and does disqualify Recipe V. It does not disqualify Recipe F: each scene keeps its own expression in its own one-line function |
| `end_14.c` | 8.28 | 9.38 | Two pairs that differ in a statement rather than a value, which is Recipe F's case and not Recipe D's |

Also cleared from the band this session: `bg_sub.c` 9.09 -> 10.00 (the mirrored scroll pair
turned out to differ only in subscripts, which are literals - correcting a refusal recorded
on the strength of a buggy ad-hoc extractor), `ck_pass_units_b.c` 9.38 -> 10.00,
`opening_bg1.c` 8.28 -> 9.38, `mtrans_buffers.c` and `ck_pass.c` 8.28 -> 8.54,
`eff09.c` 8.54 -> 8.81, `active17.c` and `pass08_4.c` 8.81 -> 9.09.

**The lesson in the mistakes.** Every wrong row was wrong in one of two ways. Either it
recorded a plateau from before a rule changed - `mtrans.c` and the Recipe S exception -
or it generalised from one failed shape to the whole file: "the twins cannot fold" when
three other groups in the same file could, "every split reproduces its shape" when the
answer was not a split. A refusal is only as wide as the thing that was actually tried,
and it expires when the catalogue grows.

### What is left, and why it is structural

Twenty-six files remain below 9.0. Twenty-five of them are the CPU script family, and they
stop for one reason, which is now measured rather than argued.

**Every one-step skeleton is the same ten lines.** `com_patterns_1step.c` holds 44
functions; CodeScene puts **39 of them in a single mutual-duplication clique**. The shape
is always

```c
void pattern_x(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
```

and the members differ in exactly two places: the `case` label, and the engine call. The
label is a literal and cannot be parameterised. The calls do not share a signature, so no
one function pointer reaches them. Folding any subset produces a helper that is itself a
member of the remaining clique - the "which cut makes the twin" rule, at the scale of a
whole file.

The same count holds across the family: `com_patterns_2step.c` 54 of 63,
`com_patterns_4step.c` 38 of 41, `com_patterns_6step.c` 19 of 24, `active08.c` 19,
`pass14.c` 18, `pass09_2.c` 15. These are not several files with duplication; they are one
shape written several hundred times, which is what a pattern script *is*.

The skeleton is exactly ten lines of code, and CodeScene's duplication check has a
ten-line floor. Nine lines would drop the whole family out of the finding, and
`AllowShortCaseLabelsOnASingleLine` would get there. That is gaming the measurement rather
than improving the code, and it is not done.

The individual folds that *are* legal in this family were taken: `com_patterns_3step.c`,
`active07.c`, `active14_2.c`, `active17.c`, `pass08_4.c`, `pass14_4.c` and
`pls00_normal_states.c` all lost their qualifying groups this session, mostly through
Recipe A with named-member parameter objects. They moved the duplication counts and, where
the clique still dominates, not the score - kept under rule 2's complexity clause.

**Overall Code Complexity on the four-, five- and six-step files** has the same shape of
answer. A skeleton's cyclomatic complexity is its arm count plus one, so the file mean is
pinned at roughly the step count. Recipe X would divide it, and `bg_zoom.c` already priced
that: one split pays, and the full set of six measures 8.54 -> 8.28, because the split
halves are themselves twins.

The twenty-sixth file is `opening_bg0.c` at 8.12, and it is a genuine measured refusal:
lifting `op_bg0_0003`'s inner block clears one Complex Method and creates a duplicate pair
with the existing `op_bg0_lay_blocks` chain, 8.12 -> 8.03. Reverted.

## The platform, SDK and shim sweep - 2026-09-20

A pass over everything that is not a numbered task and not a character script: the
platform layer, the AcrSDK, the port shims, the test harness and the scene flow. Twenty
files reached **10.00** and none of the campaign's rules were relaxed except Recipe V's
instance count, which was settled rather than relaxed - see the playbook.

| File | Before | After | What did it |
| --- | --- | --- | --- |
| `platform/video/opengl/opengl_renderer.c` | 8.87 | **10.00** | the vertex fill, the shader bind, the canvas and the buffer setup lifted out; the four shader-failure teardowns are one run |
| `AcrSDK/common/mlPAD.c` | 8.87 | **10.00** | three loops out of `flPADACRConf`, then the mapped slot's depth out of the loop that was left |
| `port/sound/adx_decoder.c` | 8.89 | **10.00** | the header split four ways; the block scales and the sample nibble out of the decode loop |
| `Game/game_demo.c` | 8.89 | **10.00** | the attract sequence's stages split twice |
| `Game/game.c` | 9.04 | **10.00** | character select's entry and exit, then the mode settings and the round reset |
| `platform/netplay/netplay.c` | 9.00 | **10.00** | the local inputs, one session event, one advance event, the transition and the teardown |
| `Source/Common/PPGWork.c` | 9.00 | **10.00** | twenty-three copies of "release it if it is loaded" became two helpers |
| `Game/io/gd3rd.c` | 9.02 | **10.00** | the queue pop and the two halves of the step; `load_it_use_any_key2` took a parameter object |
| `test/replay_game.c` | 9.02 | **10.00** | the match setup the start frame carries, and the frame test named |
| `Game/com/com_pl.c` | 8.88 | **10.00** | the shell guard's last frame, the throw wait, and the two sides of two mirrored checks |
| `Game/com/com_pl_damage.c` | 8.13 | **10.00** | all eight damage states cut down; the run two of them share taken twice |
| `Game/com/com_pl_flip.c` | 8.41 | **10.00** | the guard-lever pair folded, then `Check_Shell_Flip` taken apart four ways |
| `Game/system/sys_sub_replay.c` | 9.31 | **10.00** | three blocks out of `Replay` |
| `Game/animation/win_pl_q.c` | 9.31 | **10.00** | the distance test both facings share, then the facing-right arm |
| `port/config/config_helpers.c` | 9.63 | **10.00** | one configuration line parsed in its own function |
| `Game/ui/sc_sub_training.c` | 9.68 | **10.00** | the three draw rows |
| `AcrSDK/common/plpic.c` | 9.68 | **10.00** | the PIC header's four checks |
| `platform/app/sdl/sdl_app.c` | 9.09 | **10.00** | two conditions named, then the event dispatch split |
| `port/sdl/netplay_screen.c` | 9.47 | **10.00** | the status text, the hold, and the switch split |
| `Game/io/ioconv.c` | 9.92 | **10.00** | the turbo repeat test |
| `Game/engine/plpat09.c` | 9.92 | **10.00** | the rock loop flattened with `continue` |
| `Game/effect/effd1.c`, `effe9.c` | 9.92 | **10.00** | one arm of a mirrored pair each - the *other* arm makes a twin and costs half a point |
| `test/test_runner_compare.c` | 9.92 | **10.00** | one player's frame comparison |
| `Game/ending/end_18.c` | 9.38 | **10.00** | the two scene steps, under Recipe V's new two-instance licence |
| `arcade/cps3_decrypt.c`, `AcrSDK/.../emlRefPhd.c`, `emlSndDrv.c`, `AcrSDK/ps2/flps2render.c` | 9.68 | **10.00** | a parameter object each, one or two call sites apiece |
| `platform/netplay/sdl_net_adapter.c` | 9.68 | **10.00** | the receive loop's condition named |
| `core/renderer.c` | 8.28 | **9.38** | eleven dispatch entry points folded by signature group |
| `Game/system/sys_sub.c` | 8.66 | **9.31** | the eight button mappings; `Meltw`'s run; the hidden extra options |
| `Game/ui/sc_sub_transition.c` | 8.67 | **9.31** | the full-screen quad four transitions fill; the run two of them share; the wipe-out's directions |
| `Game/io/pulpul.c` | 8.73 | **9.68** | the pattern row walk, its two row kinds, and the row start |
| `platform/netplay/fistbump.c` | 8.79 | **9.68** | the two connect waits, the state switch and the command chain both split |
| `Game/menu/netplay_menu.c` | 8.59 | **9.41** | the input frame, the exit condition and the confirm path |
| `port/sound/emlShim.c` | 8.94 | **9.50** | one condition bit, then the switch split |
| `AcrSDK/MiddleWare/PS2/CapSndEng/emlTSB.c` | 8.85 | **9.50** | one repeat of an echo |
| `AcrSDK/common/plapx.c` | 8.99 | **9.48** | the pixel layout, the palette layout and the palette extent |
| `port/sdk/sdk_libpad2.c` | 9.38 | **9.68** | the eight button pressure bytes |
| `port/config/config.c` | 9.38 | **9.68** | `is_int`'s character test named |
| `Game/ui/sc_sub_logo.c` | 9.02 | **9.38** | the two halves of the logo wipe |
| `Source/Common/MemMan.c` | 9.00 | **9.24** | the upward gap scan only - both scans pair and cost half a point |
| `AcrSDK/common/memmgr.c` | 8.64 | **8.88** | the upward gap scan, and the two neighbour links |
| `Game/demo/demo01.c` | 9.63 | **10.00** | the title's later steps split off |
| `Game/effect/effa2.c` | 9.34 | **9.47** | the two bar-drawing steps |
| `Game/demo/demo02.c` | 9.31 | **9.38** | the frame the demo hands the screen back |
| `Game/ending/end_14.c` | 8.03 | **8.28** | the right-hand background placement |
| `Game/engine/plpnm.c` | 8.03 | 8.03 | two folds that each shrank the duplication group without moving the score |

**What was tried and reverted**, with the measurement, so it is not tried again:

| File | Attempt | Measured |
| --- | --- | --- |
| `Lz77Dec.c` | the decode loop flattened with `continue` | 8.81 -> 8.81, cc +1 |
| `port/config/keymap.c` | `get_button_name` split three ways | 8.92 -> 8.54; every split of a name switch reproduces its own shape as a duplicate |
| `Game/stage/bg_zoom.c` | the inner zoom switches lifted | 8.54 -> 8.28 |
| `Game/opening/opening_bg0.c` | scene 3's block switch lifted | 8.12 -> 8.03 |
| `AcrSDK/common/memmgr.c` | *both* gap scans lifted | 8.88 -> 8.15 |
| `Game/ending/end_01.c` | the two identical wait arms deduplicated | 9.60 -> 9.07; the file mean was under 4 and a cc-2 helper pushed it over |
| `Game/ending/end_01.c` | the fade arms split off | 9.60 -> 9.09 |
| `Game/engine/plpat00.c` | Jouka's first frame lifted | 9.06 -> 8.58 |
| `Game/effect/effa2.c` | three more blocks lifted | 9.47 -> 9.04 |
| `Game/com/com_sub_air_term.c` | the airborne half split off | 9.68 -> 9.09 |
| `AcrSDK/common/plapx.c` | the direct-colour layouts split off | 9.48 -> 9.38 |
| `Game/effect/effe9.c`, `effd1.c` | *both* arms of the mirrored pair | 10.00 -> 9.38 each |
| `AcrSDK/ps2/flps2etc.c` | the shared RGB and alpha runs | 9.84 -> 9.84 |
| `Game/rendering/mtrans_buffers.c` | the existing slot predicate reused in the 32 twin | 8.28 -> 8.28 |
| `Game/ui/netplay_menu.c` | the two page-entry arms lifted | 9.41 -> 9.09 |

**Verification.** Build and `refactor_guard.py` after every commit, in both `build` and
`build-dbg` wherever a file carries conditionally-compiled code or a `switch` over an
`enum`. The branch was then run through the wide replay gate,
`tools/replay_verify.sh <session start> 30 3600`: **30 seeds identical for 3600 saved
states each**. That covers the fight loop, which is what the pad-conversion changes in
`ioconv.c`, `sdk_libpad2.c` and `mlPAD.c` feed, and what the CPU-side changes in
`com_pl*.c` and the state changes in `plpnm.c` and `plpat09.c` sit inside.

What the replay gate does not reach is arcade, bonus and training flow, the attract
sequence, the menus and the endings. **Targeted playtesting is worth pointing at**
`Game/game.c`'s character-select entry and exit, `game_demo.c`'s attract stages,
`netplay_menu.c`'s confirm path, `pulpul.c`'s vibration state machine and `end_14.c` /
`end_18.c`.

## Folder campaigns

Work that is not a numbered task, because none of these files were in the Red band.
Recorded here so a later agent can see what has already been swept.

| Folder | Files | Before | After | Notes |
| --- | --- | --- | --- | --- |
| `Game/ui` | 9 -> 11 | 4.06 - 9.92 | mean **8.58** | `sc_sub.c` split five ways; *Recipe A clears one finding, not fifteen*, and the re-wrap its call sites needed cost two files a band until they were split again |
| `Game/com/passive` | 20 -> 67 | 4.90 - 7.55 | mean **9.65**, 47 at 10.00 | 3488 CPU pattern scripts folded onto shared skeletons; see *Where `Game/com/passive` stopped* in `PLAYBOOK.md` |
| `Game/com/active` | 20 -> 22 | 5.04 - 8.03 | mean **9.33**, 11 at 10.00 | 1621 scripts, the same shape under another name; folded folder-wide first, then again across both folders at once |
| `Game/com/patterns` | 0 -> 14 | - | mean **8.02**, 2 at 10.00 | the shared skeleton module the two script folders used to hold twice |

Both COM script folders are done, and since 2026-09-19 they share one skeleton module in
`Game/com/patterns`. `tools/passive_fold.py --family {passive,active,com}` carries the
transformations and the equivalence check; `--family com` works on both folders at once
against the shared module, which is what took the active folder past 8.64. See *Where
`Game/com/active` stopped the second time* in `PLAYBOOK.md` for the sequence, the four
refusals, and where the floor is.

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
| R04 | **1.97** | 2.25 | **10.00** | Done. Split into `extra_option.c`, `direction_menu.c`, `menu_selection.c`, `blocking_training.c` and more; `netplay_menu.c` was the lowest of the family at 8.59 and is **9.41** since 2026-09-20. |
| R05 | 2.31 | 2.31 | **9.39** | Plateau. Split into `cmd_main_checks.c`, now **7.50** and the hardest file left in the folder: its mean needs thirteen more functions against a duplicate web that charges for every one. `cmd_main.c` itself stops on the two `latch_sw_lvbt_bit_*` functions, whose cc 11 is almost entirely `case` labels. |
| R06 | 2.57 | 5.24 | **8.03** | Plateau. Split into `mtrans_seqs.c` (**10.00**), `mtrans_pool.c` (9.38) and, on 2026-09-19, `mtrans_buffers.c` (8.28). The size finding that stopped it is gone: "every seam runs through a shared static" was read as covering the buffer pool, and the statics there are *functions*, so an internal header cuts it the way PPGFile was cut. What is left is the 16/32 twin web, which travelled into the new file with the pool. |
| R07 | 2.58 | 2.58 | **8.54** | Plateau, re-tested 2026-09-19. Code Duplication is the only finding, and it is five groups of exactly **two**. Four are near-misses the catalogue refuses: `eff09_0000`/`eff09_8000` and the two `advance_*` functions differ in their conditions and their callees, and `initialize_eff09_4000`/`_11000` differ in the *operators* of their mirrored branch. The fifth, `adjust_sean_ball_left`/`_right`, differs in nothing but literals - a mask, a counter index, two limits and a column - and is blocked only by Recipe V's three-instance rule. It is the same open question `Game/ending` left: no third instance exists anywhere in the tree (`grep` finds the skeleton four times, all in this file). |
| R08 | 2.64 | 2.64 | **10.00** | Done. |
| R09 | 2.74 | 2.74 | **10.00** | Done. Split into `pls00_normal_states.c` (now **8.03**) and `pls00_damage_states.c` (10.00); the original is at 10.00. The normal-states file plateaus on a Code Duplication web between the `nm_*` state machines themselves, which no shared run reaches. |
| R10 | 3.09 | 3.09 | **10.00** | Done for the original; the folder was reworked on 2026-09-19. `opening.c` **10.00**, `opening_scenes.c` 7.42 -> **9.09**, `opening_scenes_late.c` 6.77 -> **9.09**, `opening_bg0.c` 7.54 -> **8.12**, `opening_bg1.c` **8.28**. The earlier note - that chaining one scene pays and chaining the next costs - was true of the scenes *as they were*: every scene step was eight lines of cue-test, advance and background move. Folding those onto `opening_cue_step` and its two variants (fifty steps across the two files) made the arms one line each, and only then did chaining the long switches pay. What stops the two bg files is the sibling state machines reading as copies of one another, with different statements in the same shape - Recipe D's forbidden case. |
| R11 | 3.49 | 3.49 | **9.31** | Plateau. Split five ways: `PPGFile_quads.c` (**10.00**), `PPGFile_chunks.c` (**10.00**), `PPGFile_palettes.c` (**10.00**) and `PPGFile_context.c` (**10.00**); group mean 9.86. The 2026-09-19 pass took the chunk file from 6.99 by extracting every loop and lookup out of the six setup functions and then splitting the palette side off, which cleared Overall Code Complexity on both halves. What stops `PPGFile.c` is the palette/texture twin pair - `ppgRelease*Handle` and `ppgGetUsing*Handle` differ in their type and in three callees each, which is Recipe D's forbidden case. |
| R12 | 3.56 | 3.56 | **9.38** | Plateau. Split into `appear_late.c`, now also 9.38. Overall Code Complexity is the only finding left in either. |
| R13 | 3.56 | 3.56 | **10.00** | Done. |
| R14 | 3.62 | 3.62 | **9.09** | Plateau. Split into `bg_textures.c` (10.00); the whole stage folder went with it. |
| R15 | 3.68 | 3.68 | **10.00** | Done. `plpdm.c`, `plpdm_states.c` and `plpdm_states_late.c` are all at 10.00. The last two were the campaign's most stubborn plateau until the runs the damage states share were taken instead of their arms - see *Against a twin family, share what they agree on* in `PLAYBOOK.md`. |
| R16 | 3.75 | 3.75 | **8.28** | Plateau. Split six ways: `ck_pass_checks.c` (**10.00**), `ck_pass_units_a.c` (**10.00**), `_b.c` (9.38), `_c.c` (**10.00**), `_d.c` (**10.00**). What stops `ck_pass.c` is `KEN_vs`, `HUGO_vs` and `GILL_vs`: re-diffed 2026-09-19 and the refusal holds - Hugo discards the jump check's answer and has no personal action, Gill adds a squat check. Below them, three *area helper* pairs were exact Recipe F cases and were folded on 2026-09-19, 8.03 -> 8.28; the earlier note had read the `*_vs` refusal as covering the whole file. |
| R17 | 3.86 | 3.86 | **10.00** | Done. Split into `entry_break_in.c`. |
| R18 | 3.92 | 3.92 | **9.68** | Plateau. Split twice, into `charset_commands.c` (10.00) and `charset_position.c` (10.00). What stops the original is `set_char_move_init2`'s five arguments. Recipe A would clear it, and the obstacle is scope rather than any one file: **155** call sites, 111 of them in the effect folder and 13 in animation. |
| R19 | 3.92 | 3.92 | **9.92** | Plateau. Split into `manage_bonus.c` (10.00) and `manage_result.c` (9.38). `Game_Manage_7_3`'s dead condition is the last finding and the catalogue forbids deleting it. |

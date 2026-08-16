# Statcheck

Statcheck is a special version of 3SX that can run Fightcade replays to verify engine's arcade accuracy.

## How it works

Here's a quick overview of how Statcheck works:

1. Download raw Fightcade replays using `tools/fcade-replays`.
2. Run them through `fbneo-replay-runner` to collect per-frame RAM dumps in the form of an SCRD archive.
3. Run Statcheck on the archive: parse inputs, apply them programmatically, compare engine state, repeat per-frame.

## Fightcade replays

`tools/fcade-replays` allows downloading replays from Fightcade either by id or in bulk.

### Discovery

Fightcade's REST API (https://www.fightcade.com/api/) is used to search and filter replays by date, username and game. `fcade-replays` handles paging, bulk downloads and API errors.

If you wanna download replays for a different game you can do so by specifying a different game id.

### Downloading and format

Fightcade uses a custom raw TCP protocol to transmit replay data to clients. `fcade-replays` implements this protocol to allow downloading any Fightcade replays.

The protocol consists of a handshake, acknowledgment messages and 3 types of payloads that are of interest to us: metadata, initial state and input records.

Initial state is a zlib-compressed FBNeo savestate. Fightcade's FBNeo fork starts processing input data after applying this savestate.

## Replay runner

`fbneo-replay-runner` is a custom fork of `fightcade-fbneo` that implements several features needed to enable Statcheck.

It recognizes Fightcade's input data format and replays it from the supplied savestate.

It can save per-frame RAM dumps in a specialized SCRD container.

Finally, its `--headless` mode allows running the emulator without a window to speed up replay processing

## SCRD

SCRD is a custom binary format that stores CPS3 RAM dump sequences. It's able to compress raw RAM dumps by a factor of 80-100, so a typical replay takes about 40-50MB instead of 4-5GB.

It saves a ton of space by leveraging **zero-run encoding**. Consecutive RAM frames usually differ in just a few places, so when you XOR two consecutive frames the result is gonna consist of a ton of zeros with just a few ones. This kind of data compresses well with zero-run encoding.

That makes compression algorithm very simple:

0. Store frame 0 as-is
1. Compute a XOR of the current and previous frames
2. Encode the XOR sequence with zero-run encoding and store it in the archive
3. Repeat steps 1-2 for every frame

XOR'ing by the same value twice restores the original value. This allows us to easily restore full RAM dumps:

0. Read frame 0 as-is
1. Read and zero-run decode a frame from the archive
2. XOR decoded data with the current frame to get the next frame
3. Repeat steps 1-2 for every frame

## Accuracy verification

Statcheck reads the provided SCRD archive, extracts inputs and relevant engine data and compares it to the same data in 3SX. Because SCRD frames are raw CPS3 RAM dumps, finding relevant data points boils down to knowing their memory offsets.

When Statcheck encounters a mismatch, it stops execution and prints a message with the exact values that should be equal but aren't. It mainly compares data that is visible to the player:

- health
- positions
- stun meter
- SA stocks and meter
- round timer
- round phase (start, in-game, win animation, etc.)

`tools/statcheck_runner.py` can run replays through Statcheck in bulk, report aggregate statistics and save the report with detailed mismatch descriptions to a file.

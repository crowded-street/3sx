# Fightcade Replay Tool (WIP)

Install Python dependencies for the helper scripts with:

```bash
python3 -m pip install -r tools/requirements-python.txt
```

`fcade_replay_tool.py` is a reverse-engineering helper for Fightcade replay streams.

It supports:
- Connecting to `ggpo.fightcade.com:<port>` and running the observed token handshake
- Saving framed server messages and parsing known message types (`3`, `-12`, `-13`)
- Listing and bulk-downloading Fightcade replays through the observed `searchquarks` API

## Requirements

- Python 3.9+

## Usage

Download and parse from a known `fcade://` URL:

```bash
python3 tools/fcade-replays/fcade_replay_tool.py download \
  --fcade-url "fcade://stream/fbneo/sf2ce/1771978700790-3121.7,7100" \
  --local-port 6004 \
  --idle-timeout 2 \
  --max-idle-timeouts 20 \
  --auto-dir
```

List the latest replay stream targets for a game:

```bash
export FCADE_COOKIE='cf_clearance=...'
python3 tools/fcade-replays/fcade_replay_tool.py list-replays \
  --gameid sfiii3nr1 \
  --count 30
```

Download the latest 200 replays for a game:

```bash
export FCADE_COOKIE='cf_clearance=...'
python3 tools/fcade-replays/fcade_replay_tool.py bulk-download \
  --gameid sfiii3nr1 \
  --count 200 \
  --keep-going
```

Download the monthly-best list shape observed in the Fightcade UI:

```bash
python3 tools/fcade-replays/fcade_replay_tool.py bulk-download \
  --gameid sfiii3nr1 \
  --best \
  --since 1780099200000 \
  --count 200 \
  --keep-going
```

Convert a directory of downloaded replay folders into compressed RAM dumps:

```bash
python3 tools/replay_preprocessor.py \
  tools/fcade-replays/output/bulk \
  tools/fcade-replays/output/compressed \
  --runner build/release/fbneosdlarm64
```

The wrapper runs each complete replay through the `--runner` executable, stores the
runner's uncompressed `game_N` RAM dumps in a temporary directory, then writes
`<replay-id>/game_N.scrd` under the requested output directory. Use `--runner` to
select another executable, and `--game sfiii3nr1` when the replay folders do not
have `summary.json` files. Each replay's temporary dumps are deleted before the
next replay starts.

Run statcheck against every preprocessed game archive:

```bash
python3 tools/statcheck_runner.py \
  build-statcheck/3SX.app/Contents/MacOS/3SX \
  tools/fcade-replays/output/compressed
```

The runner prints the result for each game and writes output from failed or crashed
statcheck processes to a temporary report file whose path is printed at the end.

## Output Files

Each output directory contains:

- `frames.bin`: concatenated protocol frames (`u32be length + payload`)
- `summary.json`: parsed per-message metadata
- `savestate`: first decompressed `type=-12` payload
- `inputs`: all `type=-13` record bodies concatenated in receive order

Bulk runs write one replay directory per target using `<game>-<replay-id>` names.
Each directory also contains `quark.json` with the original API row. The top-level
`bulk_manifest.json` records output paths and statuses for each attempted replay.

## Notes

- The protocol understanding is still incomplete and message field names are provisional.
- Fightcade's API may require a current Cloudflare clearance cookie copied from a browser session.
  Pass it with `--cookie 'cf_clearance=...'` or set `FCADE_COOKIE`.

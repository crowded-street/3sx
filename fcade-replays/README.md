# Fightcade Replay Tool (WIP)

`fcade_replay_tool.py` is a reverse-engineering helper for Fightcade replay streams.

It supports:
- Connecting to `ggpo.fightcade.com:<port>` and running the observed token handshake
- Saving framed server messages and parsing known message types (`3`, `-12`, `-13`)

## Requirements

- Python 3.9+

## Usage

Download and parse from a known `fcade://` URL:

```bash
python3 fcade-replays/fcade_replay_tool.py download \
  --fcade-url "fcade://stream/fbneo/sf2ce/1771978700790-3121.7,7100" \
  --local-port 6004 \
  --idle-timeout 2 \
  --max-idle-timeouts 20 \
  --auto-dir
```

## Output Files

Each output directory contains:

- `frames.bin`: concatenated protocol frames (`u32be length + payload`)
- `summary.json`: parsed per-message metadata
- `savestate`: first decompressed `type=-12` payload
- `inputs`: all `type=-13` record bodies concatenated in receive order

## Notes

- The protocol understanding is still incomplete and message field names are provisional.

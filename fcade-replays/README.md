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
- `msg_XXXX_minus12.bin`: compressed state chunk payload for `type=-12`
- `msg_XXXX_minus12.decompressed.bin`: zlib output when decompression succeeds
- `msg_XXXX_minus13.bin`: raw record body for `type=-13`

## Notes

- The protocol understanding is still incomplete and message field names are provisional.

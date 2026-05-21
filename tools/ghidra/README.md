# Ghidra Query Bridge

This directory contains the repo-side Ghidra query wrapper and Ghidra-side
script.

## Setup

Copy the repo-level example env file and adjust paths for your machine:

```sh
cp .env.example .env
```

Required values:

- `GHIDRA_HEADLESS`: path to Ghidra's `pyghidraRun` launcher.
- `JAVA_HOME`: path to a JDK supported by your Ghidra install.
- `GHIDRA_PROJECT`: path to the `.gpr` project containing the CPS3 program.
- `GHIDRA_PROGRAM`: program/file name inside that Ghidra project.

You can also provide these values as shell environment variables or override
them with command-line flags. To load a different env file, set
`GHIDRA_QUERY_ENV`.

Examples:

```sh
tools/ghidra/ghidra_query.py --search FUN_060008 --limit 5
tools/ghidra/ghidra_query.py --target 06000840 --decompile --bytes 16 --limit 5
tools/ghidra/ghidra_query.py --target FUN_06000840 --decompile
```

If the project is open in the Ghidra GUI, the wrapper automatically retries
against a temporary copy of the project so read-only queries can still run.

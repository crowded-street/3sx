"""macOS Makefiles build smoke: exercise production netplay against a local server.

Run after cmake --build build: python3 tools/tests/run_arcade_matchmaking_smoke.py
The harness uses the game's objects with a separate entry point and temp profiles.
"""
import pathlib
import shlex
import socket
import subprocess
import tempfile
import threading
import time

ROOT = pathlib.Path(__file__).resolve().parents[2]
BUILD = ROOT / "build"
flags = {}
for line in (BUILD / "CMakeFiles/3sx.dir/flags.make").read_text().splitlines():
    if " = " in line:
        name, value = line.split(" = ", 1)
        flags[name] = shlex.split(value)

with tempfile.TemporaryDirectory(prefix="3sx-netplay-test-") as directory:
    temp = pathlib.Path(directory)
    obj = temp / "smoke.o"
    exe = temp / "smoke"
    subprocess.run(["clang", *flags["C_DEFINES"], *flags["C_INCLUDES"],
                    "-std=gnu11", "-c", str(ROOT / "tools/tests/arcade_matchmaking_smoke.c"),
                    "-o", str(obj)], check=True)
    link = shlex.split((BUILD / "CMakeFiles/3sx.dir/link.txt").read_text())
    link = [arg for arg in link if not arg.endswith("/src/port/paths.c.o")]
    link[link.index("-o") + 1] = str(exe)
    link += [str(obj), "-Wl,-e,_arcade_test_main"]
    subprocess.run(link, cwd=BUILD, check=True)

    for scenario in ("start", "error", "leave", "refresh"):
        errors = []
        listener = socket.socket()
        listener.bind(("127.0.0.1", 0))
        listener.listen()
        listener.settimeout(6)
        profile = temp / scenario
        profile.mkdir()
        if scenario == "refresh":
            (profile / "token").write_text(f"test-refresh\n{int(time.time()) + 3600}\n")

        def serve():
            try:
                with listener.accept()[0] as conn:
                    conn.settimeout(5)
                    stream = conn.makefile("rb")
                    conn.sendall(b"SESSION abc1234\n")
                    expected = b"REFRESH test-refresh\n" if scenario == "refresh" else b"HELLO\n"
                    assert stream.readline() == expected
                    if scenario == "error":
                        return
                    # No queuing before login. Even a device code must leave Arcade running.
                    conn.sendall(b"DAG ABCD1234 http://localhost/activate\n")
                    time.sleep(0.05)
                    conn.sendall(f"PROFILE player\nTOKEN refresh test-token {int(time.time()) + 3600}\n".encode())
                    assert stream.readline() == b"QUEUE add\n"
                    if scenario in ("leave", "refresh"):
                        assert stream.readline() == b""
                        return
                    match = "01234567-89ab-cdef-0123-456789abcdef"
                    conn.sendall(f"MATCH {match} opponent\n".encode())
                    time.sleep(0.1)
                    # Cancellation after auto-accept must allow queueing again.
                    conn.sendall(f"CANCEL {match}\n".encode())
                    assert stream.readline() == b"QUEUE add\n"
                    conn.sendall(f"MATCH {match} opponent\n".encode())
                    time.sleep(0.1)
                    conn.sendall(b"START 1 127.0.0.1:50001\n")
                    assert stream.readline() == b""
            except BaseException as error:
                errors.append(error)
            finally:
                listener.close()

        port = listener.getsockname()[1]
        thread = threading.Thread(target=serve)
        thread.start()
        result = subprocess.run([str(exe), str(port), "leave" if scenario == "refresh" else scenario,
                                 str(profile) + "/"], capture_output=True, text=True, timeout=10)
        thread.join(timeout=7)
        assert not thread.is_alive()
        assert not errors, errors
        assert result.returncode == 0, result.stdout + result.stderr
        assert "ARCADE_MATCHMAKING_SMOKE_OK" in result.stdout
        print(f"{scenario}: ARCADE_MATCHMAKING_SMOKE_OK")

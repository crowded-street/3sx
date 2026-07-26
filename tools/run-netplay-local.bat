@echo off
REM Launches two local instances wired to each other over the direct P2P path.
REM Player 1 binds UDP 50000 and talks to 50001, player 2 does the reverse --
REM Netplay_SetParams picks those ports whenever the remote IP is 127.0.0.1.
REM
REM Pick NETWORK in the menu in BOTH windows: that is what starts the session
REM (Netplay_Menu -> Netplay_BeginDirectP2P), and the sockets bind at that point.
REM Each instance logs to build\application\bin\netplay-p{1,2}.log.

setlocal

set "BIN=%~dp0..\build\application\bin"
set "EXE=%BIN%\3sx.exe"

REM Both instances would otherwise play the same audio twice.
set "P2_EXTRA=--no-sound"

if not exist "%EXE%" (
    echo Could not find "%EXE%".
    echo Build and install first:
    echo     cmake --build build --parallel
    echo     cmake --install build --prefix build/application
    exit /b 1
)

echo Starting player 1 ^(port 50000^)...
start "3sx - player 1" /D "%BIN%" cmd /c ""%EXE%" --p2p-local-player 1 --p2p-remote-ip 127.0.0.1 > netplay-p1.log 2>&1"

echo Starting player 2 ^(port 50001^)...
start "3sx - player 2" /D "%BIN%" cmd /c ""%EXE%" --p2p-local-player 2 --p2p-remote-ip 127.0.0.1 %P2_EXTRA% > netplay-p2.log 2>&1"

echo.
echo Now pick NETWORK in the menu in both windows.
echo Logs: %BIN%\netplay-p1.log and netplay-p2.log

endlocal

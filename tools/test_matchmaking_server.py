"""
Matchmaking Server
TCP (port 9000): assigns each client a unique 7-char ID on connect.
UDP (port 9001): clients send their ID; once two clients have registered
                 UDP endpoints each receives the other's "ip:port" over TCP.
"""

import asyncio
import logging
import random
import string
from collections import deque
from dataclasses import dataclass, field

log = logging.getLogger(__name__)

TCP_HOST = "0.0.0.0"
TCP_PORT = 9000
UDP_HOST = "0.0.0.0"
UDP_PORT = 9001

_ID_LENGTH = 7
_ID_CHARS = string.ascii_lowercase + string.digits

Addr = tuple[str, int]


@dataclass
class Player:
    id: str
    writer: asyncio.StreamWriter
    udp_endpoint: Addr | None = field(default=None)


class MatchmakingCoordinator:
    """
    Safe without locks because asyncio is cooperative and every mutation
    happens in a coroutine with no await between reads and writes of shared state.
    """

    def __init__(self) -> None:
        self._players: dict[str, Player] = {}
        self._waiting: deque[str] = deque()

    async def register_player(self, writer: asyncio.StreamWriter) -> None:
        player = Player(id=self._generate_id(), writer=writer)
        self._players[player.id] = player
        log.info("Player registered: %s  %s", player.id, writer.get_extra_info("peername"))
        await self._send_tcp(writer, player.id)

    async def remove_player(self, writer: asyncio.StreamWriter) -> None:
        player = next((p for p in self._players.values() if p.writer is writer), None)
        if player is None:
            return
        del self._players[player.id]
        self._waiting = deque(pid for pid in self._waiting if pid != player.id)
        log.info("Player removed: %s", player.id)

    async def note_udp_endpoint(self, player_id: str, addr: Addr) -> None:
        player = self._players.get(player_id)
        if player is None:
            log.warning("UDP endpoint for unknown player: %s", player_id)
            return
        if player.udp_endpoint is not None:
            return
        player.udp_endpoint = addr
        self._waiting.append(player_id)
        log.info("Player %s UDP endpoint: %s:%d", player_id, *addr)
        await self._try_match()

    async def _send_match_info(self, writer: asyncio.StreamWriter, player_num: str, ep: str) -> None:
        await self._send_tcp(writer, f"{player_num} {ep}")

    async def _try_match(self) -> None:
        while len(self._waiting) >= 2:
            a = self._players.get(self._waiting.popleft())
            b = self._players.get(self._waiting.popleft())
            if a is None or b is None:
                log.warning("Match candidate vanished, skipping")
                continue
            if not a.udp_endpoint or not b.udp_endpoint:
                log.error("Player in waiting queue has no UDP endpoint")
                continue
            a_ep = f"{a.udp_endpoint[0]}:{a.udp_endpoint[1]}"
            b_ep = f"{b.udp_endpoint[0]}:{b.udp_endpoint[1]}"
            log.info("Matched %s (P1) <-> %s (P2)", a.id, b.id)
            await asyncio.gather(
                self._send_match_info(a.writer, "1", b_ep),
                self._send_match_info(b.writer, "2", a_ep),
            )

    @staticmethod
    async def _send_tcp(writer: asyncio.StreamWriter, message: str) -> None:
        try:
            writer.write((message + "\n").encode())
            await writer.drain()
        except (ConnectionResetError, BrokenPipeError) as exc:
            log.warning("Could not send TCP message: %s", exc)

    def _generate_id(self) -> str:
        while True:
            candidate = "".join(random.choices(_ID_CHARS, k=_ID_LENGTH))
            if candidate not in self._players:
                return candidate


async def TCPHandler(
    reader: asyncio.StreamReader,
    writer: asyncio.StreamWriter,
    coordinator: MatchmakingCoordinator,
) -> None:
    peer = writer.get_extra_info("peername")
    log.info("TCP connection from %s", peer)
    await coordinator.register_player(writer)
    try:
        while await reader.readline():
            pass
    except (asyncio.IncompleteReadError, ConnectionResetError, OSError):
        pass
    finally:
        log.info("TCP disconnected: %s", peer)
        await coordinator.remove_player(writer)
        writer.close()
        try:
            await writer.wait_closed()
        except OSError:
            pass


class UDPHandler(asyncio.DatagramProtocol):
    def __init__(self, coordinator: MatchmakingCoordinator) -> None:
        self._coordinator = coordinator

    def datagram_received(self, data: bytes, addr: Addr) -> None:
        if len(data) < _ID_LENGTH:
            return
        player_id = data[:_ID_LENGTH].decode(errors="replace")
        asyncio.ensure_future(self._coordinator.note_udp_endpoint(player_id, addr))

    def error_received(self, exc: Exception) -> None:
        log.error("UDP error: %s", exc)


async def main() -> None:
    logging.basicConfig(level=logging.INFO, format="%(asctime)s [%(levelname)s] %(message)s")

    coordinator = MatchmakingCoordinator()
    loop = asyncio.get_running_loop()

    tcp_server = await asyncio.start_server(
        lambda r, w: TCPHandler(r, w, coordinator),
        host=TCP_HOST,
        port=TCP_PORT,
        reuse_address=True,
    )
    log.info("TCP listening on %s:%d", TCP_HOST, TCP_PORT)

    udp_transport, _ = await loop.create_datagram_endpoint(
        lambda: UDPHandler(coordinator),
        local_addr=(UDP_HOST, UDP_PORT),
    )
    log.info("UDP listening on %s:%d", UDP_HOST, UDP_PORT)

    try:
        async with tcp_server:
            await tcp_server.serve_forever()
    finally:
        udp_transport.close()
        log.info("Server shut down.")


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        log.info("Interrupted – exiting.")

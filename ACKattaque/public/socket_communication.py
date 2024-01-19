from ACKattaque.public.game_message import GameMessage, ACKStream
from ACKattaque.public.spaceship_message import SpaceshipMessage
from ACKattaque.public.spaceship_command import SpaceshipCommand
from ACKattaque.public.tower_command import TowerCommand


from socketserver import ThreadingTCPServer, TCPServer, BaseRequestHandler
import socket
from dataclasses import dataclass
from threading import Thread

from socket import timeout
import traceback
import sys

from concurrent.futures import ThreadPoolExecutor

def send_int(sock, number):
    sock.sendall(number.to_bytes(8, 'big'))

def recv_int(sock):
    return int.from_bytes(sock.recv(8), 'big')

def stop_process_pool(executor):
    for pid, process in executor._processes.items():
        process.terminate()
    executor.shutdown()

class PythonTCPHandler(BaseRequestHandler):
    SOCKET_TIMEOUT = 25
    BOT_TIMEOUT = 1

    def handle(self):
        self.request.settimeout(self.SOCKET_TIMEOUT)  # Set the timeout for the socket

        try:
            request_type = self.request.recv(4)
            if not request_type:
                raise timeout('No data received')

            data_length = recv_int(self.request)
            game_message = ''
            while len(game_message) != data_length:
                game_message += self.request.recv(data_length).decode('utf8')

            match request_type:
                case b'TOWR':
                    with ThreadPoolExecutor() as p:
                        try:
                            f = p.submit(self.server.tower.send_messages, game_message)
                            command_json = f.result(timeout=self.BOT_TIMEOUT)
                        except timeout:
                            print(f"Tower could not answer within {self.BOT_TIMEOUT} second.")
                            command_json = TowerCommand([]).to_json()
                            stop_process_pool(p)
                case b'SHIP':
                    with ThreadPoolExecutor() as p:
                        try:
                            spaceship = self.server.spaceship_class()
                            f = p.submit(spaceship.get_command, game_message)
                            command_json = f.result(timeout=self.BOT_TIMEOUT)
                        except TimeoutError:
                            print(f"Spaceship could not answer within {self.BOT_TIMEOUT} second.")
                            command_json = SpaceshipCommand([], ACKStream([])).to_json()
                            stop_process_pool(p)
                case _:
                    raise RuntimeError('TCP server could not parse request_type from socket data.')

            send_int(self.request, len(command_json))
            self.request.sendall(command_json.encode('utf8'))
        finally:
            self.finish()

    def finish(self):
        self.request.close()
    
class PythonTCPServer(TCPServer):
    def handle_error(self, request, client_address) -> None:
        exc = sys.exception()

        if type(exc) is ConnectionAbortedError:
            print('-'*40, file=sys.stderr)
            print('Remote server timed out...', file=sys.stderr)
            print('-'*40, file=sys.stderr)
        else:
            print('-'*40, file=sys.stderr)
            print('Exception occurred during processing of request.', file=sys.stderr)
            summ = traceback.extract_tb(exc.__traceback__)
            n_show = len(summ) - 5
            traceback.print_exc(limit = -n_show)
            print('-'*40, file=sys.stderr)

            print("Shutting down the whole server...", file = sys.stderr)
            self._BaseServer__shutdown_request = True

def start_python_socket_server(port, tower_class, spaceship_class, background=False):
    server = PythonTCPServer(('localhost', port), PythonTCPHandler, bind_and_activate=False)
    server.allow_reuse_port = True
    server.server_bind()
    server.server_activate()
    server.tower = tower_class()
    server.spaceship_class = spaceship_class

    if background:
        server_thread = Thread(target=server.serve_forever)
        server_thread.daemon = True
        server_thread.start()
    else:
        server.serve_forever()

def tower_client(port, game_message: GameMessage) -> TowerCommand:
    game_message_json = game_message.to_json()

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.settimeout(0.1)
        sock.connect(('localhost', port))
        
        sock.settimeout(2.0)
        sock.sendall(b'TOWR')
        send_int(sock, len(game_message_json))
        sock.sendall(game_message_json.encode('utf8'))

        data_size = recv_int(sock)
        tower_command = sock.recv(data_size)

    return TowerCommand.from_json(tower_command)

def spaceship_client(port, game_message: SpaceshipMessage) -> SpaceshipCommand:
    game_message_json = game_message.to_json()

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.settimeout(0.1)
        sock.connect(('localhost', port))
        
        sock.settimeout(2.0)
        sock.sendall(b'SHIP')
        send_int(sock, len(game_message_json))
        sock.sendall(game_message_json.encode('utf8'))

        data_size = recv_int(sock)
        spaceship_command = sock.recv(data_size)

    return SpaceshipCommand.from_json(spaceship_command)
import socketserver
import threading

HOST = "localhost"
PORT1 = 11111
PORT2 = 11112
BUFFER_SIZE = 1024


class PingHandler(socketserver.BaseRequestHandler):
    def handle(self) -> None:
        self.data = self.request.recv(BUFFER_SIZE)
        print(
            f"{self.client_address[0]} to port {self.server.server_address[1]}: {self.data}"
        )
        self.request.sendall(
            f"Pingback from port {self.server.server_address[1]}".encode("utf-8")
        )


if __name__ == "__main__":
    server1 = socketserver.TCPServer((HOST, PORT1), PingHandler)
    server2 = socketserver.TCPServer((HOST, PORT2), PingHandler)

    print(f"Server 1 running on {HOST}: {PORT1}")
    print(f"Server 2 running on {HOST}: {PORT2}")

    thread1 = threading.Thread(target=server1.serve_forever)
    thread2 = threading.Thread(target=server2.serve_forever)

    thread1.start()
    thread2.start()

import socketserver

HOST, PORT = "localhost", 11111
BUFFER_SIZE = 1024


class TestHandler(socketserver.BaseRequestHandler):
    def handle(self) -> None:
        self.data = self.request.recv(BUFFER_SIZE).strip()
        print(f"{self.client_address[0]}: {self.data}")
        self.request.sendall("Server ping-back".encode("utf-8"))


if __name__ == "__main__":
    with socketserver.TCPServer((HOST, PORT), TestHandler) as server:
        print(f"Server is running on {HOST} port {PORT}")
        server.serve_forever()

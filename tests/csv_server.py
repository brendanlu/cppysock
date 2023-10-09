import pandas as pd
import socketserver

HOST, PORT = "localhost", 11111
BUFFER_SIZE = 1024


class CSVHandler(socketserver.BaseRequestHandler):
    def handle(self) -> None:
        pass


if __name__ == "__main__":
    with socketserver.TCPServer((HOST, PORT), CSVHandler) as server:
        print(f"Server is running on {HOST} port {PORT}")
        server.serve_forever()

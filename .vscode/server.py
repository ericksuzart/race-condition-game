# server.py
import socket
import threading

# Server settings
HOST = '127.0.0.1'  # Localhost
PORT = 5555

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((HOST, PORT))
server.listen(2)

clients = []

def handle_client(conn, addr, player_id):
    conn.send(str(player_id).encode())
    while True:
        try:
            data = conn.recv(1024).decode()
            if not data:
                break

            # Broadcast position to the other player
            for client in clients:
                if client != conn:
                    client.send(data.encode())
        except:
            break

    conn.close()
    clients.remove(conn)

def main():
    print("Server started, waiting for connections...")
    while len(clients) < 2:
        conn, addr = server.accept()
        print(f"Connected to {addr}")
        clients.append(conn)
        threading.Thread(target=handle_client, args=(conn, addr, len(clients) - 1)).start()

if __name__ == "__main__":
    main()

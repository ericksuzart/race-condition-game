# client.py
import socket
import threading

import pygame

# Pygame settings
WIDTH, HEIGHT = 500, 500
WHITE = (255, 255, 255)
RED = (255, 0, 0)
BLUE = (0, 0, 255)

# Networking settings
HOST = '127.0.0.1'
PORT = 5555

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((HOST, PORT))

player_id = int(client.recv(1024).decode())
positions = [(50, 50), (400, 400)]  # Initial positions for two players
vel = 5

def handle_server():
    while True:
        try:
            data = client.recv(1024).decode()
            x, y = map(int, data.split(','))
            positions[1 - player_id] = (x, y)  # Update opponent's position
        except:
            break

threading.Thread(target=handle_server, daemon=True).start()

def main():
    pygame.init()
    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("Multiplayer Game")

    clock = pygame.time.Clock()

    run = True
    while run:
        clock.tick(30)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False

        keys = pygame.key.get_pressed()
        x, y = positions[player_id]

        if keys[pygame.K_LEFT] and x - vel > 0:
            x -= vel
        if keys[pygame.K_RIGHT] and x + vel < WIDTH - 50:
            x += vel
        if keys[pygame.K_UP] and y - vel > 0:
            y -= vel
        if keys[pygame.K_DOWN] and y + vel < HEIGHT - 50:
            y += vel

        positions[player_id] = (x, y)

        # Send updated position to server
        client.send(f"{x},{y}".encode())

        # Draw everything
        screen.fill(WHITE)
        pygame.draw.rect(screen, RED if player_id == 0 else BLUE, (positions[0][0], positions[0][1], 50, 50))
        pygame.draw.rect(screen, BLUE if player_id == 0 else RED, (positions[1][0], positions[1][1], 50, 50))
        pygame.display.update()

    pygame.quit()
    client.close()

if __name__ == "__main__":
    main()

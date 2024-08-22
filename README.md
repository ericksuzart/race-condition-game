# Race Condition Game

## Description

Race Condition Game is a multiplayer drag racing game developed for a programming class to demonstrate the race condition phenomenon in a fun and interactive way. The game simulates a race where up to 4 clients compete by moving their cars across the screen, and the server manages the game logic. This game illustrates the concept of race conditions by showing how simultaneous actions from multiple clients are handled by the server, showing the unpredictability and potential conflicts found in concurrent programming.

Each client controls a unique car, and the objective is to be the first to cross the finish line. The server ensures that all clients are synchronized, but the outcome of the race showcases how simultaneous input from multiple clients can lead to unpredictable results.

This game is designed for local area network (LAN) play, where one machine runs the server and others connect as clients to participate in the race.

## Dependencies

This game requires the following libraries and tools to be installed on your Linux system:

- **`SDL2`**: The Simple DirectMedia Layer library for graphics rendering and input handling.
- **`g++`**: The GNU C++ compiler for building the project.
- **`Make`**: For managing the build process.

### Install SDL2 on Linux (Ubuntu/Debian)

You can install the necessary dependencies via apt:

``` bash
Copy code
sudo apt update
sudo apt install libsdl2-dev build-essential
```

## How to Run

### 1. Clone the repository

First, clone the repository containing the server and client code:

```bash
git clone git@github.com:ericksuzart/race-condition-game.git
cd race-condition-game
```

### 2. Build the Project

The project is built using Make. Run the following command in the project directory to build both the server and client:

```bash
cd race-condition-game
make 
```

This will generate two executables: server and client.

## 3. Running the Server
****
To start the server, run the following command in the terminal:

```bash
cd race-condition-game
./server
```

This starts the server and waits for up to 4 clients to connect. The server will handle the game logic and broadcast the status to the clients.

### 4. Running the Client

To run a client, use the following command:

```bash
cd race-condition-game
./client
```

This connects the client to the server running at "127.0.0.1" on port 8081.

Each client will see the game window and can participate in the race by pressing the right arrow key to move their car.

### 5. Play the Game

- Up to 4 clients can connect to the server.
- Once all clients are connected, a countdown will start, and the race will begin.
- The first client to reach the finish line wins the race. Other clients will see a losing screen.

## Notes

- Ensure that the server and client machines are on the same local network.
- The server must be started before the clients attempt to connect.
- Use the server's IP address (found using ifconfig or ip a) when running the client from other machines.

## Troubleshooting

- Firewall Issues: Ensure that the server's port is open on the firewall for both incoming and outgoing traffic.
- Same Network: Both server and client must be on the same LAN or have proper routing if in different networks.

#include "Server.hpp"

#include <chrono>
#include <thread>

Server::Server(int port)
{
    int opt = 1;
    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)))
    {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(serverFd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverFd, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is running on port " << port
              << ", waiting for connections..." << std::endl;
}

Server::~Server()
{
    for (auto & t : clientThreads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
    close(serverFd);
}

void Server::start()
{
    while (true)
    {
        int newSocket;
        int addrlen = sizeof(address);
        if ((newSocket = accept(serverFd, (struct sockaddr *)&address,
                                (socklen_t *)&addrlen)) < 0)
        {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        {
            std::lock_guard<std::mutex> lock(positionsMutex);
            if (clientCount >= MAX_CLIENTS)
            {
                std::cerr << "Maximum clients reached. Rejecting connection."
                          << std::endl;
                close(newSocket);
                continue;
            }

            // Store the client's socket for later communication
            clientSockets.push_back(newSocket);

            // Initial positions with vertical spacing
            clientsPositions.push_back({0, 100 + clientCount.load() * 60});
            clientCount++;
        }

        int clientId = clientCount - 1;
        std::cout << "New client connected: " << clientId << std::endl;
        clientThreads.push_back(
          std::thread(&Server::handleClient, this, newSocket, clientId));

        // Only when all clients are connected, notify to start the countdown
        if (clientCount == MAX_CLIENTS)
        {
            std::cout << "All clients connected. Starting countdown..."
                      << std::endl;
            allClientsConnected.store(true);
            clientsReadyCondVar.notify_all();
        }
    }
}

void Server::sendCountdown()
{
    std::cout << "Sending countdown to all clients." << std::endl;
    for (int i = 3; i >= 0; --i)
    {
        std::string message = "COUNTDOWN " + std::to_string(i);
        for (auto clientSocket : clientSockets)
        {
            sendResponse(clientSocket, message);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    for (auto clientSocket : clientSockets)
    {
        sendResponse(clientSocket, "GOO!!!");
    }
}

void Server::sendResponse(int clientSocket, const std::string & message)
{
    send(clientSocket, message.c_str(), message.size(), 0);
}

void Server::handleClient(int clientSocket, int clientId)
{
    // Send the client's ID upon connection
    std::string clientIdMessage = "CLIENT_ID " + std::to_string(clientId);
    sendResponse(clientSocket, clientIdMessage);

    // Wait for all clients to be connected
    std::unique_lock<std::mutex> lock(positionsMutex);
    clientsReadyCondVar.wait(lock,
                             [this] { return allClientsConnected.load(); });

    // Only one thread should start the countdown (ensure no duplication)
    if (clientId == 0)
    {
        sendCountdown();
    }

    Position pos = clientsPositions[clientId];
    lock.unlock();  // Unlock the mutex after sending countdown

    while (true)
    {
        char buffer[1024] = {0};
        ssize_t readSize = read(clientSocket, buffer, sizeof(buffer));
        if (readSize <= 0)
        {
            std::cout << "Client " << clientId << " disconnected."
                      << std::endl;
            break;
        }

        std::string request(buffer, readSize);

        {
            std::lock_guard<std::mutex> lock(positionsMutex);
            if (winnerId.load() == -1 && request == "MOVE_RIGHT")
            {
                pos.x += 10;

                if (pos.x >= WINNING_POSITION)
                {
                    winnerId.store(clientId);
                    std::string winnerMessage =
                      "WINNER " + std::to_string(winnerId.load());

                    // Notify all clients who the winner is
                    for (auto clientSocket : clientSockets)
                    {
                        sendResponse(clientSocket, winnerMessage);
                    }
                    break;
                }

                clientsPositions[clientId] = pos;
            }

            // Send updated positions as a response
            std::string positionsResponse;
            for (const auto & p : clientsPositions)
            {
                positionsResponse +=
                  std::to_string(p.x) + " " + std::to_string(p.y) + "\n";
            }

            sendResponse(clientSocket, positionsResponse);
        }
    }

    close(clientSocket);
}

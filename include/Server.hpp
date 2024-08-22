#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <condition_variable>
#include <cstring>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

struct Position
{
    int x;
    int y;
};

class Server
{
   public:
    Server(int port);
    ~Server();
    void start();

   private:
    static const int MAX_CLIENTS = 4;
    static const int WINNING_POSITION = 800;
    void handleClient(int clientSocket, int clientId);
    void sendCountdown();
    void sendResponse(int clientSocket, const std::string & message);

    int serverFd;
    struct sockaddr_in address;
    std::vector<Position> clientsPositions;
    std::mutex positionsMutex;
    std::condition_variable clientsReadyCondVar;
    std::vector<std::thread> clientThreads;
    std::vector<int> clientSockets;
    std::atomic<int> clientCount{0};
    std::atomic<int> winnerId{-1};
    std::atomic<bool> allClientsConnected{false};
};

#endif  // SERVER_HPP

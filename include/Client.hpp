#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <SDL2/SDL.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <atomic>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

struct Position
{
    int x;
    int y;
};

class Client
{
   public:
    Client(const std::string & serverIp, int port);
    ~Client();
    void run();

   private:
    int sock;
    int clientId;  // Store the client ID received from the server
    struct sockaddr_in servAddr;
    std::vector<Position> positions;
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Texture * backgroundTexture;
    SDL_Texture * winTexture;
    SDL_Texture * loseTexture;
    SDL_Texture * carTexture;
    std::atomic<bool> raceOngoing;
    std::atomic<bool> isWinner;
    std::atomic<bool> keepRunning;
    std::thread serverThread;

    bool initSDL();
    bool loadTextures();
    void closeSDL();
    void handleServerMessages();
    void handleCountdown(const std::string & message);
    void handleRaceStatus(const std::string & message);
    void sendRequest(const std::string & request);
    void handleClientId(const std::string & message);

    void renderTrack(int x, int y);
    void renderBackground();
};

#endif  // CLIENT_HPP

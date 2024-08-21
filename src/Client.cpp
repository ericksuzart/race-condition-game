#include "Client.hpp"

Client::Client(const std::string & serverIp, int port)
: clientId(-1),
  positions(4, {0, 100}),
  window(nullptr),
  renderer(nullptr),
  backgroundTexture(nullptr),
  winTexture(nullptr),
  loseTexture(nullptr),
  carTexture(nullptr),  // Initialize car texture
  raceOngoing(true),
  isWinner(false),
  keepRunning(true)
{
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "Socket creation error" << std::endl;
        exit(EXIT_FAILURE);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, serverIp.c_str(), &servAddr.sin_addr) <= 0)
    {
        std::cerr << "Invalid address or Address not supported" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        std::cerr << "Connection failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!initSDL() || !loadTextures())
    {
        std::cerr << "Failed to initialize SDL or load textures" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Start server communication in a separate thread
    serverThread = std::thread(&Client::handleServerMessages, this);
}

Client::~Client()
{
    keepRunning = false;  // Signal the server thread to stop
    if (serverThread.joinable())
    {
        serverThread.join();
    }
    closeSDL();
    close(sock);
}

bool Client::initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
                  << std::endl;
        return false;
    }

    window =
      SDL_CreateWindow("Drag Race Client", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Window could not be created! SDL_Error: "
                  << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Renderer could not be created! SDL_Error: "
                  << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

bool Client::loadTextures()
{
    SDL_Surface * surface = SDL_LoadBMP("assets/background.bmp");
    if (!surface)
    {
        std::cerr << "Failed to load background image! SDL_Error: "
                  << SDL_GetError() << std::endl;
        return false;
    }
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = SDL_LoadBMP("assets/win.bmp");
    if (!surface)
    {
        std::cerr << "Failed to load win screen! SDL_Error: " << SDL_GetError()
                  << std::endl;
        return false;
    }
    winTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = SDL_LoadBMP("assets/lose.bmp");
    if (!surface)
    {
        std::cerr << "Failed to load lose screen! SDL_Error: "
                  << SDL_GetError() << std::endl;
        return false;
    }
    loseTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = SDL_LoadBMP("assets/car0.bmp");
    if (!surface)
    {
        std::cerr << "Failed to load car texture! SDL_Error: "
                  << SDL_GetError() << std::endl;
        return false;
    }
    carTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return true;
}

void Client::closeSDL()
{
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(winTexture);
    SDL_DestroyTexture(loseTexture);
    SDL_DestroyTexture(carTexture);  // Free car texture
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Client::handleServerMessages()
{
    char buffer[1024] = {0};

    while (keepRunning)
    {
        ssize_t readSize = read(sock, buffer, sizeof(buffer));

        if (readSize > 0)
        {
            std::string message(buffer, readSize);

            if (message.rfind("CLIENT_ID", 0) == 0)
            {
                handleClientId(message);
            }
            else if (message.rfind("COUNTDOWN", 0) == 0)
            {
                handleCountdown(message);
            }
            else if (message.rfind("WINNER", 0) == 0)
            {
                handleRaceStatus(message);
            }
            else
            {
                // Parse positions
                std::istringstream ss(message);
                for (auto & pos : positions)
                {
                    ss >> pos.x >> pos.y;
                }
            }
        }

        SDL_Delay(10);  // Small delay to prevent excessive CPU usage
    }
}

void Client::handleClientId(const std::string & message)
{
    clientId = std::stoi(message.substr(10));
    std::cout << "Assigned Client ID: " << clientId
              << std::endl;  // Log the assigned client ID for debugging
}

void Client::handleCountdown(const std::string & message)
{
    int countdown = std::stoi(message.substr(10));
    std::cout << "Countdown: " << countdown << std::endl;

    for (int i = countdown; i > 0; --i)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
        renderBackground();  // Render the background
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);  // 1 second per countdown step
    }

    if (countdown == 0)
    {
        std::cout << "GOO!!!" << std::endl;
    }
}

void Client::handleRaceStatus(const std::string & message)
{
    int winner = std::stoi(message.substr(7));
    if (winner == -1)
    {
        raceOngoing = true;
    }
    else
    {
        raceOngoing = false;
        isWinner = (winner == clientId);  // Compare the received winner ID
                                          // with the client's own ID
    }
}

void Client::sendRequest(const std::string & request)
{
    send(sock, request.c_str(), request.size(), 0);
}

void Client::run()
{
    bool running = true;
    SDL_Event e;

    while (running)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
            }
            if (raceOngoing && e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_RIGHT)
                {
                    sendRequest("MOVE_RIGHT");
                }
            }
        }

        SDL_RenderClear(renderer);

        if (raceOngoing)
        {
            // Render background and players during the race

            SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
            renderBackground();  // Render the background

            // Render car textures at each player's position
            for (const auto & pos : positions)
            {
                SDL_Rect carRect = {pos.x, pos.y, 100, 50};
                SDL_RenderCopy(renderer, carTexture, nullptr, &carRect);
            }
        }
        else
        {
            // Render win/lose screen after the race ends
            if (isWinner)
            {
                SDL_RenderCopy(renderer, winTexture, nullptr, nullptr);
            }
            else
            {
                SDL_RenderCopy(renderer, loseTexture, nullptr, nullptr);
            }
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(16);  // Cap to ~60 FPS
    }
}

void Client::renderTrack(int x, int y)
{
    // Define the lane divisor dimensions
    int laneDivisorWidth = 30;
    int laneDivisorHeight = 5;
    int laneDivisorSpacing = 60;
    int width = 800;
    int height = 120;
    int coordX = x;
    int coordY = y;
    SDL_Color BLACK = {0, 0, 0, 255};
    SDL_Color WHITE = {255, 255, 255, 255};
    SDL_Color GREEN = {0, 255, 0, 255};
    SDL_Color GRAY = {128, 128, 128, 255};

    // Set the draw color to black and draw the road
    SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
    SDL_Rect roadRect = {coordX, coordY, width, height};
    SDL_RenderFillRect(renderer, &roadRect);

    // Set the draw color to white and draw the lane divisors
    SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a);
    for (int x = coordX; x < coordX + width; x += laneDivisorSpacing)
    {
        SDL_Rect laneDivisorRect = {
          x, coordY + height / 2 - laneDivisorHeight / 2, laneDivisorWidth,
          laneDivisorHeight};
        SDL_RenderFillRect(renderer, &laneDivisorRect);
    }

    // Define the finish line dimensions and position
    int finishLineWidth = 20;
    int finishLineHeight = height;
    int finishLineX = coordX + width - finishLineWidth;
    int finishLineY = coordY;

    // Set the draw color to green and white alternately to draw the chessboard
    // pattern
    int squareSize = 5;  // Size of each square in the chessboard pattern
    for (int y = finishLineY; y < finishLineY + finishLineHeight;
         y += squareSize)
    {
        for (int x = finishLineX; x < finishLineX + finishLineWidth;
             x += squareSize)
        {
            if (((x / squareSize) + (y / squareSize)) % 2 == 0)
            {
                SDL_SetRenderDrawColor(renderer, GREEN.r, GREEN.g, GREEN.b,
                                       GREEN.a);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b,
                                       BLACK.a);
            }
            SDL_Rect squareRect = {x, y, squareSize, squareSize};
            SDL_RenderFillRect(renderer, &squareRect);
        }
    }

    // Define the white rectangles above and below the road
    int whiteRectHeight = 10;
    SDL_Rect topWhiteRect = {coordX, coordY - whiteRectHeight, width,
                             whiteRectHeight};
    SDL_Rect bottomWhiteRect = {coordX, coordY + height, width,
                                whiteRectHeight};

    // Set the draw color to white and draw the rectangles
    SDL_SetRenderDrawColor(renderer, GRAY.r, GRAY.g, GRAY.b, GRAY.a);
    SDL_RenderFillRect(renderer, &topWhiteRect);
    SDL_RenderFillRect(renderer, &bottomWhiteRect);
}

void Client::renderBackground()
{
    SDL_Color DARK_GRAY = {64, 64, 64, 255};
    SDL_Color GRAY = {128, 128, 128, 255};
    SDL_Color BLUE = {0, 0, 255, 255};
    SDL_Color RED = {255, 0, 0, 255};
    SDL_Color BROWN = {139, 69, 19, 255};
    SDL_Color TREE_GREEN = {0, 100, 0, 255};

    int width = 800;
    int height = 600;

    // Define the number of grandstands
    int numGrandstands = 4;
    int grandstandSpacing = 80;
    int grandstandWidth =
      (width - (numGrandstands + 1) * grandstandSpacing) / numGrandstands;
    int grandstandHeight = height / 10;

    for (int i = 0; i < numGrandstands; ++i)
    {
        int grandstandX =
          grandstandSpacing + i * (grandstandWidth + grandstandSpacing);
        int grandstandY = 10;

        // Set the draw color to dark gray and draw the grandstands
        SDL_SetRenderDrawColor(renderer, DARK_GRAY.r, DARK_GRAY.g, DARK_GRAY.b,
                               DARK_GRAY.a);
        SDL_Rect grandstandRect = {grandstandX, grandstandY, grandstandWidth,
                                   grandstandHeight};
        SDL_RenderFillRect(renderer, &grandstandRect);

        // Set draw to gray and draw the inner grandstand
        SDL_SetRenderDrawColor(renderer, GRAY.r, GRAY.g, GRAY.b, GRAY.a);
        SDL_Rect innerGrandstandRect = {grandstandX + 10, grandstandY + 10,
                                        grandstandWidth - 20,
                                        grandstandHeight - 20};
        SDL_RenderFillRect(renderer, &innerGrandstandRect);

        // Define the audience dimensions and position
        int audienceWidth = 5;
        int audienceHeight = 6;
        int audienceSpacing = 10;
        int audienceRows = (grandstandHeight - 20) / audienceSpacing;
        int audienceCols = (grandstandWidth - 20) / audienceSpacing;

        // Draw the audience in blue and red sparsely and randomly
        for (int row = 0; row < audienceRows; ++row)
        {
            for (int col = 0; col < audienceCols; ++col)
            {
                // Randomly decide whether to draw an audience member
                if (rand() % 2 == 0)  // 50% chance
                {
                    // Randomly choose the color (blue or red)
                    if (rand() % 2 == 0)
                        SDL_SetRenderDrawColor(renderer, BLUE.r, BLUE.g,
                                               BLUE.b, BLUE.a);
                    else
                        SDL_SetRenderDrawColor(renderer, RED.r, RED.g, RED.b,
                                               RED.a);

                    int audienceX = grandstandX + 12 + col * audienceSpacing;
                    int audienceY = grandstandY + 12 + row * audienceSpacing;
                    SDL_Rect audienceRect = {audienceX, audienceY,
                                             audienceWidth, audienceHeight};
                    SDL_RenderFillRect(renderer, &audienceRect);
                }
            }
        }
    }

    // Define the number of trees
    int numTrees = 10;
    int treeSpacing = (width - (numTrees + 1) * grandstandSpacing) / numTrees;
    int treeWidth = 20;
    int treeHeight = 60;
    int trunkWidth = 10;
    int trunkHeight = 20;

    for (int i = 0; i < numTrees; ++i)
    {
        int treeX = grandstandSpacing + i * (treeSpacing + grandstandSpacing);
        int treeY = height - treeHeight - 10;

        SDL_SetRenderDrawColor(renderer, BROWN.r, BROWN.g, BROWN.b, BROWN.a);
        SDL_Rect trunkRect = {treeX + (treeWidth - trunkWidth) / 2,
                              treeY + treeHeight - trunkHeight, trunkWidth,
                              trunkHeight};

        SDL_RenderFillRect(renderer, &trunkRect);

        // Set the draw color to green and draw the foliage
        SDL_SetRenderDrawColor(renderer, TREE_GREEN.r, TREE_GREEN.g,
                               TREE_GREEN.b, TREE_GREEN.a);
        SDL_Point foliage[3] = {
          {treeX, treeY + treeHeight - trunkHeight},
          {treeX + treeWidth / 2, treeY},
          {treeX + treeWidth, treeY + treeHeight - trunkHeight}};

        // Draw the filled triangle
        int minY = foliage[1].y;
        int maxY = foliage[0].y;
        for (int y = minY; y <= maxY; ++y)
        {
            int x1 = foliage[1].x + (y - foliage[1].y) *
                                      (foliage[0].x - foliage[1].x) /
                                      (foliage[0].y - foliage[1].y);
            int x2 = foliage[1].x + (y - foliage[1].y) *
                                      (foliage[2].x - foliage[1].x) /
                                      (foliage[2].y - foliage[1].y);
            if (x1 > x2) std::swap(x1, x2);
            SDL_RenderDrawLine(renderer, x1, y, x2, y);
        }
    }

    renderTrack(0, 100);
    renderTrack(0, 220);
}

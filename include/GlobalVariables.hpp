#ifndef GlobalVariables_HPP
#define GlobalVariables_HPP

constexpr int MOVE_SPEED = 5;
constexpr int MS_PER_FRAME = 16;  // ~60 FPS
constexpr int CAR_SIZE = 20;
constexpr int SCREEN_HEIGHT = 720;
constexpr int SCREEN_WIDTH = 1280;

constexpr SDL_Color RED = {200, 0, 0, 255};
constexpr SDL_Color BLUE = {0, 0, 200, 255};
constexpr SDL_Color GREEN = {0, 200, 0, 255};
constexpr SDL_Color YELLOW = {200, 200, 0, 255};
constexpr SDL_Color WHITE = {255, 255, 255, 255};
constexpr SDL_Color BLACK = {0, 0, 0, 255};
constexpr SDL_Color GRAY = {128, 128, 128, 255};
constexpr SDL_Color DARK_GRAY = {64, 64, 64, 255};

#endif  // GlobalVariables_HPP

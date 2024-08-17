#include "../include/Track.hpp"
#include <iostream>

Track::Track(int width, int height, int x, int y)
    : width(width), height(height), coordX(x), coordY(y)
{
}


void Track::draw(SDL_Renderer * renderer)
{
    // int coordX = 0;
    // int coordY = height / 2;

    // Define the lane divisor dimensions
    int laneDivisorWidth = 30;
    int laneDivisorHeight = 5;
    int laneDivisorSpacing = 60;

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

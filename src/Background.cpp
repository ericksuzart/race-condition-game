#include "../include/Background.hpp"

Background::Background(int width, int height) : width(width), height(height) {}

void Background::draw(SDL_Renderer * renderer)
{
    // Set the draw color to white and draw the background
    SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a);
    SDL_Rect backgroundRect = {0, 0, width, height};
    SDL_RenderFillRect(renderer, &backgroundRect);

    // Define the grandstand dimensions and position
    int grandstandWidth = width;
    int grandstandHeight = height / 10;
    int grandstandX = 0;
    int grandstandY = height / 10;

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
    int audienceRows = grandstandHeight / audienceSpacing;
    int audienceCols = grandstandWidth / audienceSpacing;
    int spacing = 1;

    // Draw the audience in blue and red sparsely and randomly
    for (int row = spacing; row < audienceRows - spacing; ++row)
    {
        for (int col = spacing; col < audienceCols - spacing; ++col)
        {
            // Randomly decide whether to draw an audience member
            if (rand() % 2 == 0)  // 50% chance
            {
                // Randomly choose the color (blue or red)
                if (rand() % 2 == 0)
                {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255,
                                           255);  // Blue color
                }
                else
                {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0,
                                           255);  // Red color
                }

                int audienceX = grandstandX + col * audienceSpacing;
                int audienceY = grandstandY + row * audienceSpacing;
                SDL_Rect audienceRect = {audienceX, audienceY, audienceWidth,
                                         audienceHeight};
                SDL_RenderFillRect(renderer, &audienceRect);
            }
        }
    }
}

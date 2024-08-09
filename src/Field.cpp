#include "../include/Field.hpp"

Field::Field(int width, int height) : width(width), height(height) {}

void Field::draw(SDL_Renderer * renderer)
{
    const int rec_width = width / 8;
    const int rec_height = height / 2;
    const int gol_width = SIDE_MARGIN;
    const int gol_height = height / 4;
    float radius = rec_height / 2;
    int centerX = width / 2 + SIDE_MARGIN;
    int centerY = height / 2 + TOP_MARGIN;

    // Cor de fundo
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Desenhar a pista de corrida

    // Cor da pista
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);

    // Desenhar o contorno externo da pista
    SDL_Rect outerTrack = {SIDE_MARGIN, TOP_MARGIN, width, height};
    SDL_RenderFillRect(renderer, &outerTrack);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &outerTrack);

    // Desenhar o contorno interno da pista
    int innerMargin = 50;  // Margem interna para a pista
    SDL_Rect innerTrack = {SIDE_MARGIN + innerMargin, TOP_MARGIN + innerMargin,
                           width - 2 * innerMargin, height - 2 * innerMargin};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &innerTrack);

    // Desenhar as linhas das faixas
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int numLanes = 3;
    int laneWidth = (width - 2 * innerMargin) / numLanes;
    for (int i = 1; i < numLanes; ++i)
    {
        int laneX = SIDE_MARGIN + innerMargin + i * laneWidth;
        SDL_RenderDrawLine(renderer, laneX, TOP_MARGIN + innerMargin, laneX,
                           TOP_MARGIN + height - innerMargin);
    }

    // Placar
    SDL_Rect scoreboard = {0, 0, SCREEN_WIDTH, TOP_MARGIN};
    SDL_SetRenderDrawColor(renderer, 225, 225, 225, 255);
    SDL_RenderFillRect(renderer, &scoreboard);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &scoreboard);
}

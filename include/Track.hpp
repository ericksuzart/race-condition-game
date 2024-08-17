#ifndef Track_H
#define Track_H

#include <SDL2/SDL.h>
#include <math.h>

#include "GlobalVariables.hpp"

class Track
{
   public:
    // Construtor para definir posição e dimensões
    Track(int width, int height, int x, int y);

    // Função para desenhar o campo no renderizador
    void draw(SDL_Renderer * renderer);

   private:
    int width;
    int height;
    int coordX;
    int coordY;
};

#endif  // Track_H

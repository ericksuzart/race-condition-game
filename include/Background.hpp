#ifndef Background_HPP
#define Background_HPP

#include <SDL2/SDL.h>
#include "GlobalVariables.hpp"

class Background
{
   public:
    // Construtor para definir dimensão
    Background(int width, int height);

    // Função para desenhar o plano de fundo no renderizador
    void draw(SDL_Renderer * renderer);

   private:
    int width;
    int height;
};

#endif  // Background_HPP

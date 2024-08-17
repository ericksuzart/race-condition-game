#include "../include/main.hpp"

using namespace std;

int mainFunction(JoyInterface & ji)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
                  << std::endl;
        return 1;
    }

    SDL_Window * window = SDL_CreateWindow(
      "Desafio OxeBots Equipe 1", SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        std::cerr << "A janela não pôde ser criada! SDL_Error: "
                  << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer * renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL)
    {
        std::cerr << "Não foi possível criar o renderizador! SDL_Error: "
                  << SDL_GetError() << std::endl;
        return 1;
    }

    Background background(SCREEN_WIDTH, SCREEN_HEIGHT);
    Track track_0(SCREEN_WIDTH, SCREEN_HEIGHT / 4, 0, SCREEN_HEIGHT / 4);
    Track track_1(SCREEN_WIDTH, SCREEN_HEIGHT / 4, 0, 2.5 * SCREEN_HEIGHT / 4);

    // Robot car_0(BLUE, SIDE_MARGIN + TRACK_WIDTH / 2 - 100,
    //              TRACK_HEIGHT / 2 + TOP_MARGIN, 0, 1, ji);
    // Robot car_1(GREEN, SIDE_MARGIN + TRACK_WIDTH / 2 + 100,
    //              TRACK_HEIGHT / 2 + TOP_MARGIN, M_2_PI + 2.5F, 2, ji);

    // Score Score(ball, {&car_0, &car_1});

    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        // Desenha o campo
        background.draw(renderer);
        track_0.draw(renderer);
        track_1.draw(renderer);
        // car_0.draw(renderer);
        // car_1.draw(renderer);
        // Score.updateScore();

        // car_0.move();
        // car_1.move();

        // colisions.checkColisions();

        // Atualizar a tela
        SDL_RenderPresent(renderer);

        // Atraso para manter a taxa de quadros constante em ~60 FPS
        SDL_Delay(MS_PER_FRAME);

        // Processa os eventos do SDL (tela principal)
        while (SDL_PollEvent(&e) != 0)
            if (e.type == SDL_QUIT) quit = true;

        // Processa os eventos do JoyInterface (controle remoto)
        while (ji.hasKeyEvent())
        {
            proto::KeyEvent key_event = ji.getKeyEvent();
            // car_0.setMove(key_event);
            // car_1.setMove(key_event);
        }
    }

    // Fechar os recursos
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

#undef main
int main()
{
    JoyInterface ji;

    std::thread JoyInterfaceThread(&JoyInterface::run, &ji);
    std::thread MainThread(&mainFunction, std::ref(ji));

    JoyInterfaceThread.join();
    MainThread.join();

    return 0;
}

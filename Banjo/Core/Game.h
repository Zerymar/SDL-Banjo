#pragma once
#include <SDL.h>
#include "MainPlayer.h"


class Game
{
public:
    Game();
    ~Game();
    bool init();
    void run();

protected:
    SDL_Window* m_pWindow;
    SDL_Renderer* m_PRenderer;
    MainPlayer* m_mainPlayer;
};

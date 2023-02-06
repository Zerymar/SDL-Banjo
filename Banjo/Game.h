#pragma once
#include <SDL.h>

class Game
{
public:
    Game();
    ~Game();
    bool init();
    void run();
    
protected:
    SDL_Window *m_pWindow;
    SDL_Renderer *m_PRenderer;
    
    
};

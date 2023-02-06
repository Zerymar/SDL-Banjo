#include "Game.h"
#include <iostream>
#include "defs.h"

Game::Game()
{
    m_pWindow = nullptr;
    m_PRenderer = nullptr;
}

Game::~Game()
{
    SDL_DestroyRenderer(m_PRenderer);
    SDL_DestroyWindow(m_pWindow);
    SDL_Quit();
}

bool Game::init()
{
    int rendererFlags, windowFlags;

    // Tell renderer to use hardware acceleration
    rendererFlags = SDL_RENDERER_ACCELERATED;
    windowFlags = SDL_WINDOW_RESIZABLE;

    // initialize SDL2's video subsystem
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    m_pWindow = SDL_CreateWindow("Banjo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if(m_pWindow == nullptr)
    {
        std::cout << "Failed to open " <<SCREEN_WIDTH << " x " <<  SCREEN_HEIGHT << " window: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    m_PRenderer = SDL_CreateRenderer(m_pWindow, -1, rendererFlags);
    if(m_PRenderer == nullptr)
    {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void Game::run()
{
    bool bQuit = false;
    SDL_Event event;

    while(!bQuit)
    {
        while(SDL_PollEvent(&event) != 0)
        {
            if(event.type == SDL_QUIT)
            {
                bQuit = true;
            }
        }

        SDL_SetRenderDrawColor(m_PRenderer, 150, 0, 150, 255);
        SDL_RenderClear(m_PRenderer);
        SDL_RenderPresent(m_PRenderer);
    }
}

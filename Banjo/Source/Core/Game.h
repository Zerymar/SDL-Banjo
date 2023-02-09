#pragma once
#include <SDL.h>

#include "Coordinator.hpp"
#include "Systems/PhysicsSystem.h"
#include "Systems/PlayerInputSystem.h"
#include "Systems/RenderSystem.h"


class Game
{
public:
    Game();
    ~Game();
    bool init();
    void run();
    void PlayerInit();

protected:
    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;

    std::shared_ptr<RenderSystem> m_RenderSystem;
    std::shared_ptr<PhysicsSystem> m_PhysicsSystem;
    std::shared_ptr<PlayerInputSystem> m_PISystem;
};

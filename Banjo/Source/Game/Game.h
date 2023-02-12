#pragma once
#include <SDL.h>

#include "../Core/Coordinator.hpp"
#include "../Game/Systems/AsteroidSystem.h"
#include "../Core/Systems/PhysicsSystem.h"
#include "../Core/Systems/PlayerInputSystem.h"
#include "../Core/Systems/RenderSystem.h"


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
    std::vector<SDL_Point> m_points;
    std::vector<SDL_Point> m_convexHull;
    
    std::shared_ptr<RenderSystem> m_RenderSystem;
    std::shared_ptr<PhysicsSystem> m_PhysicsSystem;
    std::shared_ptr<AsteroidSystem> m_AsteroidSystem;
    std::shared_ptr<PlayerInputSystem> m_PISystem;

    Vector3 m_EntityColor;
    

};

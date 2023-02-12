#pragma once
#include <SDL.h>

#include "Coordinator.hpp"
#include "Systems/PhysicsSystem.h"
#include "Systems/PlayerInputSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/ScreenBoundsSystem.h"


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
    std::shared_ptr<PlayerInputSystem> m_PISystem;
    std::shared_ptr<ScreenBoundsSystem> m_ScreenBoundsSystem;

    Vector3 m_EntityColor;
    
    void GenerateAsteroids(const int& maxAsteroids);
};

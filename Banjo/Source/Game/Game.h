#pragma once
#include <SDL.h>
#include <SDL_mixer.h>

#include "../Core/Coordinator.hpp"
#include "../Game/Systems/CollisionSystem.h"
#include "../Game/Systems/AsteroidSystem.h"
#include "../Core/Systems/PhysicsSystem.h"
#include "../Game/Systems/PlayerInputSystem.h"
#include "../Core/Systems/RenderSystem.h"
#include "../Core/Systems/AudioSystem.h"
#include "Systems/ScoreSystem.h"


class Game
{
public:
    Game();
    ~Game();
    bool init();
    void run();
    void PlayerInit();

protected:
    bool LoadMedia();
    
    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;
    std::vector<SDL_FPoint> m_points;
    std::vector<SDL_FPoint> m_convexHull;
    
    std::shared_ptr<RenderSystem> m_RenderSystem;
    std::shared_ptr<PhysicsSystem> m_PhysicsSystem;
    std::shared_ptr<AsteroidSystem> m_AsteroidSystem;
    std::shared_ptr<CollisionSystem> m_CollisionSystem;
    std::shared_ptr<ScoreSystem> m_ScoreSystem;
    std::shared_ptr<AudioSystem> m_AudioSystem;
    std::shared_ptr<PlayerInputSystem> m_PISystem;

    std::vector<Mix_Chunk*> m_AsteroidExplosions;
    std::vector<Mix_Chunk*> m_LaserExplosions;
    std::vector<Mix_Chunk*> m_PlayerExplosions;

    Vector3 m_EntityColor;
    Mix_Music * m_BGM;

};

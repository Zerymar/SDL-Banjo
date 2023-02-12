#include "Game.h"
#include <iostream>
#include <random>

#include "../Components/Gravity.hpp"
#include "../Components/RigidBody.hpp"
#include "../Components/Transform.hpp"
#include "../Components/Player.hpp"
#include "..//Components/Projectile.hpp"
#include "../Components/Asteroid.hpp"
#include "../Utility/defs.h"
#include "../Utility/Math/Vector2.h"
#include "../Utility/Math/Geometry.hpp"
#include "../Core/Systems/PhysicsSystem.h"
#include "../Game/Systems/PlayerInputSystem.h"
#include "../Core/Systems/RenderSystem.h"
#include "../Game/Systems/AsteroidSystem.h"

extern Coordinator m_Coordinator;
Game::Game()
{
    m_pWindow = nullptr;
    m_pRenderer = nullptr;
}
SDL_FPoint Geometry::point0;
Game::~Game()
{
    SDL_DestroyRenderer(m_pRenderer);
    SDL_DestroyWindow(m_pWindow);
    SDL_Quit();
}

bool Game::init()
{
    int rendererFlags, windowFlags;

    m_EntityColor.x = 255;
    m_EntityColor.y = 255;
    m_EntityColor.z = 255;
    // Tell renderer to use hardware acceleration
    rendererFlags = SDL_RENDERER_ACCELERATED;
    windowFlags = SDL_WINDOW_RESIZABLE;

    // initialize SDL2's video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    m_pWindow = SDL_CreateWindow("Banjo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
                                 SDL_WINDOW_SHOWN);

    if (m_pWindow == nullptr)
    {
        std::cout << "Failed to open " << SCREEN_WIDTH << " x " << SCREEN_HEIGHT << " window: " << SDL_GetError() <<
            std::endl;
        return false;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, rendererFlags);
    if (m_pRenderer == nullptr)
    {
        std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    // initialize our components systems
    m_Coordinator.init();
    m_Coordinator.RegisterComponent<Gravity>();
    m_Coordinator.RegisterComponent<Transform>();
    m_Coordinator.RegisterComponent<RigidBody>();
    m_Coordinator.RegisterComponent<BasicShape>();
    m_Coordinator.RegisterComponent<Player>();
    m_Coordinator.RegisterComponent<Projectile>();
    m_Coordinator.RegisterComponent<Asteroid>();

    m_PhysicsSystem = m_Coordinator.RegisterSystem<class PhysicsSystem>();
    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<Gravity>());
        signature.set(m_Coordinator.GetComponentType<RigidBody>());
        signature.set(m_Coordinator.GetComponentType<Transform>());
        signature.set(m_Coordinator.GetComponentType<BasicShape>());
        m_Coordinator.SetSystemSignature<PhysicsSystem>(signature);
    }
    
    m_PISystem = m_Coordinator.RegisterSystem<class PlayerInputSystem>();
    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<Player>());
        signature.set(m_Coordinator.GetComponentType<Transform>());
        m_Coordinator.SetSystemSignature<PlayerInputSystem>(signature);
    }
    
    m_RenderSystem = m_Coordinator.RegisterSystem<class RenderSystem>();
    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<BasicShape>());
        m_Coordinator.SetSystemSignature<RenderSystem>(signature);
    }
    
    m_AsteroidSystem = m_Coordinator.RegisterSystem<class AsteroidSystem>();
    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<Asteroid>());
        m_Coordinator.SetSystemSignature<AsteroidSystem>(signature);
    }

    m_CollisionSystem = m_Coordinator.RegisterSystem<class CollisionSystem>();
    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<Transform>());
        signature.set(m_Coordinator.GetComponentType<BasicShape>());
        m_Coordinator.SetSystemSignature<CollisionSystem>(signature);
    }

    m_PhysicsSystem->Init();
    m_PISystem->Init();
    m_RenderSystem->Init();
    m_AsteroidSystem->Init();
    m_CollisionSystem->Init();

    //create our entity vector
    std::vector<Entity> entities(MAX_ENTITIES-1);
    
    PlayerInit();
    return true;
}

//initialize player character
void Game::PlayerInit()
{
    float triangleTipX = 65.0f;
    float triangleTipY = 25.0f;
    float r=255.0f,g=255.0f,b=255.0f;
    // Initialize our player "Entity"
    std::vector<SDL_FPoint> player_vertices;
    SDL_FPoint firstVertex;
    firstVertex.x = 0;
    firstVertex.y = 0;
    SDL_FPoint secondVertex;
    secondVertex.x = 0;
    secondVertex.y = 50;
    SDL_FPoint thirdVertex;
    thirdVertex.x = triangleTipX;
    thirdVertex.y = triangleTipY;
    Vector3 ColorWhite(r,g,b);
    
    player_vertices.push_back(firstVertex); 
    player_vertices.push_back(secondVertex); 
    player_vertices.push_back(thirdVertex); 

    float MIDDLE_X = SCREEN_WIDTH /2.0f;
    float MIDDLE_Y = SCREEN_HEIGHT /2.0f;
    
    Entity playerEntity = m_Coordinator.CreateEntity();
    m_Coordinator.AddComponent<Player>(playerEntity, {player_vertices[2]});
    m_Coordinator.AddComponent<Gravity>(playerEntity,{Vector2(0.f, 0.f)});
    m_Coordinator.AddComponent<RigidBody>(playerEntity, {Vector2(0.f, 0.f),  Vector2(0.f, 0.f), Vector2(0.f, 0.f)});
    m_Coordinator.AddComponent<Transform>(playerEntity, {Vector2(MIDDLE_X, MIDDLE_Y),  Vector2(1.0f, 1.0f), Vector2(0.f,0.f)});
    m_Coordinator.AddComponent<BasicShape>(playerEntity, {player_vertices,  ColorWhite});
    
}

void Game::run()
{
    bool bQuit = false;
    SDL_Event event;

    Vector3 convexColor;
    convexColor.x=0;
    convexColor.y=255;
    convexColor.z=255; 
    float deltaTime = 0.0f;
    
    while (!bQuit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                bQuit = true;
            }
            m_PISystem->HandleInput(event);
        }

        if(!m_Coordinator.IsPaused())
        {
            m_PISystem->Update();
            m_PhysicsSystem->Update(deltaTime);
            m_AsteroidSystem->Update();
            m_CollisionSystem->Update();
            SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(m_pRenderer);
            m_RenderSystem->RenderEntities(m_pRenderer,m_EntityColor);
            m_RenderSystem->RenderPoints(m_pRenderer,m_points,  m_EntityColor);
            m_RenderSystem->RenderLines(m_pRenderer, m_Coordinator.m_ConvexHull, convexColor);
            SDL_RenderPresent(m_pRenderer);
        }

        SDL_Delay(15);

    }
}

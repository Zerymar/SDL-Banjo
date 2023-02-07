#include "Game.h"
#include <iostream>

#include "../Components/Gravity.hpp"
#include "../Components/RigidBody.hpp"
#include "../Components/Transform.hpp"
#include "../Components/BasicShape.hpp"
#include "../Components/Player.hpp"
#include "../Utility/defs.h"
#include "../Utility/Math/Vector2.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/PlayerInputSystem.h"
#include "Systems/RenderSystem.h"


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

    m_PRenderer = SDL_CreateRenderer(m_pWindow, -1, rendererFlags);
    if (m_PRenderer == nullptr)
    {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        return false;
    }

    // initialize our components systems
    m_Coordinator.init();
    m_Coordinator.RegisterComponent<Gravity>();
    m_Coordinator.RegisterComponent<Transform>();
    m_Coordinator.RegisterComponent<RigidBody>();
    m_Coordinator.RegisterComponent<BasicShape>();
    m_Coordinator.RegisterComponent<Player>();

    m_PhysicsSystem = m_Coordinator.RegisterSystem<class PhysicsSystem>();
    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<Gravity>());
        signature.set(m_Coordinator.GetComponentType<RigidBody>());
        signature.set(m_Coordinator.GetComponentType<Transform>());
        m_Coordinator.SetSystemSignature<PhysicsSystem>(signature);
    }

    m_PhysicsSystem->Init();

    m_PISystem = m_Coordinator.RegisterSystem<class PlayerInputSystem>();
    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<Player>());
        signature.set(m_Coordinator.GetComponentType<Transform>());
        m_Coordinator.SetSystemSignature<PlayerInputSystem>(signature);
    }
    m_PISystem->Init();

    m_RenderSystem = m_Coordinator.RegisterSystem<class RenderSystem>();
    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<BasicShape>());
        m_Coordinator.SetSystemSignature<RenderSystem>(signature);
    }
    m_RenderSystem->Init();
    //create our entity vector
    std::vector<Entity> entities(MAX_ENTITIES-1);
    
    // Initialize our player "Entity"
    std::vector<SDL_Point> vertices;
    SDL_Point firstVertex;
    firstVertex.x = 0;
    firstVertex.y = 0;
    SDL_Point secondVertex;
    secondVertex.x = 25;
    secondVertex.y = 25;
    SDL_Point thirdVertex;
    thirdVertex.x = 50;
    thirdVertex.y = 0;
    Vector3 ColorRed(255,0,0);
    
    vertices.push_back(firstVertex);
    vertices.push_back(secondVertex);
    vertices.push_back(thirdVertex);
    vertices.push_back(firstVertex);

    playerEntity = m_Coordinator.CreateEntity();
    m_Coordinator.AddComponent(playerEntity, Player{});
    m_Coordinator.AddComponent<Gravity>(playerEntity,{Vector2(0.0f, 0.0f)});
    m_Coordinator.AddComponent<RigidBody>(playerEntity, {Vector2(0.0f, 0.0f),  Vector2(0.0f, 0.0f)});
    m_Coordinator.AddComponent<Transform>(playerEntity, {Vector2(25.0f, 25.0f),  Vector2(1.0f, 1.0f)});
    m_Coordinator.AddComponent<BasicShape>(playerEntity, {vertices,  ColorRed});

  
    
    return true;
}

void Game::run()
{
    bool bQuit = false;
    SDL_Event event;

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

        m_PISystem->Update();
        m_PhysicsSystem->Update(deltaTime);
        m_RenderSystem->Render(m_PRenderer);

        SDL_Delay(15);
    }
}

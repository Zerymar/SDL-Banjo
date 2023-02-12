#include "Game.h"
#include <iostream>
#include <random>

#include "../Components/Gravity.hpp"
#include "../Components/RigidBody.hpp"
#include "../Components/Transform.hpp"
#include "../Components/BasicShape.hpp"
#include "../Components/Player.hpp"
#include "../Components/Projectile.hpp"
#include "../Utility/defs.h"
#include "../Utility/Math/Vector2.h"
#include "../Utility/Math/Geometry.hpp"
#include "Systems/PhysicsSystem.h"
#include "Systems/PlayerInputSystem.h"
#include "Systems/RenderSystem.h"

extern Coordinator m_Coordinator;
Game::Game()
{
    m_pWindow = nullptr;
    m_pRenderer = nullptr;
}
SDL_Point Geometry::point0;
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
    
    PlayerInit();
    GeneratePoints();
    //
    return true;
}

//initialize player character
void Game::PlayerInit()
{
    int triangleTipX = 65;
    int triangleTipY = 25;
    int r=255,g=255,b=255;
    // Initialize our player "Entity"
    std::vector<SDL_Point> player_vertices;
    SDL_Point firstVertex;
    firstVertex.x = 0;
    firstVertex.y = 0;
    SDL_Point secondVertex;
    secondVertex.x = 0;
    secondVertex.y = 50;
    SDL_Point thirdVertex;
    thirdVertex.x = triangleTipX;
    thirdVertex.y = triangleTipY;
    Vector3 ColorWhite(r,g,b);
    
    player_vertices.push_back(firstVertex); 
    player_vertices.push_back(secondVertex); 
    player_vertices.push_back(thirdVertex); 
    player_vertices.push_back(firstVertex); 


    
    Entity playerEntity = m_Coordinator.CreateEntity();
    m_Coordinator.AddComponent<Player>(playerEntity, {player_vertices[2]});
    m_Coordinator.AddComponent<Gravity>(playerEntity,{Vector2(0, 0)});
    m_Coordinator.AddComponent<RigidBody>(playerEntity, {Vector2(0, 0),  Vector2(0, 0)});
    m_Coordinator.AddComponent<Transform>(playerEntity, {Vector2(25, 25),  Vector2(1, 1), Vector2(0,0)});
    m_Coordinator.AddComponent<BasicShape>(playerEntity, {player_vertices,  ColorWhite});


    
}
void Game::GeneratePoints()
{
    // draw a random set of points
    //
    int shapeWidth = 700;
    int shapeHeight = 700;

    int shapeXOrigin = SCREEN_WIDTH/2;
    int shapeYOrigin = SCREEN_HEIGHT/2;
    
    int MAX_POINTS =90;
    std::random_device rd; // obtain random # from hardware
    std::mt19937 gen(rd()); // seed the generator, A Mersenne Twister pseudo-random generator of 32-bit numbers with a state size of 19937 bits

    // use our max points as padding for easy debug
    std::uniform_int_distribution<> x_distr(0 , shapeWidth);
    std::uniform_int_distribution<> y_distr(0, shapeHeight);
    
    for(int i = 0; i <= MAX_POINTS;++i)
    {
        int randX = x_distr(gen);
        int randY = y_distr(gen);
        SDL_Point randPoint;
        randPoint.x =randX;
        randPoint.y =randY;

        m_points.push_back(randPoint);
    }

    
    Geometry::ConvexHull(m_convexHull, m_points);
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
        m_PISystem->Update();
        m_PhysicsSystem->Update(deltaTime);
        
        SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(m_pRenderer);
        m_RenderSystem->RenderEntities(m_pRenderer,m_EntityColor);
        m_RenderSystem->RenderPoints(m_pRenderer,m_points,  m_EntityColor);
        m_RenderSystem->RenderLines(m_pRenderer, m_convexHull, convexColor);
        SDL_RenderPresent(m_pRenderer);
        SDL_Delay(15);
    }
}

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
    //GeneratePoints();
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
    
    Entity playerEntity = m_Coordinator.CreateEntity();
    m_Coordinator.AddComponent<Player>(playerEntity, {player_vertices[2]});
    m_Coordinator.AddComponent<Gravity>(playerEntity,{Vector2(0, 0)});
    m_Coordinator.AddComponent<RigidBody>(playerEntity, {Vector2(0, 0),  Vector2(0, 0)});
    m_Coordinator.AddComponent<Transform>(playerEntity, {Vector2(25, 25),  Vector2(1, 1), Vector2(0,0)});
    m_Coordinator.AddComponent<BasicShape>(playerEntity, {player_vertices,  ColorWhite});
    
}

void Game::GenerateAsteroids(const int& minAsteroids)
{
    std::random_device rd; // obtain random # from hardware
    std::mt19937 gen(rd()); // seed the generator, A Mersenne Twister pseudo-random generator of 32-bit numbers with a state size of 19937 bits

    int r=255,g=255,b=255;
    Vector3 ColorWhite(r,g,b);
    
    // use our max points as padding for easy debug
    std::uniform_int_distribution<> width_distr(-10 , MAX_ASTEROID_WIDTH);
    std::uniform_int_distribution<> height_distr(-10, MAX_ASTEROID_HEIGHT);

    for(int i = 0; i < MAX_ASTEROIDS; ++i)
    {
        std::vector<SDL_Point> randomVertices;
        std::vector<SDL_Point> asteroidVertices;
        // Generate out Asteroid 
        for(int j = 0; j <= ASTEROID_VERTICES; ++j)
        {
            int randX = width_distr(gen);
            int randY = height_distr(gen);
            SDL_Point randPoint;
            randPoint.x =randX + MIN_ASTEROID_WIDTH;
            randPoint.y =randY + MIN_ASTEROID_HEIGHT;
            randomVertices.push_back(randPoint);
        }
        Geometry::ConvexHull(asteroidVertices, randomVertices);

        
        // dist randomly based on either under/before our min screen or after
        const bool beforeAfter = rand() % 2;
        const int xOffScreenBuffer = beforeAfter ? OFFSCREEN_SPAWN_BUFFER *-1 : SCREEN_WIDTH+OFFSCREEN_SPAWN_BUFFER ; // Our buffer for position off screen
        const int x_min = beforeAfter ? xOffScreenBuffer : SCREEN_WIDTH;
        const int x_max = beforeAfter ?  0 : xOffScreenBuffer ;

        const int YOffScreenBuffer = beforeAfter ? OFFSCREEN_SPAWN_BUFFER *-1 : SCREEN_HEIGHT+OFFSCREEN_SPAWN_BUFFER ; // Our buffer for position off screen
        const int Y_min = beforeAfter ? YOffScreenBuffer : SCREEN_HEIGHT;
        const int Y_max = beforeAfter ?  0 : YOffScreenBuffer ;
        
        std::uniform_int_distribution<> x_spawn_dist(x_min , x_max);
        std::uniform_int_distribution<> y_spawn_dist(Y_min, Y_max);
        Vector2 asteroidPosition;
        asteroidPosition.x = width_distr(gen);
        asteroidPosition.y = height_distr(gen);
        
        Vector2 asteroidVelocity;
        // Make our way to center of screen
        int xVelocity = SCREEN_WIDTH/2 - asteroidPosition.x > 0 ? 1 : -1;
        int yVelocity = SCREEN_HEIGHT/2 - asteroidPosition.y > 0 ? 1 : -1;
        asteroidVelocity.x = xVelocity ;
        asteroidVelocity.y = yVelocity;

        //Inc speed if necessary
        asteroidVelocity *= 1;
        
        Entity asteroidEntity = m_Coordinator.CreateEntity();
        m_Coordinator.AddComponent<Gravity>(asteroidEntity,{Vector2(0, 0)});
        m_Coordinator.AddComponent<RigidBody>(asteroidEntity, {asteroidVelocity,  Vector2(0, 0)});
        m_Coordinator.AddComponent<Transform>(asteroidEntity, {asteroidPosition,  Vector2(1, 1), Vector2(0,0)});
        m_Coordinator.AddComponent<BasicShape>(asteroidEntity, {asteroidVertices,  ColorWhite});
    }
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

    GenerateAsteroids(1);
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
        m_RenderSystem->RenderLines(m_pRenderer, m_Coordinator.m_ConvexHull, convexColor);
        SDL_RenderPresent(m_pRenderer);
        SDL_Delay(15);
    }
}

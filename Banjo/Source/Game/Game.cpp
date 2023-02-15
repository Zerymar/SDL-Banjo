#include "Game.h"
#include <iostream>
#include <random>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>

#include "../Components/Gravity.hpp"
#include "../Components/RigidBody.hpp"
#include "../Components/Transform.hpp"
#include "../Components/Player.hpp"
#include "../Components/Projectile.hpp"
#include "../Components/Asteroid.hpp"
#include "../Components/SFX.hpp"
#include "../Components/ScorePoints.hpp"

#include "../Utility/Math/Vector2.h"
#include "../Utility/Util.hpp"

#include "../Game/Systems/PlayerInputSystem.h"
#include "../Game/Systems/AsteroidSystem.h"

#include "../Core/Systems/RenderSystem.h"
#include "../Core/Systems/PhysicsSystem.h"


extern Coordinator m_Coordinator;
Game::Game()
{
    m_pWindow = nullptr;
    m_pRenderer = nullptr;
}

//Need to keep track of Point0 for our convex hulls generator, see Geometry.hpp
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
    // Tell renderer to use hardware acceleration
    rendererFlags = SDL_RENDERER_ACCELERATED;
    windowFlags = SDL_WINDOW_RESIZABLE;

    // initialize SDL2's video subsystem
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
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

    // Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if(! (IMG_Init(imgFlags) & imgFlags))
    {
        std::cout << "Couldn't initialize SDL_Image: " << SDL_GetError() << std::endl;
        return false;
    }

    //initialize mixer
    if( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "Couldn't initialize SDL_Mixer: " << SDL_GetError() << std::endl;
        return false;
    }
    
    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, rendererFlags);
    if (m_pRenderer == nullptr)
    {
        std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return false;
    }
    // Attempt to load media before anything else
    if(!LoadMedia())
    {
        std::cout << "Failed to load media: " << SDL_GetError() << std::endl;
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
    m_Coordinator.RegisterComponent<SFX>();
    m_Coordinator.RegisterComponent<ScorePoints>();

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

    m_AudioSystem = m_Coordinator.RegisterSystem<class AudioSystem>();
    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<SFX>());
        m_Coordinator.SetSystemSignature<AudioSystem>(signature);
    }
    
    m_ScoreSystem = m_Coordinator.RegisterSystem<class ScoreSystem>();
    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<ScorePoints>());
        m_Coordinator.SetSystemSignature<ScoreSystem>(signature);
    }

    m_ScoreSystem->Init();
    m_PhysicsSystem->Init();
    m_PISystem->Init(m_LaserExplosions);
    m_RenderSystem->Init();
    m_AsteroidSystem->Init(m_AsteroidExplosions);
    m_CollisionSystem->Init();

    //m_Coordinator.SetScoreSystem(m_ScoreSystem);
    //create our entity vector
    std::vector<Entity> entities(MAX_ENTITIES-1);
    
    PlayerInit();
    return true;
}

//initialize player character
void Game::PlayerInit()
{
    const float triangleTipX = 65.0f;
    const float triangleTipY = 25.0f;
    const float r=0.0f;
    const float g=255.0f;
    const float b=0.0f;
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
    
    Vector3 Color(r,g,b);
    
    player_vertices.push_back(firstVertex); 
    player_vertices.push_back(secondVertex); 
    player_vertices.push_back(thirdVertex); 

    float MIDDLE_X = SCREEN_WIDTH /2.0f;
    float MIDDLE_Y = SCREEN_HEIGHT /2.0f;

    Mix_Chunk* explosion = Util::Random_Element(m_PlayerExplosions);
    
    Entity playerEntity = m_Coordinator.CreateEntity();
    m_Coordinator.AddComponent<Player>(playerEntity, {player_vertices[2]});
    m_Coordinator.AddComponent<Gravity>(playerEntity,{Vector2(0.f, 0.f)});
    m_Coordinator.AddComponent<RigidBody>(playerEntity, {Vector2(0.f, 0.f),  Vector2(0.f, 0.f), Vector2(0.f, 0.f)});
    m_Coordinator.AddComponent<Transform>(playerEntity, {Vector2(MIDDLE_X, MIDDLE_Y),  Vector2(1.0f, 1.0f), Vector2(0.f,0.f)});
    m_Coordinator.AddComponent<BasicShape>(playerEntity, {player_vertices,  Color});
    m_Coordinator.AddComponent<SFX>(playerEntity, {nullptr, explosion});
}

bool Game::LoadMedia()
{
    // Music
    std::string bgmPath = "G:/GitHub/Zerymar/Banjo/SDL-Banjo/Banjo/Source/Resources/Media/Audio/Music/maingame_bgm.wav";
    m_BGM = Mix_LoadMUS(bgmPath.c_str());
    if(m_BGM == nullptr)
    {
        std::cout << "Failed to load file " << bgmPath << std::endl;
        return false;
    }

    
    // Asteroids
    for(int i  = 1; i <= ASTEROID_SFX_COUNT; i++)
    {
        std::string filePath = "G:/GitHub/Zerymar/Banjo/SDL-Banjo/Banjo/Source/Resources/Media/Audio/SFX/Asteroid/explosion0" + std::to_string(i) + ".wav";
        Mix_Chunk* explosionSound = Mix_LoadWAV(filePath.c_str());
        if(explosionSound == nullptr)
        {
            std::cout << "Failed to load file " << filePath << std::endl;
            return false;
        }
        m_AsteroidExplosions.push_back(explosionSound);
    }

    // Laser
    for(int i  = 1; i <= LASER_SFX_COUNT; i++)
    {
        
        std::string filePath = "G:/GitHub/Zerymar/Banjo/SDL-Banjo/Banjo/Source/Resources/Media/Audio/SFX/Laser/laser0" + std::to_string(i) + ".wav";
        Mix_Chunk* laserSound = Mix_LoadWAV(filePath.c_str());
        if(laserSound == nullptr)
        {
            std::cout << "Failed to load file " << filePath << std::endl;
            return false;
        }
        m_LaserExplosions.push_back(laserSound);
    }

    //Player death
    for(int i  = 1; i <= PLAYER_EXPLOSION_SFX_COUNT; i++)
    {
        std::string filePath = "G:/GitHub/Zerymar/Banjo/SDL-Banjo/Banjo/Source/Resources/Media/Audio/SFX/Player/p_explosion0" + std::to_string(i) + ".wav";
        Mix_Chunk* playerExplosion = Mix_LoadWAV(filePath.c_str());
        if(playerExplosion == nullptr)
        {
            std::cout << "Failed to load file " << filePath << std::endl;
            return false;
        }
        m_PlayerExplosions.push_back(playerExplosion);
    }
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

        if(Mix_PlayingMusic() == 0)
        {
            Mix_PlayMusic(m_BGM, -1);
        }

        if(m_Coordinator.ShouldRestart())
        {
            // delete all our entities and redo our player init
            m_Coordinator.ToggleRestartFlag();
            m_Coordinator.DestroyAllEntities();
            PlayerInit();
        }
        
        if(!m_Coordinator.IsPaused())
        {
            m_PISystem->Update();
            m_PhysicsSystem->Update(deltaTime);
            m_AsteroidSystem->Update();
            m_CollisionSystem->Update();
            m_AudioSystem->Update();
            SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(m_pRenderer);
            m_RenderSystem->RenderEntities(m_pRenderer);
            SDL_RenderPresent(m_pRenderer);
        }
        SDL_Delay(15);
    }
}

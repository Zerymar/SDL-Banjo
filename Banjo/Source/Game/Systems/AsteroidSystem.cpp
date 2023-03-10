#include "AsteroidSystem.h"
#include <random>

#include "../../Components/Asteroid.hpp"
#include "../../Utility/defs.h"
#include "../../Utility/Util.hpp"

#include "../../Components/Gravity.hpp"
#include "../../Components/RigidBody.hpp"
#include "../../Components/Transform.hpp"
#include "../../Components/BasicShape.hpp"
#include "../../Components/ScorePoints.hpp"
#include "../../Components/SFX.hpp"

void AsteroidSystem::Init(const std::vector<Mix_Chunk*>& asteroidExplosions)
{
    m_asteroidExplosions = asteroidExplosions;
}

void AsteroidSystem::OnEntityDelete(Entity entity) 
{
    System::OnEntityDelete(entity);
    if(m_ActiveAsteroids.find(entity) != m_ActiveAsteroids.end())
    {
        m_ActiveAsteroids.erase(entity);
    }
    
}

void AsteroidSystem::Update()
{
    if(m_ActiveAsteroids.size() < MAX_ASTEROIDS)
    {
        CreateAsteroid();
    }
}
void AsteroidSystem::GenerateAsteroids()
{
    for(int i = 0; i < MAX_ASTEROIDS; ++i)
    {
        CreateAsteroid();
    }
}

void AsteroidSystem::CreateAsteroid()
{
    std::vector<SDL_FPoint> asteroidVertices;
    CreateOutline(asteroidVertices);
    // dist randomly based on either under/before our min screen or after
    Vector2 asteroidPosition;
    GetRandomOffScreen(asteroidPosition);
        
    Vector2 asteroidVelocity;
    GenerateVelocity(asteroidVelocity, asteroidPosition);
        
    //Inc speed if necessary
    asteroidVelocity *= (rand() % ASTEROID_VELOCITY + 2);
    CreateAsteroidEntity(asteroidPosition, asteroidVelocity, asteroidVertices);
}

void AsteroidSystem::CreateOutline(std::vector<SDL_FPoint>& asteroidVertices)
{
    std::random_device rd; // obtain random # from hardware
    std::mt19937 gen(rd()); // seed the generator, A Mersenne Twister pseudo-random generator of 32-bit numbers with a state size of 19937 bits

    std::uniform_int_distribution<> width_distr(-10 , MAX_ASTEROID_WIDTH);
    std::uniform_int_distribution<> height_distr(-10, MAX_ASTEROID_HEIGHT);
    
    std::vector<SDL_FPoint> randomVertices;
    // Generate out Asteroid 
    for(int j = 0; j <= ASTEROID_VERTICES; ++j)
    {
        float randX = width_distr(gen);
        float randY = height_distr(gen);
        SDL_FPoint randPoint;
        randPoint.x =randX + MIN_ASTEROID_WIDTH;
        randPoint.y =randY + MIN_ASTEROID_HEIGHT;
        randomVertices.push_back(randPoint);
    }
    Geometry::ConvexHull(asteroidVertices, randomVertices);
}

// Go towards center of screen
void AsteroidSystem::GenerateVelocity(Vector2& asteroidVelocity, const Vector2& asteroidPosition)
{
    // Make our way to center of screen
    int xVelocity = SCREEN_WIDTH/2 - asteroidPosition.x > 0 ? 1 : -1;
    int yVelocity = SCREEN_HEIGHT/2 - asteroidPosition.y > 0 ? 1 : -1;
    asteroidVelocity.x = xVelocity ;
    asteroidVelocity.y = yVelocity;
}

void AsteroidSystem::CreateAsteroidEntity(Vector2 asteroidPosition, Vector2 asteroidVelocity, std::vector<SDL_FPoint> asteroidVertices)
{
    //Choose random explosion
    Mix_Chunk* explosion = Util::Random_Element(m_asteroidExplosions);
    Entity asteroidEntity = m_Coordinator.CreateEntity();
    m_Coordinator.AddComponent<RigidBody>(asteroidEntity, {asteroidVelocity,  Vector2(0, 0)});
    m_Coordinator.AddComponent<Transform>(asteroidEntity, {asteroidPosition,  Vector2(1, 1), Vector2(0,0)});
    m_Coordinator.AddComponent<BasicShape>(asteroidEntity, {asteroidVertices,  m_Color});
    m_Coordinator.AddComponent<Asteroid>(asteroidEntity, {});
    m_Coordinator.AddComponent<SFX>(asteroidEntity, { explosion, nullptr });
    m_Coordinator.AddComponent<ScorePoints>(asteroidEntity, { 100 });
    m_ActiveAsteroids.insert(asteroidEntity);
}

void AsteroidSystem::GetRandomOffScreen(Vector2 &point)
{
    std::random_device rd; // obtain random # from hardware
    std::mt19937 gen(rd());
    const int aboveOrBelow = rand() % 2;
    int Y_min = aboveOrBelow ? SCREEN_HEIGHT : OFFSCREEN_SPAWN_BUFFER * -1;
    int Y_max = aboveOrBelow ? OFFSCREEN_SPAWN_BUFFER + SCREEN_HEIGHT : 0 ;

    const int x_min = OFFSCREEN_SPAWN_BUFFER * -1;
    const int x_max = OFFSCREEN_SPAWN_BUFFER + SCREEN_WIDTH;
        
    std::uniform_int_distribution<int> x_spawn_dist(x_min , x_max);
    const int x = x_spawn_dist(gen);

    // if x is "off screen", change the boundaries of our Y
    if(x < 0 || x > SCREEN_WIDTH)
    {
        // regenerate our Y to also spawn inbetween
        Y_min = 0;
        Y_max = SCREEN_HEIGHT;
    }
    std::uniform_int_distribution<int> y_spawn_dist(Y_min, Y_max);
    const int y = y_spawn_dist(gen);
    point.x = x;
    point.y = y;
}
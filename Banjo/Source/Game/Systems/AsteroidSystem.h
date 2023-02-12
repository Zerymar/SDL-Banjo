#pragma once
#include "../../Core/Coordinator.hpp"
#include "../../Utility/Math/Vector2.h"
#include "../../Utility/Math/Vector3.h"
#include "../../Core/Systems/System.hpp"

extern Coordinator m_Coordinator;
class AsteroidSystem : public System
{
    
public:
    void GenerateAsteroids();
    void GetRandomOffScreen(Vector2& point);
    void Update();
    void Init();
    void OnEntityDelete(Entity entity) override;
    void CreateAsteroidEntity(Vector2 asteroidPosition, Vector2 asteroidVelocity, std::vector<SDL_FPoint> asteroidVertices, Vector3 Color);
    void GenerateVelocity(Vector2& asteroidVelocity, const Vector2& asteroidPosition);
    void CreateOutline(std::vector<SDL_FPoint>& asteroidVertices);
    void CreateAsteroid(const Vector3& Color);

protected:
    Vector3 ColorWhite;
    std::set<Entity> m_ActiveAsteroids;
};

#pragma once

#include <vector>
#include "System.hpp"
#include <SDL.h>
#include "../../Components/Transform.hpp"

class CollisionSystem : public System
{
 public:
 void Init();
 void Update();
 void OnEntityDelete(Entity entity) override;
 void GetTrueVertices(std::vector<SDL_Point>& true_vertices, Transform  transformComponent);
 bool IsCollision(std::vector<SDL_Point> shapeOne, std::vector<SDL_Point> shapeTwo);
 
 std::vector<Entity> m_EntitiesToDelete;
    
};

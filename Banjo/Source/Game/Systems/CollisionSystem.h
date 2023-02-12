#pragma once

#include <vector>
#include "../../Core/Systems/System.hpp"
#include "../../Components/Transform.hpp"
#include "../../Core/Coordinator.hpp"
#include "../../Components/BasicShape.hpp"

class CollisionSystem : public System
{
 public:
    void Init();
    void Update();
    void OnEntityDelete(Entity entity) override;
    void GetTrueVertices(std::vector<SDL_FPoint>& true_vertices, Transform  transformComponent);
    bool PointInPolygon(std::vector<SDL_FPoint> points, std::vector<SDL_FPoint> polygon);

    template <class T>
   void HandleCollision(const Entity& entity, const BasicShape& a_basicShapeComponent,
       const Transform& a_transformComponent);
 
 std::vector<Entity> m_EntitiesToDelete;
    
};

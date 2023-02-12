#include "CollisionSystem.h"

#include "../../Components/BasicShape.hpp"
#include "../../Components/Transform.hpp"
#include "../../Components/Asteroid.hpp"
#include "../Coordinator.hpp"
#include "../../Components/Projectile.hpp"

extern Coordinator m_Coordinator;


void CollisionSystem::Init()
{
}

void CollisionSystem::Update()
{
    for(auto& entity : m_Entities)
    {
        auto& a_transformComponent = m_Coordinator.GetComponent<Transform>(entity);
        auto& a_basicShapeComponent = m_Coordinator.GetComponent<BasicShape>(entity);
        bool bIsProjectile = m_Coordinator.ContainsEntity<Projectile>(entity);

        if(bIsProjectile)
        {
            std::vector<SDL_Point> a_TrueVertices = a_basicShapeComponent.m_Vertices;
            GetTrueVertices(a_TrueVertices, a_transformComponent);
            for(const auto& otherEntity : m_Entities)
            {
                if(entity != otherEntity)
                {
                    auto& b_transformComponent = m_Coordinator.GetComponent<Transform>(otherEntity);
                    auto& b_basicShapeComponent = m_Coordinator.GetComponent<BasicShape>(otherEntity);
                    bool bIsAsteroid = m_Coordinator.ContainsEntity<Asteroid>(entity);
                    std::vector<SDL_Point> b_TrueVertices = b_basicShapeComponent.m_Vertices;
                    GetTrueVertices(b_TrueVertices, b_transformComponent);

                    if(IsCollision(a_TrueVertices, b_TrueVertices))
                    {
                        std::cout << "Collision between " << entity << " and " << otherEntity << std::endl;
                        m_EntitiesToDelete.push_back(otherEntity);
                    }
                }
            }
        }

    }

    for(auto& entity : m_EntitiesToDelete)
    {
        m_Coordinator.DestroyEntity(entity);
    }
    m_EntitiesToDelete.clear();
}

void CollisionSystem::GetTrueVertices(std::vector<SDL_Point>& true_vertices, Transform  transformComponent)
{
    for (auto& vertex : true_vertices)
    {
        vertex.x += transformComponent.position.x;
        vertex.y += transformComponent.position.y;
    }
}

void CollisionSystem::OnEntityDelete(Entity entity)
{
    System::OnEntityDelete(entity);
}

// vs is an array of vertices, [[1,0],[1,1],[0,1],[0,0]] for example
bool CollisionSystem::IsCollision(std::vector<SDL_Point> shapeOne, std::vector<SDL_Point> shapeTwo) {
    /*
      ray-casting algorithm based on
      https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html

    https://stackoverflow.com/questions/22521982/check-if-point-is-inside-a-polygon/29915728#29915728
    */

    bool inside = false;
    for(const auto& vertex : shapeOne)
    {
        int x = vertex.x;
        int y = vertex.y;

        for(int i = 0, j = shapeTwo.size()-1; i < shapeTwo.size(); j = ++i)
        {
            int xI = shapeTwo[i].x;
            int yI = shapeTwo[i].y;

            int xJ = shapeTwo[j].x;
            int yJ = shapeTwo[j].y;

            bool intersect = ((yI > y) != (yJ > y))
            && (x < (xJ - xI) * (y - yI) / (yJ - yI) + xI);

            if(intersect)
            {
                inside = !inside;
            }
        }
        if(inside)
        {
            return true;
        }
    }

    return inside;
}
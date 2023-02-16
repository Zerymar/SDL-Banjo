#include "CollisionSystem.h"


#include "ScoreSystem.h"
#include "../../Components/Transform.hpp"
#include "../../Components/Asteroid.hpp"
#include "../../Components/Projectile.hpp"
#include "../../Components/Player.hpp"
#include "../../Components/SFX.hpp"
#include "../../Core/Systems/AudioSystem.h"
#include "../../Utility/Util.hpp"

extern Coordinator m_Coordinator;


void CollisionSystem::Init()
{
}

// Do Simple Point in Polygon, queue up entities to delete 
template <typename T>
void CollisionSystem::HandleCollision(const Entity& entity, const BasicShape& a_basicShapeComponent,
    const Transform& a_transformComponent)
{
    std::vector<SDL_FPoint> a_TrueVertices = a_basicShapeComponent.m_Vertices;
    GetTrueVertices(a_TrueVertices, a_transformComponent);
    for(const auto& otherEntity : m_Entities)
    {
        if(entity != otherEntity)
        {
            auto& b_transformComponent = m_Coordinator.GetComponent<Transform>(otherEntity);
            auto& b_basicShapeComponent = m_Coordinator.GetComponent<BasicShape>(otherEntity);
            bool bIsComponentT = m_Coordinator.ContainsEntity<T>(otherEntity);
            std::vector<SDL_FPoint> b_TrueVertices = b_basicShapeComponent.m_Vertices;
            GetTrueVertices(b_TrueVertices, b_transformComponent);

            if(PointInPolygon(a_TrueVertices, b_TrueVertices) && bIsComponentT)
            {
                m_Coordinator.QueueEntityToDelete(entity);
                m_Coordinator.QueueEntityToDelete(otherEntity);
                m_EntitiesToDelete.insert(entity);
                m_EntitiesToDelete.insert(otherEntity);
            }
        }
    }
}


//Check for any collisions and delete any entities
void CollisionSystem::Update()
{
    for(auto& entity : m_Entities)
    {
        auto& a_basicShapeComponent = m_Coordinator.GetComponent<BasicShape>(entity);
        auto& a_transformComponent = m_Coordinator.GetComponent<Transform>(entity);

        //Projectile logic
        bool bIsProjectile = m_Coordinator.ContainsEntity<Projectile>(entity);
        if(bIsProjectile)
        {
            HandleCollision<Asteroid>(entity, a_basicShapeComponent, a_transformComponent);
        }
        
        //Asteroid hits Player log
        bool bIsAsteroid = m_Coordinator.ContainsEntity<Asteroid>(entity);
        if(bIsAsteroid)
        {
            HandleCollision<Player>(entity, a_basicShapeComponent, a_transformComponent);
        }
    }

    for(auto& entity : m_EntitiesToDelete)
    {
        
        bool bHasTransform = m_Coordinator.ContainsEntity<Transform>(entity);
        bool bHasSFX = m_Coordinator.ContainsEntity<SFX>(entity);
        if(bHasTransform && bHasSFX)
        {
            auto& sfxComponent = m_Coordinator.GetComponent<SFX>(entity);
            auto& transformComponent = m_Coordinator.GetComponent<Transform>(entity);

        //Only want to play sound if our asteroid is destroyed "on screen" aka by a projectile or ran into the player
            if(!Util::IsOutOfBounds(transformComponent))
            {
                AudioSystem::PlaySound(sfxComponent.DestroySFX, 0);
            }
        }
    }
    m_EntitiesToDelete.clear();
}

void CollisionSystem::GetTrueVertices(std::vector<SDL_FPoint>& true_vertices, Transform  transformComponent)
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

// checks if any point of our polygon intersects
// While not the most efficient, our data set is so minimal
bool CollisionSystem::PointInPolygon(std::vector<SDL_FPoint> points, std::vector<SDL_FPoint> polygon) {

    // number of vertices in polygon
    int verticesPolygon = polygon.size();
    for(const auto& point : points)
    {
        int intersections = 0;
        for(int i =0; i < verticesPolygon; ++i)
        {
            int j = (i +1) % verticesPolygon;
            if((polygon[i].y > point.y) != (polygon[j].y > point.y))
            {
                int x = polygon[i].x + (point.y - polygon[i].y) *
                    (polygon[j].x - polygon[i].x) / (polygon[j].y - polygon[i].y);
                if(x > point.x)
                {
                    ++intersections;
                }
            }
        }

        // at least one point is in the polygon
        if(intersections %2 == 1)
        {
            return true;
        }
    }

    return false;

}
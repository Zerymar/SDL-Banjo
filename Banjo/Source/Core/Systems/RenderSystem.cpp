#include "RenderSystem.h"
#include <vector>
#include "../../Components/BasicShape.hpp"
#include "../Coordinator.hpp"

extern Coordinator m_Coordinator;

void RenderSystem::Init()
{

}

void RenderSystem::SetOffset(std::vector<SDL_FPoint>& vertices, Transform transformComponent)
{
    for (auto& vertex : vertices)
    {
        vertex.x += transformComponent.position.x;
        vertex.y += transformComponent.position.y;

    }
}

void RenderSystem::RenderEntities(SDL_Renderer* renderer)
{
    // 2. Iterate through all entites that need to be rendered
    for(auto const& entity : m_Entities)
    {
        
        auto& basicShapeComponent = m_Coordinator.GetComponent<BasicShape>(entity);
        auto& transformComponent = m_Coordinator.GetComponent<Transform>(entity);
        std::vector<SDL_FPoint> vertices = basicShapeComponent.m_Vertices;
        Vector3 Color = basicShapeComponent.color;
        SDL_SetRenderDrawColor(renderer, (Uint8) Color.x, (Uint8) Color.y, (Uint8) Color.z, 255);
        if(basicShapeComponent.m_Vertices.size() == 1)
        {
            SDL_RenderDrawPointF(renderer, vertices[0].x, vertices[0].y);
        }
        else if(basicShapeComponent.m_Vertices.size() > 1)
        {
            
            // Make sure we draw back to our starting vertex
            SDL_FPoint firstVertex = vertices[0];
            vertices.push_back(firstVertex);
            SetOffset(vertices, transformComponent);
            const int polygonSides = static_cast<int>(vertices.size());
            SDL_RenderDrawLinesF(renderer, vertices.data(), polygonSides);
        }
    }
}


void RenderSystem::RenderPoints(SDL_Renderer* renderer, const std::vector<SDL_FPoint> points, const Vector3 Color)
{
    if(points.empty()) return;
    SDL_SetRenderDrawColor(renderer, static_cast<Uint8>(Color.x), static_cast<Uint8>(Color.y),
        static_cast<Uint8>(Color.z), 255);
    for(auto point : points)
    {
        SDL_RenderDrawPointF(renderer, point.x, point.y);
    }
}

void RenderSystem::RenderLines(SDL_Renderer* renderer, std::vector<SDL_FPoint> points, const Vector3 Color)
{
    if(points.empty() || points.size() <= 1) return;
    SDL_SetRenderDrawColor(renderer, static_cast<Uint8>(Color.x), static_cast<Uint8>(Color.y),
        static_cast<Uint8>(Color.z), 255);
    // add our first point again at the end
    const SDL_FPoint originPoint = points[0];
    points.push_back(originPoint);
    
    SDL_RenderDrawLinesF(renderer, points.data(), static_cast<int>(points.size()));
}

void RenderSystem::OnEntityDelete(Entity entity)
{
    System::OnEntityDelete(entity);
}

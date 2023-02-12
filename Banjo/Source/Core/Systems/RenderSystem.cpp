#include "RenderSystem.h"
#include <vector>
#include "../../Components/BasicShape.hpp"
#include "../Coordinator.hpp"

extern Coordinator m_Coordinator;

void RenderSystem::Init()
{

}

void RenderSystem::SetOffset(std::vector<SDL_Point>& vertices, Transform transformComponent)
{
    for (auto& vertex : vertices)
    {
        vertex.x += transformComponent.position.x;
        vertex.y += transformComponent.position.y;

    }
}

void RenderSystem::RenderEntities(SDL_Renderer* renderer, Vector3 Color)
{
    SDL_SetRenderDrawColor(renderer, Color.x, Color.y, Color.z, 255);
    
    // 2. Iterate through all entites that need to be rendered
    for(auto const& entity : m_Entities)
    {
        auto& basicShapeComponent = m_Coordinator.GetComponent<BasicShape>(entity);
        auto& transformComponent = m_Coordinator.GetComponent<Transform>(entity);
        std::vector<SDL_Point> vertices = basicShapeComponent.m_Vertices;
        
  
        if(basicShapeComponent.m_Vertices.size() == 1)
        {
            SDL_RenderDrawPoint(renderer, vertices[0].x, vertices[0].y);
        }
        else if(basicShapeComponent.m_Vertices.size() > 1)
        {
            // Make sure we draw back to our starting vertex
            SDL_Point firstVertex = vertices[0];
            vertices.push_back(firstVertex);
            
            SetOffset(vertices, transformComponent);
            SDL_RenderDrawLines(renderer, vertices.data(), vertices.size());
        }
        
    }
    
}


void RenderSystem::RenderPoints(SDL_Renderer* renderer, const std::vector<SDL_Point> points, const Vector3 Color)
{
    if(points.empty()) return;
    SDL_SetRenderDrawColor(renderer, Color.x, Color.y, Color.z, 255);
    for(auto point : points)
    {
        SDL_RenderDrawPoint(renderer, point.x, point.y);
    }
}

void RenderSystem::RenderLines(SDL_Renderer* renderer, std::vector<SDL_Point> points, const Vector3 Color)
{
    if(points.empty() || points.size() <= 1) return;
    SDL_SetRenderDrawColor(renderer, Color.x, Color.y, Color.z, 255);
    // add our first point again at the end
    const SDL_Point originPoint = points[0];
    points.push_back(originPoint);
    SDL_RenderDrawLines(renderer, points.data(), points.size());
}
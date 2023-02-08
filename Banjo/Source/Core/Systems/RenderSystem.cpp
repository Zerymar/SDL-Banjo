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

void RenderSystem::Render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    // 2. Iterate through all entites that need to be rendered
    for(auto const& entity : m_Entities)
    {
        auto& basicShapeComponent = m_Coordinator.GetComponent<BasicShape>(entity);
        auto& transformComponent = m_Coordinator.GetComponent<Transform>(entity);
        std::vector<SDL_Point> vertices = basicShapeComponent.m_Vertices;
        SetOffset(vertices, transformComponent);
        Vector3 Color = basicShapeComponent.color;
        int xColor = (int) Color.x;
        int yColor = (int) Color.y;
        int zColor = (int) Color.z;
        
        SDL_RenderDrawLines(renderer, vertices.data(), vertices.size());
    }
    
}

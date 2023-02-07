#include "RenderSystem.h"

#include "../Coordinator.hpp"
#include "../../Components/BasicShape.hpp"

extern Coordinator g_Coordinator;

void RenderSystem::Init()
{
}

void RenderSystem::Render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    
    // 1. Clear Renderer
    SDL_RenderClear(renderer);

    // 2. Iterate through all entites that need to be rendered
    for(auto const& entity : m_Entities)
    {
        auto& basicShapeComponent = g_Coordinator.GetComponent<BasicShape>(entity);
        std::vector<SDL_Point> vertices = basicShapeComponent.m_Vertices;
        const int linesToDraw = vertices.size() + 1;
        Vector3 Color = basicShapeComponent.color;
        SDL_SetRenderDrawColor(renderer, (int) Color.x, (int)Color.y, (int) Color.z, 255);
        SDL_RenderDrawLines(renderer, vertices.data(), linesToDraw);
    }
    //3. Present
    // 3.3 Present Renderer
    SDL_RenderPresent(renderer);
}

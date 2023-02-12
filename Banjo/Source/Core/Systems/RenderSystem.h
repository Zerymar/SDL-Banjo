#pragma once
#include <SDL_render.h>
#include <vector>
#include "../../Components/Transform.hpp"
#include "System.hpp"
#include "../../Utility/Math/Vector3.h"

struct Transform;

class RenderSystem : public System
{
protected:
    void SetOffset(std::vector<SDL_Point>& vetices, Transform transformComponent);
public:
    void Init();
    void RenderEntities(SDL_Renderer* renderer, Vector3 Color);
    void RenderPoints(SDL_Renderer* renderer, std::vector<SDL_Point> points, Vector3 Color);
    void RenderLines(SDL_Renderer* renderer, std::vector<SDL_Point> points, Vector3 Color);
    void OnEntityDelete(Entity entity) override;
    
};

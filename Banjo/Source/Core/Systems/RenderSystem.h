#pragma once
#include <SDL_render.h>
#include <vector>
#include "../../Components/Transform.hpp"
#include "System.hpp"


struct Transform;

class RenderSystem : public System
{
protected:
    void SetOffset(std::vector<SDL_Point>& vetices, Transform transformComponent);
public:
    void Init();
    void Render(SDL_Renderer* renderer);
    
};

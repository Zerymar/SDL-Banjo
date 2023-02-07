#pragma once
#include <SDL_render.h>

#include "System.hpp"

class RenderSystem : public System
{
public:
    void Init();
    void Render(SDL_Renderer* renderer);
    
};

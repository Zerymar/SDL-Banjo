#pragma once
#include "../Utility/Vector2.h"
#include "SDL.h"

class Shape
{
protected:
    Vector2 position;
    SDL_Renderer *m_pRenderer;
    
public:
    Shape(SDL_Renderer *renderer);
    ~Shape();
};

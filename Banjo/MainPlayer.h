#pragma once
#include <SDL_render.h>

#include "Components/InputComponent.h"
#include "Shapes/TriangleShape.h"
#include "Utility/Vector2.h"

class MainPlayer
{
protected:
    SDL_Renderer *m_pRenderer;
    TriangleShape m_Triangle;

public:
    InputComponent inputComp;
    Vector2 m_PlayerPosition;
    MainPlayer(SDL_Renderer *renderer, int triangle_sideLength, Vector2 position);
    Vector2 GetCurrentPosition();
    void Translate(Vector2 vector);
    void Render();
};

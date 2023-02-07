#pragma once
#include "Shape.h"

class TriangleShape :  public Shape
{
public:
    TriangleShape(SDL_Renderer *renderer, int sideLength, Vector2 *Position);
    ~TriangleShape();
    void Render();
    void UpdatePosition();
    Vector2 *m_Position;
    
protected:
    void GetTriangleHeight(int& height);
    SDL_Point m_vertices[4];
    
    int m_sideLength;
    
};

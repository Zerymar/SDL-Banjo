#include "TriangleShape.h"


#include <ostream>

TriangleShape::TriangleShape(SDL_Renderer *renderer, int sideLength, Vector2 *Position) : Shape(renderer)
{
    //Initialize our globals
    m_sideLength = sideLength;
    m_Position = Position;
    UpdatePosition();
    
}
void TriangleShape::Render()
{
    // Update our positions based on position vector
    UpdatePosition();
    SDL_RenderDrawLines(m_pRenderer, m_vertices, 4);
    
}

void TriangleShape::UpdatePosition()
{
    m_vertices[0].x = m_Position->x;
    m_vertices[0].y = m_Position->y;

    //Just set the height to the sidelength initially
    int height = m_sideLength;
    GetTriangleHeight(height);
    m_vertices[1].x = m_Position->x + m_sideLength/2;
    m_vertices[1].y = m_Position->y + height;

    
    m_vertices[2].x = m_Position->x + m_sideLength;
    m_vertices[2].y = m_Position->y;

    m_vertices[3] = m_vertices[0];
}
void TriangleShape::GetTriangleHeight(int& height)
{
    // a^2 + b^2 = c^2
    // a = sideLength/2, b = height, C = sidelight
    // b = sqrt(a*a + c*c)
    int a = m_sideLength/2;
    int c = m_sideLength;
    height = sqrt(c*c - a*a);
}

TriangleShape::~TriangleShape() 
{
}

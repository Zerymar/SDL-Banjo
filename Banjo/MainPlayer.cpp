#include "MainPlayer.h"

MainPlayer::MainPlayer(SDL_Renderer* renderer, int triangle_sideLength, Vector2 position) : m_Triangle(renderer, triangle_sideLength, &position)
{
    m_pRenderer = renderer;
    m_PlayerPosition = position;
    m_Triangle.m_Position = &m_PlayerPosition;
    inputComp.m_ParentPosition = &m_PlayerPosition;
    
}

void MainPlayer::Render()
{
    SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    m_Triangle.Render();
}


Vector2 MainPlayer::GetCurrentPosition()
{
    return m_PlayerPosition;
}

void MainPlayer::Translate(Vector2 vector)
{
    m_PlayerPosition.x += vector.x;
    m_PlayerPosition.y += vector.y;
}

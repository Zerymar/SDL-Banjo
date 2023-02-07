#include "InputComponent.h"


InputComponent::InputComponent()
{
    m_ParentPosition = nullptr;
    m_Velocity.x = 0;
    m_Velocity.y = 0;
}
void InputComponent::Update()
{
    m_ParentPosition->x += m_Velocity.x;
    m_ParentPosition->y += m_Velocity.y;
}
void InputComponent::HandleInput(const SDL_Event& event)
{
    switch(event.type)
    {
    case SDL_KEYDOWN:
        switch(event.key.keysym.sym)
        {
            case SDLK_a:
                m_Velocity.x = -1;
                break;
            case SDLK_d:
                m_Velocity.x = 1;
                break;
            case SDLK_w:
                m_Velocity.y = -1;
                break;
            case SDLK_s:
                m_Velocity.y = 1;
                break;  
        }
        break;
        // Only touch when we are not moving in a direction already upon release
    case SDL_KEYUP:
        switch(event.key.keysym.sym)
        {
            case SDLK_a:
                if(m_Velocity.x < 0){ m_Velocity.x = 0; }
                break;
            case SDLK_d:
                if(m_Velocity.x > 0){ m_Velocity.x = 0; }
                break;
            case SDLK_w:
                if(m_Velocity.y < 0){ m_Velocity.y = 0; }
                 break;
            case SDLK_s:
                if(m_Velocity.y > 0){ m_Velocity.y = 0; }
                break;  
        }
        break;
    default:
        break;
    }
}

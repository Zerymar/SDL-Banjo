#pragma once
#include <SDL_events.h>
#include"../Utility/Math/Vector2.h"

class InputComponent
{
protected:
    
    
public:
    InputComponent();
    Vector2 *m_ParentPosition;
    Vector2 m_Velocity;
    int m_ScalarVelocity;
    
    void HandleInput(const SDL_Event& event);
    void Update();
};

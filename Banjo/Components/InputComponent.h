#pragma once
#include <SDL_events.h>
#include"../Utility/Vector2.h"

class InputComponent
{
public:
    InputComponent();
    Vector2 *m_ParentPosition;
    Vector2 m_Velocity;
    void HandleInput(const SDL_Event& event);
    void Update();
};

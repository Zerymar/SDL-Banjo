#pragma once
#include <bitset>
#include <SDL_events.h>

#include "System.hpp"

class PlayerInputSystem : public System
{
public:
   void Init();
   void Update();
   void HandleInput(const SDL_Event& event);

private:
   //std::bitset<8> m_buttons;
   float m_ScalarVelocity = 1.0f;
};

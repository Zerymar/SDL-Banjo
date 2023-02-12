#pragma once
#include <bitset>
#include <SDL_events.h>


#include "System.hpp"

class PlayerInputSystem : public System
{
public:
   void Init();
   void Update();
   void CreatePlayerProjectile();
   void HandleInput(const SDL_Event& event);
   void MouseButtonEvent(const SDL_MouseButtonEvent& button);
   

private:
   //std::bitset<8> m_buttons;
   int m_ScalarVelocity = 5;
   SDL_Point m_ClickPoint;
   bool m_bLeftButtonDown = false;
};

#pragma once
#include "PlayerInputSystem.h"

#include <iostream>

#include "../../Components/RigidBody.hpp"
#include "../../Components/Transform.hpp"
#include "../Coordinator.hpp"

extern Coordinator m_Coordinator;

void PlayerInputSystem::Init()
{
}

void PlayerInputSystem::Update()
{
    for(auto& entity :m_Entities)
    {
        auto& transformComp = m_Coordinator.GetComponent<Transform>(entity);
        auto& rigidBodyComp = m_Coordinator.GetComponent<RigidBody>(entity);

        transformComp.position += rigidBodyComp.velocity;
    }
    
}

void PlayerInputSystem::HandleInput(const SDL_Event& event)
{
    for(auto& entity :m_Entities)
    {
        auto& rigidBodyComp = m_Coordinator.GetComponent<RigidBody>(entity);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_a:
                        rigidBodyComp.velocity.x = -m_ScalarVelocity;
                        break;
                    case SDLK_d:
                        rigidBodyComp.velocity.x = m_ScalarVelocity;
                         break;
                    case SDLK_w:
                        rigidBodyComp.velocity.y = -m_ScalarVelocity;
                        break;
                    case SDLK_s:
                        rigidBodyComp.velocity.y = m_ScalarVelocity;
                        break;  
                }
                break;
            // Only touch when we are not moving in a direction already upon release
            case SDL_KEYUP:
                switch(event.key.keysym.sym)
                {
                    case SDLK_a:
                        if(rigidBodyComp.velocity.x < 0){ rigidBodyComp.velocity.x = 0; }
                        break;
                    case SDLK_d:
                        if(rigidBodyComp.velocity.x > 0){ rigidBodyComp.velocity.x = 0; }
                        break;
                    case SDLK_w:
                        if(rigidBodyComp.velocity.y < 0){ rigidBodyComp.velocity.y = 0; }
                         break;
                    case SDLK_s:
                        if(rigidBodyComp.velocity.y > 0){ rigidBodyComp.velocity.y = 0; }
                        break;  
                }
                break;
            default:
                break;
        }
    }
}

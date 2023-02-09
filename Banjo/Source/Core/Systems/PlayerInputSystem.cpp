#pragma once
#include "PlayerInputSystem.h"

#include <iostream>

#include "../../Components/RigidBody.hpp"
#include "../../Components/Transform.hpp"
#include "../../Components/BasicShape.hpp"
#include "../../Components/Player.hpp"
#include "../../Components/Gravity.hpp"
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

void PlayerInputSystem::CreatePlayerProjectile()
{
    for(auto& entity : m_Entities)
    {
        auto& playerComponent = m_Coordinator.GetComponent<Player>(entity);
        auto& transformComp = m_Coordinator.GetComponent<Transform>(entity);
        // straight line based on the 3rd vertix
        std::vector<SDL_Point> projectile_verticies;
        SDL_Point startPoint = playerComponent.ProjectileSpawnLocation;
        SDL_Point endPoint;
        endPoint.x = startPoint.x + 3;
        endPoint.y = startPoint.y ;

        projectile_verticies.push_back(startPoint);
        projectile_verticies.push_back(endPoint);
        
        int r=255,g=255,b=255;
        Vector3 ColorWhite(r,g,b);

        int projectileSpawnX = transformComp.position.x;
        int projectileSpawnY = transformComp.position.y;
        
        Entity projectileEntity = m_Coordinator.CreateEntity();
        m_Coordinator.AddComponent<RigidBody>(projectileEntity, {Vector2(5, 0),  Vector2(0, 0)});
        m_Coordinator.AddComponent<Transform>(projectileEntity, {Vector2(projectileSpawnX, projectileSpawnY),  Vector2(1, 1), Vector2(0,0)});
        m_Coordinator.AddComponent<BasicShape>(projectileEntity, {projectile_verticies,  ColorWhite});
        m_Coordinator.AddComponent<Gravity>(projectileEntity,{Vector2(0, 0)});
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
                    case SDLK_SPACE:
                        CreatePlayerProjectile();
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

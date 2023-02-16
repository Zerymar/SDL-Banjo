#pragma once

#include "PhysicsSystem.h"

#include "../../Components/Gravity.hpp"
#include "../../Components/RigidBody.hpp"
#include "../../Components/Transform.hpp"
#include "../Coordinator.hpp"
#include "../../Components/BasicShape.hpp"
#include "../../Components/SFX.hpp"
#include "../../Utility/Util.hpp"

extern Coordinator m_Coordinator;


void PhysicsSystem::Init()
{
}

void PhysicsSystem::Update(float deltaTime)
{
    for(auto &entity : m_Entities)
    {
        auto& rigidBodyComponent = m_Coordinator.GetComponent<RigidBody>(entity);
        auto& transformComponent = m_Coordinator.GetComponent<Transform>(entity);
        
        
        //Apply our movement
        transformComponent.position += rigidBodyComponent.velocity; //* deltaTime;

        //If our entity goes too far out of bounds, destroy it
        if(Util::IsOutOfBounds(transformComponent))
        {
            //remove destroy SFX, we don't want to play it
            bool bHasSFX = m_Coordinator.ContainsEntity<SFX>(entity);
            if(bHasSFX)
            {
                m_Coordinator.RemoveComponent<SFX>(entity);
            }
            m_Coordinator.QueueEntityToDelete(entity);
        }
    }
}

void PhysicsSystem::OnEntityDelete(Entity entity)
{
    System::OnEntityDelete(entity);
}

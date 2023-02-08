#pragma once

#include "PhysicsSystem.h"

#include "../../Components/Gravity.hpp"
#include "../../Components/RigidBody.hpp"
#include "../../Components/Transform.hpp"
#include "../Coordinator.hpp"

extern Coordinator m_Coordinator;


void PhysicsSystem::Init()
{
}

void PhysicsSystem::Update(float deltaTime)
{
    for(auto const& entity : m_Entities)
    {
        auto& rigidBodyComponent = m_Coordinator.GetComponent<RigidBody>(entity);
        auto& transformComponent = m_Coordinator.GetComponent<Transform>(entity);

        // Gravity force
        auto const& gravity = m_Coordinator.GetComponent<Gravity>(entity);

        //Apply our movement
        transformComponent.position += rigidBodyComponent.velocity; //* deltaTime;
        rigidBodyComponent.velocity += gravity.force; //* deltaTime;
        
    }
}

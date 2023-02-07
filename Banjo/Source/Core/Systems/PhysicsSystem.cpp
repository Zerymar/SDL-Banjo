#pragma once

#include "PhysicsSystem.h"
#include "../Coordinator.hpp"
#include "../../Components/Gravity.hpp"
#include "../../Components/RigidBody.hpp"
#include "../../Components/Transform.hpp"

extern Coordinator g_Coordinator;

void PhysicsSystem::Init()
{
}

void PhysicsSystem::Update(float deltaTime)
{
    for(auto const& entity : m_Entities)
    {
        auto& rigidBodyComponent = g_Coordinator.GetComponent<RigidBody>(entity);
        auto& transformComponent = g_Coordinator.GetComponent<Transform>(entity);

        // Gravity force
        auto const& gravity = g_Coordinator.GetComponent<Gravity>(entity);

        //Apply our movement
        transformComponent.position += rigidBodyComponent.velocity * deltaTime;
        rigidBodyComponent.velocity += gravity.force * deltaTime;
        
    }
}

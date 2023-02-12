#pragma once

#include "PhysicsSystem.h"

#include "../../Components/Gravity.hpp"
#include "../../Components/RigidBody.hpp"
#include "../../Components/Transform.hpp"
#include "../Coordinator.hpp"
#include "../../Components/BasicShape.hpp"
#include "../../Utility/defs.h"

extern Coordinator m_Coordinator;


void PhysicsSystem::Init()
{
}

void PhysicsSystem::Update(float deltaTime)
{
    for(auto const &entity : m_Entities)
    {
        auto& rigidBodyComponent = m_Coordinator.GetComponent<RigidBody>(entity);
        auto& transformComponent = m_Coordinator.GetComponent<Transform>(entity);
        auto& basicShapeComponent = m_Coordinator.GetComponent<BasicShape>(entity);

        // Gravity force
        auto& gravity = m_Coordinator.GetComponent<Gravity>(entity);

        //Apply our movement
        transformComponent.position += rigidBodyComponent.velocity; //* deltaTime;
        rigidBodyComponent.velocity += gravity.force; //* deltaTime;


        //If our entity goes too far out of bounds, destroy it
        int min_deadzone_x = 0 - OFFSCREEN_DEAD_ZONE;
        int max_deadzone_x = SCREEN_WIDTH + OFFSCREEN_DEAD_ZONE;

        int min_deadzone_y = 0 - OFFSCREEN_DEAD_ZONE;
        int max_deadzone_y = SCREEN_HEIGHT + OFFSCREEN_DEAD_ZONE;

        if(transformComponent.position.x < min_deadzone_x || transformComponent.position.x > max_deadzone_x
            ||transformComponent.position.y < min_deadzone_y || transformComponent.position.y > max_deadzone_y)
        {
            m_EntitiesToDelete.push_back(entity);
        }
    }

    for(auto const& entity: m_EntitiesToDelete)
    {
        m_Coordinator.DestroyEntity(entity);
    }

    //clear out queue
    m_EntitiesToDelete.clear();
}

void PhysicsSystem::OnEntityDelete(Entity entity)
{
    System::OnEntityDelete(entity);
}

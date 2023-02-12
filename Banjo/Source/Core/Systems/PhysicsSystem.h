#pragma once
#include <vector>

#include "System.hpp"


class PhysicsSystem : public System
{
    
public:
    void Init();
    void Update(float deltaTime);
    void OnEntityDelete(Entity entity) override;
    std::vector<Entity> m_EntitiesToDelete;
};

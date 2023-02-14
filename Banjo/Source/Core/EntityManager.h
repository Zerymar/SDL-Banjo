#pragma once

#include <queue>
#include <array>
#include <set>

#include "../Utility/types.hpp"

class EntityManager
{
public:
    EntityManager();
    Entity CreateEntity();
    Signature GetSignature(Entity entity);
    void InvalidateEntity(Entity entity);
    void DestroyEntity(Entity entity);
    void SetSignature(Entity entity, Signature signature);
    void DestroyAllEntities(std::set<Entity>& DestroyedEntities);

protected:
    //unused entity IDs
    std::queue<Entity> m_AvailableEntities{};

    std::set<Entity> m_CreatedEntities;

    // Array of signatures where index corresponds to entity Id
    std::array<Signature,MAX_ENTITIES> m_Signatures{};

    // Total living entities - how many currently exist
    uint32_t m_LivingEntityCount{};
};

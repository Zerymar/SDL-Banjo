#include "EntityManager.h"

#include <cassert>

EntityManager::EntityManager()
{
    //Initialize all possible entities
    for(Entity entity = 0; entity < MAX_ENTITIES; ++entity)
    {
        m_AvailableEntities.push(entity);
    }
}

Entity EntityManager::CreateEntity()
{
    // Do not make more than we have
    assert(m_LivingEntityCount < MAX_ENTITIES && "Too many entities exist!");

    Entity availableId = m_AvailableEntities.front();
    m_AvailableEntities.pop(); // pop -Beau
    ++m_LivingEntityCount;
    m_CreatedEntities.insert(availableId);
    return availableId;
}

Signature EntityManager::GetSignature(Entity entity)
{
    assert(entity < MAX_ENTITIES && "Entity out of range.");
    return m_Signatures[entity];
}

void EntityManager::DestroyEntity(Entity entity)
{
    assert(entity < MAX_ENTITIES && "Entity out of range.");
    InvalidateEntity(entity);
    m_CreatedEntities.erase(entity);
}

void EntityManager::InvalidateEntity(Entity entity)
{
    //Invalidate signature
    m_Signatures[entity].reset();

    //Push ID back into Queue
    m_AvailableEntities.push(entity);
    --m_LivingEntityCount;
}

void EntityManager::DestroyAllEntities(std::set<Entity>& DestroyedEntities)
{
    for(auto& entity : m_CreatedEntities)
    {
        InvalidateEntity(entity);
        DestroyedEntities.insert(entity);
    }
    m_CreatedEntities.clear();
}

void EntityManager::SetSignature(Entity entity, Signature signature)
{
    assert(entity < MAX_ENTITIES && "Entity out of range.");
    m_Signatures[entity] = signature;
}

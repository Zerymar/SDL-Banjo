#include "ComponentArray.h"

#include <cassert>

template <typename T>
void ComponentArray<T>::InsertData(Entity entity, T component)
{
    // Assert condition, where && is "then do"
    assert(m_EntityToIndexMap.find(entity) == m_EntityToIndexMap.end() && "Component added to same entity more than once.");

    //put a new entry at end of array, always use our size
    size_t newIndex = m_Size;
    m_EntityToIndexMap[entity] = newIndex;
    m_IndexToEntityMap[newIndex] = entity;
    m_ComponentArray[newIndex] = component;
    ++m_Size;
}

template <typename T>
void ComponentArray<T>::RemoveData(Entity entity)
{
    // Assert condition, where && is "then do"
    assert(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end() && "Unable to remove non-existent component.");
    size_t indexOfRemovedEntity = m_EntityToIndexMap[entity];
    size_t indexOfLastElement = m_Size - 1;
    
    //Copy element into the deleted elements place to maintain the density
    m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement];

    //Update map information
    Entity entityOfLastElement = m_IndexToEntityMap[indexOfLastElement];
    m_EntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
    m_IndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

    // Finally, remove the information
    m_EntityToIndexMap.erase(entity);
    m_IndexToEntityMap.erase(indexOfLastElement);

    --m_Size;
}

template <typename T>
void ComponentArray<T>::EntityDestroyed(Entity entity) 
{
    if(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end())
    {
        RemoveData(entity);
    }
}

template <typename T>
T& ComponentArray<T>::GetData(Entity entity)
{
    assert(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end() && "Unable to find non-existent component.");
    return m_ComponentArray[m_EntityToIndexMap[entity]];
}

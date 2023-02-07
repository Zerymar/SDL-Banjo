#pragma once
#include "../Utility/types.hpp"
#include <array>
#include <unordered_map>

class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
    void InsertData(Entity entity, T component);
    void RemoveData(Entity entity);
    void EntityDestroyed(Entity entity) override;
    T& GetData(Entity entity);

protected:

    //Packed array of components, each entity has it's own unique spot
    std::array<T,MAX_ENTITIES> m_ComponentArray;
    std::unordered_map<Entity, size_t> m_EntityToIndexMap;
    std::unordered_map<size_t, Entity> m_IndexToEntityMap;

    //Total size of valid entries in the array
    size_t m_Size{};
    
};



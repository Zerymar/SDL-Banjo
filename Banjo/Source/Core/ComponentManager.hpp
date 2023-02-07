﻿#pragma once

#include "ComponentArray.h"
#include "../Utility/types.hpp"
#include <any>
#include <cassert>
#include <memory>
#include <unordered_map>

class ComponentManager
{
public:
    template<typename T>
    void RegisterComponent()
    {
        const char* typeName = typeid(T).name();
        
        assert(m_ComponentTypes.find(typeName) == m_ComponentTypes.end() && "Registering component type more than once.");

        m_ComponentTypes.insert({typeName, m_nextComponentType});
        m_ComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

        ++m_nextComponentType;
        
    }

    // Return this component's type - used for creating signatures
    template<typename T>
    ComponentType GetComponentType()
    {
        const char* typeName = typeid(T).name();

        assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component not registed before use.");

        return m_ComponentTypes[typeName];
    }

    // Add a component to the array for an entity
    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        GetComponentArray<T>()->InsertData(entity, component);
    }

    template<typename T>
    void RemoveComponent(Entity entity)
    {
        GetComponentArray<T>()->RemoveData(entity);
    }

    template<typename  T>
    T& GetComponent(Entity entity)
    {
        return GetComponentArray<T>()->GetData(entity);
    }

    void EntityDestroyed(Entity entity)
    {
        for (auto const& pair : m_ComponentArrays)
        {
            auto const& component = pair.second;
            component->EntityDestroyed(entity);
        }
    }

private:
    std::unordered_map<const char*, ComponentType> m_ComponentTypes{};
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_ComponentArrays{};
    ComponentType m_nextComponentType{};

    template<typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray()
    {
        const char* typeName = typeid(T).name();
        assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component not registered before use.");
        return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
    }
    
};
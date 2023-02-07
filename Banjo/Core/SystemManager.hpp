#pragma once
#include "System.hpp"
#include "../Utility/types.h"
#include <cassert>
#include <memory>
#include <unordered_map>

class SystemManager
{
public:
    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        const char* typeName  = typeid(T).name();
        assert(m_Systems.find(typeName) == m_Systems.end() && "Registering system more than once.");

        auto system = std::make_shared<T>();
        m_Systems.insert({typeName, system});
        return system;
    }

    template<typename T>
    void SetSignature(Signature signature)
    {
        const char* typeName = typeid(T).name();
        assert(m_Systems.find(typeName) != m_Systems.end() && "System used before registered");
        m_Signatures.insert({typeName, signature});
    }
    
    // Erase a destroyed entity from all system lists
    // m_Entities is a set so no check needed
    void EntityDestroyed(Entity entity)
    {
        for(auto const& pair: m_Systems)
        {
            auto const& system = pair.second;
            system->m_Entities.erase(entity);
        }
    }

    // Notify each system that an entity's signature changed
    
    void EntitySignatureChanged(Entity entity, Signature entitySignature)
    {
        for(auto const& pair: m_Systems)
        {
            auto const& type = pair.first;
            auto const& system = pair.second;
            auto const& systemSignature = m_Signatures[type];

            // Entity signature matches system signature - insert into set
            if((entitySignature & systemSignature) == systemSignature)
            {
                system->m_Entities.insert(entity);
            }
            // Entity signature does not match system signature - erase from set
            else
            {
                system->m_Entities.erase(entity);
            }
        }
    }

protected:
    std::unordered_map<const char*, Signature> m_Signatures{};
    std::unordered_map<const char*, std::shared_ptr<System>> m_Systems{};
};
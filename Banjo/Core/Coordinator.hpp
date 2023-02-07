#pragma once


#include "EntityManager.h"
//#include "EventManager.hpp"
#include "SystemManager.hpp"
#include "../Utility/types.h"
#include <memory>
#include "ComponentManager.hpp"


class Coordinator
{
public:
    void init()
    {
        m_ComponentManager = std::make_unique<ComponentManager>();
        m_EntityManager = std::make_unique<EntityManager>();
        m_SystemManager = std::make_unique<SystemManager>();
    }
    
// Entity block
    Entity CreateEntity()
    {
        return m_EntityManager->CreateEntity();
    }

    void DestroyEntity(Entity entity)
    {
        m_EntityManager->DestroyEntity(entity);
        m_ComponentManager->EntityDestroyed(entity);
        m_SystemManager->EntityDestroyed(entity);
    }
//Component Methods
    template<typename T>
    void RegisterComponent()
    {
        m_ComponentManager->RegisterComponent<T>();
    }
    
    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        m_ComponentManager->AddComponent(entity, component);

        //Update signature and notify systems
        auto signature = m_EntityManager->GetSignature(entity);
        signature.set(m_ComponentManager->GetComponentType<T>(), true);
        m_EntityManager->SetSignature(entity, signature);
        m_SystemManager->EntitySignatureChanged(entity, signature);
    }
    template<typename T>
    void RemoveComponent(Entity entity)
    {
        m_ComponentManager->RemoveComponent<T>(entity);

        //Update signature and ntoify systems
        auto signature = m_EntityManager->GetSignature(entity);
        signature.set(m_ComponentManager->GetComponentType<T>(), false);
        m_EntityManager->SetSignature(entity, signature);
        m_SystemManager->EntitySignatureChanged(entity, signature);
    }

    template<typename T>
    T& GetComponent(Entity entity)
    {
        return m_ComponentManager->GetComponent<T>(entity);
    }

    template<typename T>
    ComponentType GetComponentType()
    {
        return m_ComponentManager->GetComponentType<T>();
    }

    // system methods
    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        return m_SystemManager->RegisterSystem<T>();
    }

    template<typename T>
    void SetSystemSignature(Signature signature)
    {
        m_SystemManager->SetSignature<T>(signature);
    }
    
    
protected:
    std::unique_ptr<ComponentManager> m_ComponentManager;
    std::unique_ptr<EntityManager> m_EntityManager;
    //std::unique_ptr<EventManager> m_EventManager;
    std::unique_ptr<SystemManager> m_SystemManager;
};

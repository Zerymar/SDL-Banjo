#pragma once

#include <iostream>

#include "ComponentManager.hpp"
#include "EntityManager.h"
#include "SystemManager.hpp"
#include "../Utility/types.hpp"
#include <memory>
#include <ostream>
#include <SDL.h>

#include "../Game/Systems/ScoreSystem.h"
#include "../Utility/Math/Geometry.hpp"

class Coordinator
{
public:
    std::vector<SDL_FPoint> m_ConvexHull;
    
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

    void DestroyAllEntities()
    {
        std::set<Entity> destroyedEntities;
        m_EntityManager->DestroyAllEntities(destroyedEntities);
        for(auto& entity : destroyedEntities)
        {
            m_ComponentManager->EntityDestroyed(entity);
            m_SystemManager->EntityDestroyed(entity);
        }
        destroyedEntities.clear();
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

    void AddDrawnPoint(SDL_FPoint point)
    {
        m_DrawnPoints.push_back(point);
    }

    void GenerateConvexHull()
    {
        Geometry::ConvexHull(m_ConvexHull, m_DrawnPoints);
    }

    template<typename T>
    bool ContainsEntity(Entity entity)
    {
        return m_ComponentManager->ContainsEntity<T>(entity);
    }

    bool IsPaused()
    {
        return m_bIsPaused;
    }

    void TogglePause()
    {
        m_bIsPaused = !m_bIsPaused;
    }

    bool ShouldRestart()
    {
        return m_bRestartGame;
    }

    void ToggleRestartFlag()
    {
        m_bRestartGame = !m_bRestartGame;
    }
    


protected:
    std::unique_ptr<ComponentManager> m_ComponentManager;
    std::unique_ptr<EntityManager> m_EntityManager;
   // std::unique_ptr<EventManager> m_EventManager;
    std::unique_ptr<SystemManager> m_SystemManager;
    std::vector<SDL_FPoint> m_DrawnPoints;
    bool m_bIsPaused = false;
    bool m_bRestartGame = false;
    
};

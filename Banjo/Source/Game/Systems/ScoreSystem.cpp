#include "ScoreSystem.h"

#include "../../Components/ScorePoints.hpp"
#include "../../Core/Coordinator.hpp"

extern Coordinator m_Coordinator;
void ScoreSystem::Init()
{
    m_Score = 0;
}

void ScoreSystem::Update()
{
    for(auto& entity: m_Entities)
    {
        std::set<Entity> entitiesQueued;
        m_Coordinator.GetQueuedEntitiesToDelete(entitiesQueued);
        if(entitiesQueued.find(entity) != entitiesQueued.end())
        {
            auto& scorePointsComp = m_Coordinator.GetComponent<ScorePoints>(entity);
            m_Score += scorePointsComp.points;
        }
    }
}

void ScoreSystem::OnEntityDelete(Entity entity)
{
    //auto& scorePoints = m_Coordinator.GetComponent<ScorePoints>(entity);
   // m_Score += scorePoints.points;
    System::OnEntityDelete(entity);
}

double ScoreSystem::GetScore()
{
    return m_Score;
}

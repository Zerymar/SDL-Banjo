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

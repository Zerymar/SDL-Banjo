#include "ScoreSystem.h"

void ScoreSystem::Init()
{
    m_Score = 0;
}

void ScoreSystem::Update()
{
}

void ScoreSystem::OnEntityDelete(Entity entity)
{
    System::OnEntityDelete(entity);
}

void ScoreSystem::IncrementScore(double points)
{
    m_Score += points;
}

double ScoreSystem::GetScore()
{
    return m_Score;
}

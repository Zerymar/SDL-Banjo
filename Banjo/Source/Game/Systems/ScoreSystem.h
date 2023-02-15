#pragma once
#include "../../Core/Coordinator.hpp"
#include "../../Core/Systems/System.hpp"


class ScoreSystem : public System
{
public:
    void Init();
    void Update();
    void OnEntityDelete(Entity entity) override;
    double GetScore();
    
protected:
    int m_Score;
};

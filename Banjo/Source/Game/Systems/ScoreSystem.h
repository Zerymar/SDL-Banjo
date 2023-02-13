#pragma once
#include <bitset>
#include <SDL_events.h>
#include <SDL_mixer.h>
#include "../../Core/Coordinator.hpp"
#include "../../Core/Systems/System.hpp"
#include "../../Utility/Math/Geometry.hpp"
#include "../../Utility/Math/Vector2.h"

class ScoreSystem : public System
{
public:
    void Init();
    void Update();
    void OnEntityDelete(Entity entity) override;
    void IncrementScore(double points);
    double GetScore();
    
protected:
   static double m_Score;
};

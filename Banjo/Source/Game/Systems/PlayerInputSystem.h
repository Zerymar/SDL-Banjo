#pragma once
#include <bitset>
#include <SDL_events.h>
#include <SDL_mixer.h>
#include "../../Core/Coordinator.hpp"
#include "../../Core/Systems/System.hpp"
#include "../../Utility/Math/Geometry.hpp"
#include "../../Utility/Math/Vector2.h"

class PlayerInputSystem : public System
{
public:
   void Init(std::vector<Mix_Chunk*> laserExplosions);
   void Update();
   void CreatePlayerProjectile();
   void HandleInput(const SDL_Event& event);
   void MouseButtonEvent(const SDL_MouseButtonEvent& button);
   void OnEntityDelete(Entity entity) override;

    

private:
    float m_ScalarVelocity = 5;
    bool m_bIsAccelerating = false;
    ORIENTATION m_Orientation = Collinear;
    void HandleRotation(Entity entity, ORIENTATION orientation);
    void RotateShape(std::vector<SDL_FPoint>& vertices, float theta);
    void ScaleRotatedPoints(const std::vector<SDL_FPoint>& prevVertices,
        std::vector<SDL_FPoint>& currentVertices, const SDL_FPoint& centerPoint);
    Vector2 GetPlayerPointDirection(const Entity& entity);
    std::vector<Mix_Chunk*> m_LaserExplosions;
    int m_laserIndex = 0;

    bool m_FireProjectile = false;
};

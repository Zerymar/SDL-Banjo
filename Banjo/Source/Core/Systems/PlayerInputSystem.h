#pragma once
#include <bitset>
#include <SDL_events.h>


#include "System.hpp"
#include "../../Utility/Math/Geometry.hpp"

class PlayerInputSystem : public System
{
public:
   void Init();
   void Update();
   void CreatePlayerProjectile();
   void HandleInput(const SDL_Event& event);
   void MouseButtonEvent(const SDL_MouseButtonEvent& button);
   void OnEntityDelete(Entity entity) override;

    

private:
    int m_ScalarVelocity = 5;
    ORIENTATION m_Orientation = Collinear;
    void HandleRotation(Entity entity, ORIENTATION orientation);
    void RotateShape(std::vector<SDL_Point>& vertices, double theta);
    void ScaleRotatedPoints(const std::vector<SDL_Point>& prevVertices,
        std::vector<SDL_Point>& currentVertices, const SDL_Point& centerPoint);
};

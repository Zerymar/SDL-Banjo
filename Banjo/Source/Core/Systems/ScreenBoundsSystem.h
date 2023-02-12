#pragma once

#include "System.hpp"


// Our system to ensure:
// Projectiles are deleted when out of bounds
// Players are contained in bounds
class ScreenBoundsSystem : public System
{
public:
    void Init();
    void UpdatePositions();
    
};

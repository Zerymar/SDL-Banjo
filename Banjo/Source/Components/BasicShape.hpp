#pragma once
#include <SDL_rect.h>
#include <vector>
#include "../Utility/Math/Vector3.h"

struct BasicShape
{
    std::vector<SDL_FPoint> m_Vertices;
    Vector3 color;
};

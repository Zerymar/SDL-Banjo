#pragma once
#include <cstdlib>
#include <algorithm>
#include <iterator>
#include "defs.h"
#include "../Components/Transform.hpp"

class Util
{
public:
    template <typename I>
    static I Random_Iterator(I begin, I end)
    {
        const unsigned long n = std::distance(begin, end);
        const unsigned long divisor = (RAND_MAX + 1) / n;

        unsigned long k;
        do { k = std::rand() / divisor; } while (k >= n);

        std::advance(begin, k);
        return begin;
    }

    template <typename T>
    static T Random_Element(std::vector<T> vector)
    {
        auto iterator = Random_Iterator(vector.begin(), vector.end());
        T element = nullptr;
        for(iterator;iterator< vector.end(); ++iterator )
        {
            element = *iterator;
            break;
        }
        return element;
    }

    static bool IsOutOfBounds(Transform transformComponent)
    {
        int min_deadzone_x = 0 - OFFSCREEN_DEAD_ZONE;
        int max_deadzone_x = SCREEN_WIDTH + OFFSCREEN_DEAD_ZONE;

        int min_deadzone_y = 0 - OFFSCREEN_DEAD_ZONE;
        int max_deadzone_y = SCREEN_HEIGHT + OFFSCREEN_DEAD_ZONE;

        return transformComponent.position.x < min_deadzone_x || transformComponent.position.x > max_deadzone_x
            ||transformComponent.position.y < min_deadzone_y || transformComponent.position.y > max_deadzone_y;
    }
};

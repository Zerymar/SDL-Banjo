#pragma once
#include <cstdlib>
#include <algorithm>
#include <iterator>

static class Util
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
};

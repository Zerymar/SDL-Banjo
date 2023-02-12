#pragma once
#include <bitset>
#include <cstdint>

// Entity is an alias for uint32_t
// Each component has a unique "ID"
// Say Transform,RigidBody, and Gravity are components (1,2,3 respectively)
// - The Signatuve would be 0b111 (bits 0,1,2 are set)
// Entity might have more components than a system BUT we want to ensure we have every type of component the
// system cares about

using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 5000;

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;


// Source: https://gist.github.com/Lee-R/3839813
constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
{
    return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u; // NOLINT (hicpp-signed-bitwise)
}

constexpr std::uint32_t operator "" _hash(char const* s, std::size_t count)
{
    return fnv1a_32(s, count);
}


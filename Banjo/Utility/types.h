#include <bitset>
#include <cstdint>

// Entity is an alias for uint32_t
using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 5000;

// Create our component alias
// Component can be a small struct of functionality related data
using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;

// Each component has a unique "ID"
// Say Transform,RigidBody, and Gravity are components (1,2,3 respectively)
// - The Signatuve would be 0b111 (bits 0,1,2 are set)
// Entity might have more components than a system BUT we want to ensure we have every type of component the
// system cares about

using Signature = std::bitset<MAX_COMPONENTS>;

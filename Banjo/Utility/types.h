#include <bitset>
#include <cstdint>

#include "defs.h"

// Entity is an alias for uint32_t
// Each component has a unique "ID"
// Say Transform,RigidBody, and Gravity are components (1,2,3 respectively)
// - The Signatuve would be 0b111 (bits 0,1,2 are set)
// Entity might have more components than a system BUT we want to ensure we have every type of component the
// system cares about

using Entity = std::uint32_t;
const Entity MAX_ENTITIES = MAX_ENTITIES_LIMIT;

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = MAX_COMPONENTS_LIMIT;

using Signature = std::bitset<MAX_COMPONENTS>;

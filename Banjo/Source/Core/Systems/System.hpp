#pragma once

#include "../../Utility/types.hpp"
#include <set>

class System
{
public:
    std::set<Entity> m_Entities;
    virtual void OnEntityDelete(Entity entity){}
};
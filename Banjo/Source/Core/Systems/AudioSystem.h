#pragma once
#include "System.hpp"


class AudioSystem : public System
{
public:
    void Init();
    void Update();
    void OnEntityDelete(Entity entity) override;
    
};

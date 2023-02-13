#pragma once
#include <SDL_mixer.h>
#include "System.hpp"


class AudioSystem : public System
{
public:
    static void PlaySound(Mix_Chunk* sound, int channel);
    void Init();
    void Update();
    void OnEntityDelete(Entity entity) override;
    
};

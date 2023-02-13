#include "AudioSystem.h"
#include "../Coordinator.hpp"
#include "../../Components/SFX.hpp"
#include "../../Components/Transform.hpp"
#include "../../Utility/Util.hpp"


extern Coordinator m_Coordinator;

void AudioSystem::PlaySound(Mix_Chunk* sound, int channel)
{
    Mix_PlayChannel(channel, sound, 0);
}

void AudioSystem::Init()
{
}

void AudioSystem::Update()
{
}

void AudioSystem::OnEntityDelete(Entity entity)
{
    System::OnEntityDelete(entity);
}



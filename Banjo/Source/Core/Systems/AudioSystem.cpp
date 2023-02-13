#include "AudioSystem.h"
#include "../Coordinator.hpp"
#include "../../Components/SFX.hpp"


extern Coordinator m_Coordinator;
void AudioSystem::Init()
{

}

void AudioSystem::Update()
{
    for(auto const &entity : m_Entities)
    {
        auto& sfxComponent = m_Coordinator.GetComponent<SFX>(entity);
        if(!sfxComponent.m_bSpawnPlayed && sfxComponent.SpawnSFX!= nullptr)
        {
            Mix_PlayChannel( -1, sfxComponent.SpawnSFX, 0 );
            sfxComponent.m_bSpawnPlayed = true;
        }
    }
}

void AudioSystem::OnEntityDelete(Entity entity)
{
    
    for(auto const &entity : m_Entities)
    {
        auto& sfxComponent = m_Coordinator.GetComponent<SFX>(entity);
        if(sfxComponent.DestroySFX != nullptr)
        {
            Mix_PlayChannel( -1, sfxComponent.DestroySFX, 0 );
        }
    }
    
    System::OnEntityDelete(entity);
}

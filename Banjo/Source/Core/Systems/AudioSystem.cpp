#include "AudioSystem.h"
#include "../Coordinator.hpp"
#include "../../Components/SFX.hpp"
#include "../../Components/Transform.hpp"
#include "../../Utility/Util.hpp"


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
    
    for(auto const &otherEntity : m_Entities)
    {
        auto& sfxComponent = m_Coordinator.GetComponent<SFX>(otherEntity);
        if(sfxComponent.DestroySFX != nullptr)
        {
            Mix_PlayChannel( 0, sfxComponent.DestroySFX, 0 );
        }
    }
    
    System::OnEntityDelete(entity);
}

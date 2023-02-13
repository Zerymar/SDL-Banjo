#include <SDL_mixer.h>

struct SFX
{
    Mix_Chunk* DestroySFX;
    Mix_Chunk* SpawnSFX;
    bool m_bSpawnPlayed =false;
};

#include <SDL.h>
#include "Game/Game.h"
#include "Core/Coordinator.hpp"

Coordinator m_Coordinator;

int main(int argc, char* argv[])
{
    Game game_app;

    if (game_app.init())
    {
        game_app.run();
    }

    return 0;
}

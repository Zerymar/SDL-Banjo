#include <SDL.h>
#include "Core/Game.h"

int main(int argc, char* argv[])
{
    Game game_app;

    if (game_app.init())
    {
        game_app.run();
    }

    return 0;
}

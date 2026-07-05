#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#include "core/Game.h"

int main()
{
    Game game;
    game.run();

    return 0;
}

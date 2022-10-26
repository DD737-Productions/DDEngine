#include "Definitions.h"
#include "Boolisting.hpp"

int main()
{

    Game game;

    game.Init(TITLE, WIDTH, HEIGHT, Color(BGR, BGG, BGB, BGA), FULLSCREEN);

    Init(&game);

    game.Run();
    game.Clean();

    return 0;

}

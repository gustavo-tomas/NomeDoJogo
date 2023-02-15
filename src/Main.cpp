#include "../header/Game.h"
// #include "../header/TitleState.h"
#include "../header/CreditState.h"

int main(int argc, char** argv)
{
    Game& game = Game::GetInstance();
    game.Push(new CreditState());
    game.Run();
    return 0;
}

// @TODO: refactor Resources! no :(

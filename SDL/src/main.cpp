#include"Game.h"
#include <stdio.h>
int main(int argc, char* args[]) {
	Game::GetInstance()->Run();
	Game::GetInstance()->FreeInstance();
	return 0;
}
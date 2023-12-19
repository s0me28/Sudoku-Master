#pragma once

#include <SFML\Graphics.hpp>
#include <string>
#include "Engine.h"

using namespace std;
using namespace sf;

class Game
{
public:
	Game();
	void runGame();
private:
	Font font;
	enum GameState { MENU, GAME, HELP, END };
	GameState state;
	void levels();
	void help();
	void menu();
};

	


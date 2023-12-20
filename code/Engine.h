#pragma once

#include "Game.h"
#include <string>

using namespace std;
using namespace sf;

struct Input {
	int val;
	string value;
	Text cell;
};

class Engine
{
	public:
	 Engine();
	 ~Engine();
	 bool runEngine(RenderWindow &window ,int level);
	private:
	 Font font;
	 Texture texture;
	 enum engineState { GAME, GAMEOVER };
	 engineState state;
	 int CellSize, FontSize;
	 void fillTable(int **m, int &size, RenderWindow &window);
	 void setLevel(int **m, bool **b, int size, int difficulty);
	 void selectCell(RenderWindow &window, int size, int &index, int **matrice, bool **block, Input &input);
	 void drawSquare(RenderWindow &window, int **matrice, bool **block, int size, int index);
	 bool checkRow(int **m, int size, int row, int value);
	 bool checkColumn(int **m, int size, int column, int value);
	 bool checkSquare(int **m, int size, int row, int column, int value);
	 bool checkFalse(bool verificbool[9], int marime);
	 bool checkWin(int **m, int size);
};




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
	 void fillTable(int m[20][20], int size, RenderWindow &window);
	 void setLevel(int m[20][20], bool b[20][20], int size, int difficulty);
	 void selectCell(RenderWindow &window, int size, int index, int matrix[20][20], bool block[20][20], Input input);
	 void drawSquare(RenderWindow &window, int matrix[20][20], bool bloc[20][20], int size, int index);
	 bool checkRow(int m[20][20], int size, int row, int value);
	 bool checkColumn(int m[20][20], int size, int column, int value);
	 bool checkSquare(int m[20][20], int size, int row, int column, int value);
	 bool toremove(bool bol[16], int t);
	 bool checkWin(int m[20][20], int size);
};




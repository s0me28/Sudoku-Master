#include "Engine.h"

sf::Clock timer;
float timeLimit;

Engine::Engine()
{
	timer.restart();
    timeLimit = 300.0f;
	srand(time(NULL));

	font.loadFromFile("arialceb.ttf");
	texture.loadFromFile("white.jpg");
	
	Game game;	
	state = GAME;
}

Engine::~Engine()
{
}

bool Engine::runEngine(RenderWindow &window, int level)
{
	Sprite background(texture);
	int size=9,index=0,m[20][20];// in m am valorile din sudoku
	bool b[20][20]; //in b am valori boolean pe fiecare valoare din sudoku sa vad daca tre completat
	fillTable(m, size, window);
	setLevel(m, b, size, level);

	Input input;
	input.val = 0;
	input.value = "";

	CellSize = 40;
	FontSize = 20;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event) && state == GAME)
		{
			switch (event.type)
			{
			case Event::Closed:
				curat(m,b,size);
				window.close();
				break;

			case Event::KeyPressed:

				if ((Keyboard::isKeyPressed(Keyboard::Escape)))
				{
					curat(m,b,size);
					return false;
				}
				selectCell(window, size, index, m, b, input);
				break;

			default:
				break;
			}
		}

		window.clear();
		window.draw(background);

		drawSquare(window, m, b, size, index);

		input.cell.setCharacterSize(20);
		input.cell.setColor(Color(80, 80, 80));
		input.cell.setFont(font);
		input.cell.setString(input.value);
		input.cell.setPosition(10, 10);
		input.cell.setPosition((400 - size / 2 * CellSize) + index%size*CellSize + FontSize / 2, (300 - size / 2 * CellSize) + index / size*CellSize + FontSize / 2);

		window.draw(input.cell);

		if (checkWin(m, size))
			state = GAMEOVER;

		if (state == GAMEOVER)
		{
			string over = "Bravo!";
			Text overAfis(over, font, 60);
			overAfis.setPosition(270, 50);
			overAfis.setColor(Color(80, 80, 80));
			window.draw(overAfis);
			timer.restart();   //aici o sa adaug numele si timer-ul in fisier

			if ((Keyboard::isKeyPressed(Keyboard::Escape)))
			{
				curat(m,b,size);
				return false;
			}

		}
		window.display();	
	}
	return true;	
}
		
bool Engine::checkRow(int m[20][20], int size, int row, int value)
{
	for (int i = 0; i<size; i++)
	{
		if (m[row][i] == value)
		{
			return false;
		}
	}
	return true;
}

bool Engine::checkColumn(int m[20][20], int size, int column, int value)
{
	for (int i = 0; i<size; i++)
	{
		if (m[i][column] == value)
		{
			return false;
		}
	}
	return true;
}

bool Engine::checkSquare(int m[20][20], int size, int row, int column, int value)
{
	int quadx = 3, quady = 3;

	int sizey = row / quady;
	int sizex = column / quadx;

	sizey = (sizey*quady);
	sizex = (sizex*quadx);

	for (int i = sizey; i<sizey + quady; i++)
	{
		for (int j = sizex; j<sizex + quadx; j++)
		{
			if (m[i][j] == value)
			{
				return false;
			}
		}
	}
	return true;

}

bool Engine::toremove(bool bol[9], int t)
{
	for (int i = 0; i<t; i++)
	{
		if (bol[i])
		{
			return false;
		}
	}
	return true;
}

void Engine::selectCell(RenderWindow &window, int size,int index, int matrix[20][20], bool block[20][20], Input input)
{
	if(Keyboard::isKeyPressed(Keyboard::Left))
	{
		index--;
		if(index<0)
		{
			index=size*size-1;
		}
		input.value="";
	}

	if(Keyboard::isKeyPressed(Keyboard::Right))
	{
		index++;
		if(index>size*size-1)
		{
			index=0;
		}
		input.value="";
	}
	if(Keyboard::isKeyPressed(Keyboard::Up))
	{
		index-=size;
		if(index<0)
		{
			index=((size*size))-size+((index+size)%size);
		}
		input.value="";
	}

	if(Keyboard::isKeyPressed(Keyboard::Down))
	{
		index+=size;
		if(index>size*size-1)
		{
			index=index%size;
		}
		input.value="";
	}
	if(!block[index/size][index%size] && input.value.size()<=1)
	{
		if(Keyboard::isKeyPressed(Keyboard::Num0) || Keyboard::isKeyPressed(Keyboard::Numpad0))
		{
			input.value+='0';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num1) || Keyboard::isKeyPressed(Keyboard::Numpad1))
		{
			input.value+='1';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num2) || Keyboard::isKeyPressed(Keyboard::Numpad2))
		{
			input.value+='2';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num3) || Keyboard::isKeyPressed(Keyboard::Numpad3))
		{
			input.value+='3';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num4) || Keyboard::isKeyPressed(Keyboard::Numpad4))
		{
			input.value+='4';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num5) || Keyboard::isKeyPressed(Keyboard::Numpad5))
		{
			input.value+='5';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num6) || Keyboard::isKeyPressed(Keyboard::Numpad6))
		{
			input.value+='6';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num7) || Keyboard::isKeyPressed(Keyboard::Numpad7))
		{
			input.value+='7';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num8) || Keyboard::isKeyPressed(Keyboard::Numpad8))
		{
			input.value+='8';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num9) || Keyboard::isKeyPressed(Keyboard::Numpad9))
		{
			input.value+='9';
		}
	}
	if(!block[index/size][index%size])
	{
		if(Keyboard::isKeyPressed(Keyboard::Return))
		{
			input.val = atoi(input.value.c_str());

			bool correct;
			
			correct=checkColumn(matrix,size,index%size,input.val) && checkRow(matrix,size,index/size,input.val) && checkSquare(matrix,size,index/size,index%size, input.val);

			if(input.val>0 && input.val<=size && correct)
			{
				matrix[index/size][index%size]=input.val;
				input.value="";
			}
			else
			{
				input.value="";
			}
		}
	}
}

void Engine::drawSquare(RenderWindow &window, int matrix[20][20],bool block[20][20], int size, int index)
{
	float elapsedSeconds = timer.getElapsedTime().asSeconds();
    string timerText = "Time: " + to_string(static_cast<int>(elapsedSeconds)) + "s";
    Text timerDisplay(timerText, font, 20);
    timerDisplay.setPosition(10, 10);
    timerDisplay.setColor(Color(80, 80, 80));
    window.draw(timerDisplay);

	int quadx = 3, quady = 3;
		
	int cellSize = 40, fontSize = 20;

	Font font;
	font.loadFromFile("arialceb.ttf");

	RectangleShape mediumSquare;
	mediumSquare.setSize(Vector2f(cellSize*quadx, cellSize*quady));
	mediumSquare.setOutlineThickness(2);
	mediumSquare.setOutlineColor(sf::Color::Black);
	mediumSquare.setFillColor(sf::Color::Transparent);

	RectangleShape bigSquare;
	bigSquare.setSize(Vector2f(cellSize*size, cellSize*size));
	bigSquare.setOutlineThickness(4);
	bigSquare.setOutlineColor(sf::Color::Black);
	bigSquare.setFillColor(sf::Color::Transparent);
	bigSquare.setPosition((400-size/2*cellSize), (300-size/2*cellSize));

	RectangleShape smallSquare;
	smallSquare.setSize(Vector2f(cellSize, cellSize));
	smallSquare.setOutlineThickness(1);
	smallSquare.setOutlineColor(Color(200,200,200));
	smallSquare.setFillColor(sf::Color::Transparent);

	for(int i=0; i<size; i++)
	{
		for(int j=0; j<size; j++)
		{
			smallSquare.setPosition((400-size/2*cellSize)+j*cellSize,(300-size/2*cellSize)+i*cellSize);
			window.draw(smallSquare);
			string text;
			if(matrix[i][j]>0)
			{
				text = to_string(matrix[i][j]);
			}
			else
			{
				text="";
			}
			if(index%size==j && index/size==i)
			{
				smallSquare.setFillColor(Color(0,255,0,210));
				smallSquare.setPosition((400-size/2*cellSize)+j*cellSize,(300-size/2*cellSize)+i*cellSize);
				window.draw(smallSquare);
				smallSquare.setFillColor(sf::Color::Transparent);
			}

			Text cell(text, font, fontSize);
			cell.setPosition((400-size/2*cellSize)+j*cellSize+fontSize/2, (300-size/2*cellSize)+i*cellSize+fontSize/2);
			if(block[i][j])
			{
				cell.setColor(Color(80, 80, 80));
			}
			else
			{
				cell.setColor(Color::Blue);
			}


			window.draw(cell);
		}
	}

	for(int i=0; i<size; i++)
	{
		for(int j=0; j<size; j++)
		{
			if(i%quady==0 && j%quadx==0)
			{
				mediumSquare.setPosition((400-size/2*cellSize)+j*cellSize,(300-size/2*cellSize)+i*cellSize);
				window.draw(mediumSquare);
			}
		}
	}
	window.draw(bigSquare);
}

void Engine::fillTable(int m[20][20], int size, RenderWindow &window)
{
	int valoare[9];
	bool valoareBoolean[9];

	for (int i = 0; i<9; i++)
	{
		valoare[i] = i + 1;
		valoareBoolean[i] = true;
	}

	int valueRow = (rand() % size); ///0-8
	int reset = 0;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			
			if (valoareBoolean[valueRow])
			{
				bool ok;
				ok = checkRow(m, size, i, valoare[valueRow]) && checkColumn(m, size, j, valoare[valueRow]) && checkSquare(m, size, i, j, valoare[valueRow]);
				if (ok)
				{
					m[i][j] = valoare[valueRow];
					for (int i = 0; i<9; i++)
					{
						valoareBoolean[i] = true;
					}
					valueRow = (rand() % size);
				}
				else
				{
					valoareBoolean[valueRow] = false;
					j--;
					valueRow = (valueRow + 1) % size;
				}
			}
			else
			{
				valueRow = (valueRow + 1) % size;
				j--;
				if(toremove(valoareBoolean, size))
				{
					for (int x = 0; x<9; x++)
					{
						valoareBoolean[x] = true;
						m[i][x] = 0;
					}
					reset++;
					j = -1;
					if (reset>1000)
					{
						i = -1;
						j = -1;
						reset = 0;
						for (int i = 0; i < size; i++)
						{
							for (int j = 0; j < size; j++)
							{
								m[i][j] = 0;
							}
						}
						break;
					}
				}
			}
		}
	}

}

void Engine::setLevel(int m[20][20], bool b[20][20], int size, int difficulty)
{
	int amount;
	switch (difficulty)
	{
	case 0:
		amount = 2;
		break;
	case 1:
		amount = 3;
		break;
	case 2:
		amount = 5;
		break;
	default:
		amount = 2;
		break;
	}
	int numberCells = (size*size);
	int left = numberCells / amount;
	int deleted = numberCells - left;

	int row = rand() % size;
	int column = rand() % size;
	for (int i = 0; i<deleted; i++)
	{
		if (m[row][column] > 0)
		{
			m[row][column] = 0;
			b[row][column] = false;
		}
		else
		{
			row = rand() % size;
			column = rand() % size;
			i--;
		}
	}
}

bool Engine::checkWin(int m[20][20], int size)
{
	for (int i = 0; i<size; i++)
	{
		for (int j = 0; j<size; j++)
		{
			if (m[i][j] == 0)
			{
				return false;
			}
		}
	}
	return true;
}

void curat(int m[20][20],bool b[20][20],int size)
{
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<size;j++)
		{
			m[i][j]=0;
			b[i][j]=true;
		}
	}
}
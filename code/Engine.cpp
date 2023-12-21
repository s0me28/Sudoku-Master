#include "Engine.h"

Engine::Engine()
{
	timer.restart();
    timeLimit = 300.0f;
	srand(time(NULL));

	font.loadFromFile("arialceb.ttf");
	texture.loadFromFile("white.jpg");	
	state = GAME;
}

Engine::~Engine()
{
}

bool Engine::runEngine(RenderWindow &window, int level)
{
	Sprite background(texture);
	int size = 9;
	int index = 0;  //unde sunt in joc 0-80

	int **m = new int*[20];
	m[0] = new int[20 * 20];

	for (int i = 1; i<20; i++)
	{
		m[i] = m[i - 1] + 20;
	}

	bool **b = new bool*[20];
	b[0] = new bool[20 * 20];

	for (int i = 1; i<20; i++)
	{
		b[i] = b[i - 1] + 20;
	}

	for (int i = 0; i<20; i++)
	{
		for (int j = 0; j<20; j++)
		{
			b[i][j] = true;
		}

	}

	fillTable(m, size, window);
	setLevel(m, b, size, level);

	Input input;
	input.val = 0;   //int pentru verificare o sa fac atoi din string
	input.value = ""; //string 

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
				delete[]m[0];
				delete[]b[0];
				window.close();
				break;

			case Event::KeyPressed:

				if ((Keyboard::isKeyPressed(Keyboard::Escape)))
				{
					delete[]m[0];
					delete[]b[0];
					scor=0;
				    vieti=3;
				    timer.restart();
				    helper="Helper: ";
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
        
		float elapsedSeconds = timer.getElapsedTime().asSeconds();
    	string timerText = "Time: " + to_string(static_cast<int>(elapsedSeconds)) + "s";
    	Text timerDisplay(timerText, font, 20);
    	timerDisplay.setPosition(10, 10);
    	timerDisplay.setColor(Color(80, 80, 80));
    	window.draw(timerDisplay);

    	string scorText = "Scor: " + to_string(scor);
    	Text scorDisplay(scorText, font, 20);
    	scorDisplay.setPosition(10, 30);
    	scorDisplay.setColor(Color(80, 80, 80));
    	window.draw(scorDisplay);

		string vietiText = "Vieti: " + to_string(vieti);
    	Text vietiDisplay(vietiText, font, 20);
    	vietiDisplay.setPosition(10, 50);
    	vietiDisplay.setColor(Color(80, 80, 80));
    	window.draw(vietiDisplay);

		Text helperDisplay(helper, font, 20);
    	helperDisplay.setPosition(10, 90);
    	helperDisplay.setColor(Color(80, 80, 80));
    	window.draw(helperDisplay);

		drawSquare(window, m, b, size, index);      //trebuie sa pun draw Square in Game

		input.cell.setCharacterSize(20);            // aici adaug valoarea in celula
		input.cell.setColor(Color(80, 80, 80));
		input.cell.setFont(font);
		input.cell.setString(input.value);
		input.cell.setPosition(10, 10);
		input.cell.setPosition((400 - size / 2 * CellSize) + index%size*CellSize + FontSize / 2, (300 - size / 2 * CellSize) + index / size*CellSize + FontSize / 2);

		window.draw(input.cell);

		if(vieti==0)
		 state = GAMEOVER;

		if (checkWin(m, size))
			state = GAMEOVER;

		if (state == GAMEOVER)
		{
			string over = "Bravo!";
			Text overAfis(over, font, 60);
			overAfis.setPosition(300, 50);
			overAfis.setColor(Color(80, 80, 80));
			window.draw(overAfis);
			timer.restart();   

			if ((Keyboard::isKeyPressed(Keyboard::Escape)))
			{
				delete[]m[0];
				delete[]b[0];
				scor=0;
				vieti=3;
				timer.restart();
				helper="Helper: ";
				return false;
			}

		}
		window.display();	
	}
	return true;	
}
		
bool Engine::checkRow(int **m, int size, int row, int value)
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

bool Engine::checkColumn(int **m, int size, int column, int value)
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

bool Engine::checkSquare(int **m, int size, int row, int column, int value)
{
	int quadx = 3, quady = 3;   //un patrat e 3x3

	int sizey = row / quady;    //coordonatele patratului 0 1 2
	int sizex = column / quadx;

	sizey = (sizey*quady);      //stanga-sus din patrat si parcurg in el
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

bool Engine::checkFalse(bool verificbool[9], int marime) //verific daca tot sirul e false => sudoku nerezolvabil
{
	for (int i = 0; i<marime; i++)
	{
		if (verificbool[i])
		{
			return false;
		}
	}
	return true;
}

void Engine::selectCell(RenderWindow &window, int size,int &index, int **matrice, bool **block, Input &input)
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
		if(Keyboard::isKeyPressed(Keyboard::Enter))
		{
			input.val = atoi(input.value.c_str());

			bool correct;
			correct=checkColumn(matrice,size,index%size,input.val) && checkRow(matrice,size,index/size,input.val) && checkSquare(matrice,size,index/size,index%size,input.val);

			if(input.val>0 && input.val<=size && correct)
			{
				matrice[index/size][index%size]=input.val;
				scor+=5;
				input.value="";
				helper="Helper: ";
			}
			else
			{
				vieti--;
				scor-=5;
				if(scor<0)
				 scor=0;
				helper = "Helper: Mistake on (" + to_string(index/size)+","+to_string(index%size) + ") !";
				input.value="";
			}
		}
	}
}

void Engine::drawSquare(RenderWindow &window, int **matrice,bool **block, int size, int index)
{

	int quadx = 3, quady = 3;	
	int cellSize = 40, fontSize = 20;

	Font font;
	font.loadFromFile("arialceb.ttf");

	RectangleShape mediumSquare;
	mediumSquare.setSize(Vector2f(cellSize*quadx, cellSize*quady)); // 3x3 
	mediumSquare.setOutlineThickness(2);
	mediumSquare.setOutlineColor(sf::Color::Black);
	mediumSquare.setFillColor(sf::Color::Transparent);

	RectangleShape bigSquare; // 9 x 9
	bigSquare.setSize(Vector2f(cellSize*size, cellSize*size));
	bigSquare.setOutlineThickness(4);
	bigSquare.setOutlineColor(sf::Color::Black);
	bigSquare.setFillColor(sf::Color::Transparent);
	bigSquare.setPosition((400-size/2*cellSize), (300-size/2*cellSize));

	RectangleShape smallSquare;
	smallSquare.setSize(Vector2f(cellSize, cellSize));
	smallSquare.setOutlineThickness(1);
	smallSquare.setOutlineColor(Color(200,200,200)); // alb
	smallSquare.setFillColor(sf::Color::Transparent);

	for(int i=0; i<size; i++)
	{
		for(int j=0; j<size; j++)
		{
			smallSquare.setPosition((400-size/2*cellSize)+j*cellSize,(300-size/2*cellSize)+i*cellSize);
			window.draw(smallSquare);
			string text;
			if(matrice[i][j]>0)   // pastrez valori valide, unde e 0 sterg (e 0 doar la reset)
			{
				text = to_string(matrice[i][j]);
			}
			else
			{
				text="";
			}
			if(index%size==j && index/size==i)
			{
				smallSquare.setFillColor(Color(0,255,0,210)); // verde deschis si transparent sa se vada numerele
				smallSquare.setPosition((400-size/2*cellSize)+j*cellSize,(300-size/2*cellSize)+i*cellSize);
				window.draw(smallSquare);
				smallSquare.setFillColor(sf::Color::Transparent);
			}

			Text cell(text, font, fontSize); //text de mai sus scrie cu gri numarul temporar
			cell.setPosition((400-size/2*cellSize)+j*cellSize+fontSize/2, (300-size/2*cellSize)+i*cellSize+fontSize/2);
			if(block[i][j]) //daca e false inseamna ca se poate edita
			{
				cell.setColor(Color(80, 80, 80)); //gri
			}
			else
			{
				cell.setColor(Color::Blue);
			}
			window.draw(cell);
		}
	}

	for(int i=0; i<size; i++) // 0 3 6
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

void Engine::fillTable(int **m, int &size, RenderWindow &window)
{
	int valoareint[9];
	bool valoarebool[9];
	for (int i = 0; i<9; i++)
	{
		valoareint[i] = i + 1;    //sir de la 1-9 de adaugat in matrice
		valoarebool[i] = true;    //temporar valorile sunt corecte
	}

	int valoare = (rand() % size); // ceva % 9 == numerele de la 0-8
	int reset = 0;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (valoarebool[valoare]) //testam corectitudinea 
			{
				bool check;
				check = checkRow(m, size, i, valoareint[valoare]) && checkColumn(m, size, j, valoareint[valoare]) && checkSquare(m, size, i, j, valoareint[valoare]);
				if (check)
				{
					m[i][j] = valoareint[valoare]; // daca poate fi pusa si e si corecta o salvam in matrice, resetam sirul boolean si sarim la alta valoare
					for (int i = 0; i<9; i++)
					{
						valoarebool[i] = true;
					}
					valoare = (rand() % size);
				}
				else
				{
					valoarebool[valoare] = false; //daca poate fi pusa,dar nu e corecta
					j--;
					valoare = (valoare + 1) % size;
				}
			}        
			else //daca intra pe else inseamna ca nu exista valori corecte => trebuie sa resetam ca nu o sa genereze un sudoku rezolvabil
			{
				valoare = (valoare + 1) % size; 
				j--;
				if (checkFalse(valoarebool, size)) // verific daca tot sirul e false (nu am ce valori sa mai pun ca le-am verificat si nu sunt bune) => sudoku nerezolvabil
				{
					for (int x = 0; x<9; x++)   // pune 0 pe toata coloana (o resetez)
					{
						valoarebool[x] = true;
						m[i][x] = 0;
					}
					reset++;
					j = -1;
					if (reset>1000) //full reset
					{
						i = -1;
						j = -1;
						reset = 0;
						for (int i2 = 0; i2 < size; i2++)
						{
							for (int j2 = 0; j2 < size; j2++)
							{
								m[i2][j2] = 0;
							}
						}
						break;
					}
				}
			}
		}
	}

}

void Engine::setLevel(int **m, bool **b, int size, int difficulty)
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
	int left = numberCells / amount; //cate raman
	int deleted = numberCells - left; //cate trebuie sterse

	int row = rand() % size;
	int column = rand() % size;
	for (int i = 0; i<deleted; i++)
	{
		if (m[row][column] > 0) //il ascund
		{
			m[row][column] = 0;
			b[row][column] = false;
		}
		else
		{
			row = rand() % size; //mai baga o fisa la random
			column = rand() % size;
			i--;
		}
	}
}

bool Engine::checkWin(int **m, int size)
{
	for (int i = 0; i<size; i++)
	{
		for (int j = 0; j<size; j++)
		{
			if (m[i][j] == 0) // daca e 0 => ascuns
			{
				return false;
			}
		}
	}
	return true;
}

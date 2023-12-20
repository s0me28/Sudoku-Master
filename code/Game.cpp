#include "Game.h"

RenderWindow window(VideoMode(800, 600), "Sudoku", sf::Style::Titlebar | sf::Style::Close);


Game::Game()
{
	state = END;

	if (!font.loadFromFile("arialceb.ttf"))
		return;

	state = MENU;
}
Game::~Game()
{

}

void Game::runGame()
{
	while (state != END)
	{
		switch (state)
		{
		case GameState::MENU:
			menu();
			break;
		case GameState::HELP:
			help();
			break;
		case GameState::GAME:
			levels();
			break;
		}		
	}
}


void Game::menu()
{
	Text title("Sudoku", font, 80);
	title.setStyle(Text::Bold);

	title.setPosition(800 / 2 - title.getGlobalBounds().width / 2, 60);

	const int n = 3;

	Text tab[n];

	string str[] = { "Play", "Help","Exit" };
	for (int i = 0; i<n; i++)
	{
		tab[i].setFont(font);
		tab[i].setCharacterSize(65);

		tab[i].setString(str[i]);
		tab[i].setPosition(800 / 2 - tab[i].getGlobalBounds().width / 2, 200 + i * 120);
	}

	while (state == MENU)
	{
		Vector2f mouse(Mouse::getPosition(window));
		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed || event.type == Event::KeyPressed &&
				event.key.code == Keyboard::Escape)
				state = END;                                        
			else if (tab[0].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = GAME;
			}
			else if (tab[1].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = HELP;
			}
			else if (tab[2].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = END;
			}
		}
		for (int i = 0; i<n; i++)
			if (tab[i].getGlobalBounds().contains(mouse))
				tab[i].setColor(Color::Green);
			else tab[i].setColor(Color::White);

			window.clear();

			window.draw(title);
			for (int i = 0; i<n; i++)
				window.draw(tab[i]);

			window.display();
	}
}

void Game::levels()
{
	sf::Text title("Levels", font, 90);
	title.setStyle(sf::Text::Bold);

	title.setPosition(800 / 2 - title.getGlobalBounds().width / 2, 30);
	title.setStyle(sf::Text::Bold);
	
	string easy, medium, hard;
	const int n = 4;

	sf::Text tab[n];

	std::string level[] = { "Easy","Medium","Hard", "back" };
	for (int i = 0; i<n; i++)
	{
		tab[i].setFont(font);
		tab[i].setCharacterSize(65);
		tab[i].setString(level[i]);
		tab[i].setPosition(800 / 2 - tab[i].getGlobalBounds().width / 2, 150 + i * 120);
	}


	while (state == GAME && window.isOpen())
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		sf::Event event;
		Engine engine;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (tab[0].getGlobalBounds().contains(mouse) && event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				if (engine.runEngine(window,0))
					state = GAME;
				else
					state = MENU;
			}

			else if (tab[1].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				if (engine.runEngine(window, 1))
					state = GAME;
				else
					state = MENU;
			}

			else if (tab[2].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				if (engine.runEngine(window, 2))
					state = GAME;
				else
					state = MENU;
			}

			else if (tab[3].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				state = MENU;
			}
		}

		for (int i = 0; i<n; i++)
			if (tab[i].getGlobalBounds().contains(mouse))
				tab[i].setColor(Color::Green);
			else tab[i].setColor(Color::White);


		window.clear();
		window.draw(title);
		for (int i = 0; i<n; i++)
			window.draw(tab[i]);
		window.display();
	}
}

void Game::help()
{
	sf::Text title("HELP", font, 90);
	title.setStyle(sf::Text::Bold);

	title.setPosition(800 / 2 - title.getGlobalBounds().width / 2, 60);
	title.setStyle(sf::Text::Bold);

	const int n = 2;

	sf::Text tab[n];

	std::string rules[] = { "1-9 unique(row/col)", "back" };
	for (int i = 0; i<n; i++)
	{
		tab[i].setFont(font);
		tab[i].setCharacterSize(65);
		tab[i].setString(rules[i]);
		tab[i].setPosition(800 / 2 - tab[i].getGlobalBounds().width / 2, 200 + i * 120);
	}


	while (state == HELP && window.isOpen())
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (tab[1].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				state = MENU;
			}
		}
		
		if (tab[1].getGlobalBounds().contains(mouse))
			tab[1].setColor(Color::Green);
		else tab[1].setColor(Color::White);


		window.clear();
		window.draw(title);
		for (int i = 0; i<n; i++)
			window.draw(tab[i]);
		window.display();
	}
}
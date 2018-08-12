/*
Into Asteroid Belt - a LD 42 entry, theme: 'Running out of space'
Copyright (C) 2018 Jakub Kazimierski

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Game.hpp"

const sf::Color Game::BACKGROUND_COLOR 
	= sf::Color(15, 56, 15, 255);

Game::Game()
	: Window(sf::VideoMode(160, 144), "IAB", sf::Style::Titlebar | sf::Style::Close),
	 GameArea(0, 0, Window.getSize().x, Window.getSize().y),
	 MainPlayer(GameArea), Indicator(GameArea), MenuSystem(GameArea), ActualState(Splash)
{
	sf::Image Icon;
	Icon.loadFromFile("assets/Icon.png");
	Window.setIcon(18, 18, Icon.getPixelsPtr());

	SplashScreenTexture.loadFromFile("assets/SplashScreen.png");
	SplashScreen.setTexture(SplashScreenTexture);
	SplashScreen.setPosition(0, 0);

	initPointsCounter();
	restart();
}

void Game::initPointsCounter()
{
	PointsCounter.setFont(MenuSystem.getFont());
	PointsCounter.setCharacterSize(15);
	PointsCounter.setPosition(
		static_cast<float>(GameArea.left + 5),
		static_cast<float>(GameArea.top + 5)
	);
	PointsCounter.setFillColor(sf::Color(139, 172, 15, 255));
	PointsCounter.setOutlineColor(sf::Color(155, 188, 15, 255));
	PointsCounter.setString("000000");
}

void Game::restart()
{
	Points = 0;
	NextBomb = 10;
	MainPlayer.init();
	Asteroids.clear();
	Bombs.clear();
	PointsClock.restart();
	BombClock.restart();
}

void Game::start()
{
	/* I'm using constant frame rate to make it as close to original
		GameBoy as possible.*/
	const int FRAMES_PER_SECOND = 60,
			ONE_FRAME_TIME = 1000 / FRAMES_PER_SECOND; //In milliseconds

	sf::Time Delta = sf::Time::Zero;
	sf::Clock Clock;

	while(Window.isOpen())
	{
		Delta += Clock.getElapsedTime();
		Clock.restart();

		if(Delta.asMilliseconds() >= ONE_FRAME_TIME)
		{
			processEvents();

			handleInput();
			update();
			render();

			Delta = sf::Time::Zero;
		}
	}
}

void Game::processEvents()
{
	sf::Event ActualEvent;
	while(Window.pollEvent(ActualEvent))
	{
		if(ActualEvent.type == sf::Event::Closed)
		{
			Window.close();
		}
	}
}

void Game::update()
{
	switch(ActualState)
	{
		case Restart:
		{
			restart();
			ActualState = Play;
		}

		case Play:
		{
			spawnAsteroids();
			spawnBombs();

			MainPlayer.update();
			updateAsteroids();
			updateBombs();
			Indicator.update(MainPlayer.getHP());

			if(MainPlayer.didLose())
			{
				ActualState = End;
			}

			updatePoints();
			updatePointsCounter();

			break;
		}
		
		case End:
		{
			MenuSystem.updateScore(PointsCounter.getString());
		}
		case MainMenu:
		{
			MenuSystem.update();

			Menu::Request ActualRequest = MenuSystem.getRequest();

			if(ActualRequest == Menu::Request::Restart)
			{
				ActualState = Restart;
			}

			if(ActualRequest == Menu::Request::Quit)
			{
				Window.close();
			}

			break;
		}

		case Splash:
		{
			++NumberOfFrames;

			if(NumberOfFrames == 50)
			{
				ActualState = MainMenu;
			}
			break;
		}
	}
	
}

void Game::updateAsteroids()
{
	for(size_t i = 0; i < Asteroids.size(); ++i)
	{
		MainPlayer.handleCollision(*Asteroids[i]);

		for(size_t j = 0; j < Asteroids.size(); ++j)
		{
			if(j == i)
			{
				continue;
			}

			Asteroids[i]->handleCollision(*Asteroids[j]);
		}

		if(Asteroids[i]->shouldBeRemoved())
		{
			Asteroids.erase(Asteroids.begin() + i);
			--i;
		}

		if(i != -1)
		{
			Asteroids[i]->update();
		}
	}
}

void Game::updateBombs()
{
	for(size_t i = 0; i < Bombs.size(); ++i)
	{
		Bombs[i]->update();
		MainPlayer.handleCollision(*Bombs[i]);

		if(Bombs[i]->shouldBeRemoved())
		{
			if(Bombs[i]->wasActivated())
			{
				bomb();
			}

			Bombs.erase(Bombs.begin() + i);
			--i;
		}
	}
}

void Game::spawnAsteroids()
{
	std::random_device Device;
	std::uniform_int_distribution<unsigned int> TimeDistribution(1500, 2500);

	if(!wasGameStarted)
	{
		for(int i = 0; i < NUMBER_OF_STARTING_ASTEROIDS; ++i)
		{
			Asteroids.push_back(std::make_unique<Asteroid>(GameArea));
		}
		wasGameStarted = true;
		NextSpawn = TimeDistribution(Device);
	}
	else if(SpawnClock.getElapsedTime().asMilliseconds() > NextSpawn)
	{
		Asteroids.push_back(std::make_unique<Asteroid>(GameArea));
		NextSpawn = TimeDistribution(Device);
		SpawnClock.restart();
	}
}

void Game::spawnBombs()
{
	std::random_device Device;
	std::uniform_int_distribution<unsigned int> NextPowerUpDistribution(7, 15);

	if(BombClock.getElapsedTime().asSeconds() > NextBomb)
	{
		NextBomb = NextPowerUpDistribution(Device);
		Bombs.push_back(std::make_unique<Bomb>(GameArea));
		BombClock.restart();
	}
}

void Game::bomb()
{
	const unsigned int NUMBER_OF_ASTEROIDS = Asteroids.size();

	if(NUMBER_OF_ASTEROIDS != 0)
	{
		std::random_device Device;
		std::uniform_int_distribution<unsigned int> AsteroidsDistribution;

		for(size_t i = 0;
			  i < (NUMBER_OF_ASTEROIDS < 2 ? 1 : NUMBER_OF_ASTEROIDS / 2); ++i)
		{
			AsteroidsDistribution =
				std::uniform_int_distribution<unsigned int>(0, Asteroids.size() - 1);

			Asteroids[AsteroidsDistribution(Device)]->destroy();
		}
	}
}

void Game::updatePoints()
{
	if(PointsClock.getElapsedTime().asSeconds() > 1)
	{
		Points += 5;
		Points += Asteroids.size() * 2;
		PointsClock.restart();
	}
}

void Game::updatePointsCounter()
{
	const size_t NumberOfAdditionalZeros  
		= 6 - static_cast<size_t>(std::log10(Points));
	std::stringstream NewText;

	for(size_t i = 0; i < NumberOfAdditionalZeros; ++i)
	{
		NewText << '0';
	}

	NewText << Points;

	PointsCounter.setString(NewText.str());
	
}

void Game::handleInput()
{
	switch(ActualState)
	{
		case Play:
			MainPlayer.handleInput();
			break;

		case MainMenu:
		case End:
			MenuSystem.handleInput();
			break;
	}
	
}

void Game::render()
{
	Window.clear(BACKGROUND_COLOR);

	switch(ActualState)
	{
		case Play:
		{
			for(auto& Bomb : Bombs)
			{
				Window.draw(*Bomb);
			}

			Window.draw(MainPlayer);

			for(auto& Asteroid : Asteroids)
			{
				Window.draw(*Asteroid);
			}

			Window.draw(Indicator);
			Window.draw(PointsCounter);

			break;
		}

		case MainMenu:
		case End:
		{
			Window.draw(MenuSystem);
			break;
		}

		case Splash:
			Window.draw(SplashScreen);
			break;
	}

	Window.display();
}

Game::~Game()
{
}

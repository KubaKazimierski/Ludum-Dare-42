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
	MainPlayer(sf::IntRect(0, 0, Window.getSize().x, Window.getSize().y))
{
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

		if(Delta.asMilliseconds() >= ONE_FRAME_TIME)
		{
			processEvents();

			MainPlayer.handleInput();
			MainPlayer.update();

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

void Game::render()
{
	Window.clear(BACKGROUND_COLOR);
	Window.draw(MainPlayer);
	Window.display();
}

Game::~Game()
{
}

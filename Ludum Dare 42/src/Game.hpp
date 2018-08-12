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

#pragma once
#include "Player.hpp"
#include "Asteroid.hpp"
#include "HPIndicator.hpp"
#include "Menu.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>
#include <string>
#include <sstream>

class Game
{
	enum State
	{
		Splash, 
		MainMenu,
		Play, 
		End, 
		Restart
	};

public:
	Game();
	void start();
	~Game();

private:
	static const sf::Color BACKGROUND_COLOR;
	static const unsigned int NUMBER_OF_STARTING_ASTEROIDS = 3;

	sf::RenderWindow Window;
	sf::IntRect GameArea;
	sf::Sprite SplashScreen;
	sf::Texture SplashScreenTexture;
	sf::Clock SpawnClock, PointsClock;
	sf::Text PointsCounter;

	State ActualState;
	Player MainPlayer;
	Menu MenuSystem;
	HPIndicator Indicator;
	std::vector<std::unique_ptr<Asteroid>> Asteroids;

	bool wasGameStarted = false;
	unsigned int NumberOfFrames = 0, NextSpawn;
	unsigned long long int Points;

	void initPointsCounter();
	void restart();
	void processEvents();
	void render();
	void update();
	void updateAsteroids();
	void updatePoints();
	void updatePointsCounter();
	void spawnAsteroids();
	void handleInput();
};


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
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Game
{
public:
	Game();
	void start();
	~Game();

private:
	static const sf::Color BACKGROUND_COLOR;

	sf::RenderWindow Window;
	sf::IntRect GameArea;
	Player MainPlayer;
	std::vector<std::unique_ptr<Asteroid>> Asteroids;

	void processEvents();
	void render();
	void update();
	void handleInput();
};


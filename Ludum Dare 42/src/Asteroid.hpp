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
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include <chrono>

#define _USE_MATH_DEFINES
#include <math.h>

class Asteroid : public sf::Drawable
{
public:
	Asteroid(const sf::IntRect& GameArea);
	void draw(sf::RenderTarget& Target, sf::RenderStates States) const;
	void update();
	void handleCollision(Asteroid& Object);
	const sf::FloatRect getGlobalBounds();
	void destroy();
	bool shouldBeRemoved();
	~Asteroid();

private:
	static sf::Image SpriteSheet, ExplosionSheet;
	static bool wereAssetsLoaded;

	const sf::IntRect GameArea;
	const float MIN_SPEED = 1.5f, MAX_SPEED = 2.0f;
	const sf::Vector2f SIZE = { 18, 18 };
	const sf::FloatRect NotExist = { 1000, 1000, 1, 1 };

	int Angle;
	float Speed;
	bool isBeingDestroyed = false;
	unsigned int State = 0;

	sf::FloatRect LastPenetration;
	sf::CircleShape Sprite;
	sf::Texture SpriteTexture;
	sf::Music ExplosionSound;
	sf::Vector2f Direction;
	sf::Clock Clock;

	Asteroid* LastCollision;

	void randomize();
	void move();
};


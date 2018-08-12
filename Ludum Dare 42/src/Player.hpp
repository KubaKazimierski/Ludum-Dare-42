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
#include "Asteroid.hpp"
#include <SFML/Graphics.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

class Player : public sf::Drawable
{
	enum Mode
	{
		Idle = 0,
		Moving
	};

public:
	Player(const sf::IntRect& GameArea);
	void init();
	void draw(sf::RenderTarget& Target, sf::RenderStates States) const;
	void update();
	void handleInput();
	void handleCollision(Asteroid& Object);
	bool didLose();
	unsigned int getHP();
	~Player();

private:
	const sf::IntRect GameArea;
	const sf::Vector2f SIZE = { 18, 18 };
	const int MAX_HP = 6;
	const float MAX_SPEED = 3.0f,
		ACCELERATION = 0.75f,
		DECELERATION = 0.5f;

	sf::Sprite Sprite;
	sf::Image SpriteSheet, ExplosionSheet;
	sf::Texture SpriteTexture;
	sf::Vector2f  Direction;
	sf::Clock Clock;
	sf::Time Delta;

	Mode ActualMode;

	float Speed = 0;
	unsigned int HP = 6, NumberOfBlinks = 0, ExplosionState = 0;
	bool Blink, isBlinking, DontDraw = false, isBeingDestroyed = false;

	void setSpriteTexture();
	void move();
	bool canMove();
	void blink();
	void destroy();
};


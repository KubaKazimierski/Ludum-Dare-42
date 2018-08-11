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

#include "Player.hpp"
#include <iostream>

Player::Player(const sf::IntRect& GameArea) 
	: Direction(0, 0), GameArea(GameArea), ActualMode(Idle)
{
	SpriteSheet.loadFromFile("assets/Spaceship.png");
	setSpriteTexture();

	Sprite.setPosition(GameArea.left + (GameArea.width / 2) - SIZE.x / 2,
					   GameArea.top + (GameArea.height / 2) - SIZE.y / 2);
}

void Player::setSpriteTexture()
{
	SpriteTexture.loadFromImage(SpriteSheet,
								sf::IntRect(SIZE.x * ActualMode, 0,
											static_cast<int>(SIZE.x),
											static_cast<int>(SIZE.y)));
	Sprite.setTexture(SpriteTexture);
}

void Player::draw(sf::RenderTarget& Target, sf::RenderStates States) const
{
	Target.draw(Sprite, States);
}

void Player::update()
{
	static const sf::Vector2f ZERO = { 0, 0 };

	if(Direction != ZERO)
	{
		if(Speed < MAX_SPEED)
		{
			ActualMode = Moving;
			Speed += ACCELERATION;
		}
		
	}
	else
	{
		if(Speed < DECELERATION)
		{
			ActualMode = Idle;
			Speed == 0;
		}
		else
		{
			ActualMode = Moving;
			Speed -= DECELERATION;
		}
	}

	if(canMove())
	{
		Sprite.setPosition(Sprite.getPosition() + Direction * Speed);
	}
	
	setSpriteTexture();
	Direction = ZERO;
}

void Player::calculateRealDirection()
{
	float Angle;

	if(Direction.x != 0 && Direction.y != 0)
	{
		int Sum = Direction.x + Direction.y;

		if(Sum == 2)
		{
			Angle = 3 * (M_PI / 4);
		}
		else if(Sum == -2)
		{
			Angle = 7 * (M_PI / 4);
		}
		else
		{
			Angle = (Direction.x < 0 ? 5 : 1) * (M_PI / 4);
		}

		Direction = sf::Vector2f(sin(Angle), -cos(Angle));
	}
}

bool Player::canMove()
{
	sf::Vector2f ActualPosition = Sprite.getPosition();

	bool Result = true;

	if((GameArea.left > ActualPosition.x && Direction.x < 0)
	   || (GameArea.left + GameArea.width < ActualPosition.x + SIZE.x 
		   && Direction.x > 0))
	{
		Result = false;
	}

	if((GameArea.top > ActualPosition.y && Direction.y < 0)
	   || (GameArea.top + GameArea.height < ActualPosition.y + SIZE.y - 3
		   && Direction.y > 0))
	{
		Result = false;
	}

	return Result;
}

void Player::handleInput()
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		Direction.y -= 1;
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		Direction.y += 1;
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		Direction.x -= 1;
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		Direction.x += 1;
	}
}

Player::~Player()
{
}

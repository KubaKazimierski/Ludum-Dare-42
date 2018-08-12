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
	ExplosionSheet.loadFromFile("assets/Explosion.png");
	init();
}

void Player::init()
{
	HP = MAX_HP;
	Blink = false;
	isBlinking = false;
	ExplosionState = 0;
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
	if(!DontDraw)
	{
		Target.draw(Sprite, States);
	}
}

void Player::update()
{
	if(HP != 0)
	{
		move();

		if(Blink)
		{
			blink();
		}
	}
	else
	{
		destroy();
	}
	
}

void Player::move()
{
	static const sf::Vector2f ZERO = { 0, 0 };
	ActualMode = Idle;

	if(Direction != ZERO)
	{
		ActualMode = Moving;
		if(Speed < MAX_SPEED)
		{
			Speed += ACCELERATION;
		}

	}
	else
	{
		if(Speed < DECELERATION)
		{
			Speed = 0;
		}
		else
		{
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

void Player::blink()
{
	if(!isBlinking)
	{
		Delta = sf::Time::Zero;
		Clock.restart();
		isBlinking = true;
		NumberOfBlinks = 0;
	}
	

	Delta += Clock.getElapsedTime();
	Clock.restart();

	if(Delta.asMilliseconds() > 50)
	{
		DontDraw = !DontDraw;
		Delta = sf::Time::Zero;
		++NumberOfBlinks;
	}

	if(NumberOfBlinks == 6)
	{
		Blink = false;
		isBlinking = false;
	}
}

void Player::destroy()
{
	if(!isBeingDestroyed)
	{
		isBeingDestroyed = true;
		ExplosionState = 0;
		SpriteTexture.loadFromImage(ExplosionSheet,
									sf::IntRect(static_cast<int>(SIZE.x), 0,
												static_cast<int>(SIZE.x), static_cast<int>(SIZE.y)));
		Sprite.setTexture(SpriteTexture);
		Clock.restart();
	}

	if(Clock.getElapsedTime().asMilliseconds() > 60 && ExplosionState != 5)
	{
		SpriteTexture.loadFromImage(ExplosionSheet,
									sf::IntRect(static_cast<int>(SIZE.x) * ExplosionState, 0,
												static_cast<int>(SIZE.x), static_cast<int>(SIZE.y)));
		Sprite.setTexture(SpriteTexture);
		Clock.restart();
		++ExplosionState;
	}
}

unsigned int Player::getHP()
{
	return HP;
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
	   || (GameArea.top + GameArea.height < ActualPosition.y + SIZE.y - 1
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

void Player::handleCollision(Asteroid& Object)
{
	sf::FloatRect PlayerBounds = Sprite.getGlobalBounds();

	if(ActualMode == Moving)
	{
		PlayerBounds.height -= 2;
	}

	if(Sprite.getGlobalBounds().intersects(Object.getGlobalBounds()) 
	   && !Blink && HP > 0)
	{
		Object.destroy();
		Blink = true;
		--HP;
	}
}

void Player::handleCollision(Bomb& Object)
{
	if(Sprite.getGlobalBounds().intersects(Object.getGlobalBounds()))
	{
		Object.activate();
	}
}

bool Player::didLose()
{
	return ExplosionState == 5;
}

Player::~Player()
{
}

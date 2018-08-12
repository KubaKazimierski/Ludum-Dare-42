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

#include "Asteroid.hpp"

sf::Image Asteroid::SpriteSheet, Asteroid::ExplosionSheet;

bool Asteroid::wereAssetsLoaded = false;

Asteroid::Asteroid(const sf::IntRect& GameArea)
	: GameArea(GameArea), LastPenetration(0, 0, 0, 0)
{
	if(!wereAssetsLoaded)
	{
		SpriteSheet.loadFromFile("assets/Asteroids.png");
		ExplosionSheet.loadFromFile("assets/Explosion.png");
		wereAssetsLoaded = true;
	}

	ExplosionSound.openFromFile("assets/Explosion.wav");

	randomize();
	Sprite.setRadius(9);
	Sprite.setTexture(&SpriteTexture);
}

void Asteroid::randomize()
{
	std::random_device Device;

	std::uniform_real_distribution<float>
		Speed_Distribution(MIN_SPEED, MAX_SPEED);

	Speed = Speed_Distribution(Device);

	std::uniform_int_distribution<int>
		Direction_Distribution(20, 160);

	Angle = Direction_Distribution(Device);

	Direction = sf::Vector2f(std::sin(Angle), -std::cos(Angle));

	std::uniform_int_distribution<int>
		Texture_Distribution(0, 9);

	SpriteTexture.loadFromImage(SpriteSheet,
								sf::IntRect(static_cast<int>(SIZE.x * Texture_Distribution(Device)), 0,
											static_cast<int>(SIZE.x), static_cast<int>(SIZE.y)));
	std::uniform_real_distribution<float>
		Scale_Distribution(0.8, 1.8);
	float LinearScale = Scale_Distribution(Device);
	Sprite.setScale(sf::Vector2f(LinearScale, LinearScale));

	std::uniform_real_distribution<float>
		Position_Distribution(GameArea.left, GameArea.left + GameArea.width);

	Sprite.setPosition(Position_Distribution(Device), GameArea.top + GameArea.height + 40);
}

void Asteroid::draw(sf::RenderTarget& Target, sf::RenderStates States) const
{
	Target.draw(Sprite, States);
}

void Asteroid::update()
{
	if(!isBeingDestroyed)
	{
		move();
	}
	else
	{
		destroy();
	}
}

void Asteroid::move()
{
	sf::Vector2f ActualPosition = Sprite.getPosition();

	if(LastPenetration != sf::FloatRect(0, 0, 0, 0))
	{
		ActualPosition.x += Direction.x / std::abs(Direction.x) * (LastPenetration.width / 2);
		ActualPosition.x += Direction.y / std::abs(Direction.y) * (LastPenetration.height / 2);
		LastPenetration = sf::FloatRect(0, 0, 0, 0);
	}

	if(ActualPosition.x <= GameArea.left - 38
	   || ActualPosition.x >= GameArea.width + 38)
	{
		ActualPosition.x
			= (ActualPosition.x > 0
			   ? GameArea.left - 28
			   : GameArea.width + 28);
	}

	if(ActualPosition.y <= GameArea.top - 38
	   || ActualPosition.y >= GameArea.height + 38)
	{
		ActualPosition.y
			= (ActualPosition.y > 0
			   ? GameArea.top - 28
			   : GameArea.height + 28);
	}

	Sprite.setPosition(ActualPosition + Direction * Speed);
}

void Asteroid::handleCollision(Asteroid& Object)
{
	sf::Vector2f ActualPosition = Sprite.getPosition();

	if(Sprite.getGlobalBounds().intersects(Object.Sprite.getGlobalBounds(), LastPenetration)
		&& !isBeingDestroyed && LastCollision != &Object)
	{
		Direction = -Direction;

		LastCollision = &Object;
		Clock.restart();
	}

}

const sf::FloatRect Asteroid::getGlobalBounds()
{
	return (isBeingDestroyed ? 
			NotExist
			: Sprite.getGlobalBounds());
}

void Asteroid::destroy()
{
	if(!isBeingDestroyed)
	{
		isBeingDestroyed = true;
		ExplosionSound.play();
		SpriteTexture.loadFromImage(ExplosionSheet,
									sf::IntRect(static_cast<int>(SIZE.x), 0,
												static_cast<int>(SIZE.x), static_cast<int>(SIZE.y)));
		Sprite.setTexture(&SpriteTexture);
		Clock.restart();
	}

	if(Clock.getElapsedTime().asMilliseconds() > 60 && State != 5)
	{
		SpriteTexture.loadFromImage(ExplosionSheet,
									sf::IntRect(static_cast<int>(SIZE.x) * State, 0,
												static_cast<int>(SIZE.x), static_cast<int>(SIZE.y)));
		Sprite.setTexture(&SpriteTexture);
		Clock.restart();
		++State;
	}
}

bool Asteroid::shouldBeRemoved()
{
	return State == 5;
}

Asteroid::~Asteroid()
{
	ExplosionSound.stop();
}

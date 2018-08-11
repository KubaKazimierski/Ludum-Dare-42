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

Asteroid::Asteroid(const sf::IntRect& GameArea)
	: GameArea(GameArea)
{
	SpriteSheet.loadFromFile("assets/Asteroids.png");
	randomize();
	Sprite.setTexture(SpriteTexture);
}

void Asteroid::randomize()
{
	std::random_device Device;

	std::uniform_real_distribution<float>
		Speed_Distribution(MIN_SPEED, MAX_SPEED);

	Speed = Speed_Distribution(Device);

	std::uniform_int_distribution<int>
		Direction_Distribution(20, 160);

	Direction = sf::Vector2f(std::sin(Direction_Distribution(Device)),
							 -std::cos(Direction_Distribution(Device)));

	std::uniform_int_distribution<int>
		Texture_Distribution(0, 9);

	SpriteTexture.loadFromImage(SpriteSheet,
								sf::IntRect(static_cast<int>(SIZE.x * Texture_Distribution(Device)), 0,
											static_cast<int>(SIZE.x), static_cast<int>(SIZE.y)));
	std::uniform_real_distribution<float>
		Scale_Distribution(0.8, 1.5);
	float LinearScale = Scale_Distribution(Device);
	Sprite.setScale(sf::Vector2f(LinearScale, LinearScale));

	std::uniform_real_distribution<float>
		Position_Distribution(GameArea.left, GameArea.left + GameArea.width);

	Sprite.setPosition(Position_Distribution(Device), 0);
}

void Asteroid::draw(sf::RenderTarget& Target, sf::RenderStates States) const
{
	Target.draw(Sprite, States);
}

void Asteroid::update()
{
	sf::Vector2f ActualPosition = Sprite.getPosition();

	if(ActualPosition.x <= GameArea.left - 30
	   || ActualPosition.x >= GameArea.width + 30)
	{
		ActualPosition.x 
			= (ActualPosition.x > 0 
			   ? GameArea.left - 28
			   : GameArea.width + 28);
	}

	if(ActualPosition.y <= GameArea.top - 30
	   || ActualPosition.y >= GameArea.height + 30)
	{
		ActualPosition.y
			= (ActualPosition.y > 0
			   ? GameArea.top - 28
			   : GameArea.height + 28);
	}

	Sprite.setPosition(ActualPosition + Direction * Speed);
}

void Asteroid::handleCollision(const Asteroid& Object)
{
	sf::Vector2f ActualPosition = Sprite.getPosition();

	if(Sprite.getGlobalBounds().intersects(Object.Sprite.getGlobalBounds()))
	{
		 
		Direction = -Direction;
		
		update();
	}

	update();
}

Asteroid::~Asteroid()
{
}

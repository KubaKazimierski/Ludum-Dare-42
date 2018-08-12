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

#include "Bomb.hpp"

Bomb::Bomb(const sf::IntRect& GameArea)
	: GameArea(GameArea), Delta(sf::Time::Zero)
{
	Texture.loadFromFile("assets/Bomb.png");
	Sprite.setTexture(Texture);
	randomize();
}

void Bomb::randomize()
{
	std::random_device Device;
	std::uniform_real_distribution<float> Position[2]
		= { std::uniform_real_distribution<float>(GameArea.left + 6, GameArea.left + GameArea.width - 6),
		std::uniform_real_distribution<float>(GameArea.top + Sprite.getGlobalBounds().width + 5, GameArea.top + GameArea.height - Sprite.getGlobalBounds().width - 5) };

	Sprite.setPosition(Position[0](Device), Position[1](Device));
}

void Bomb::update()
{
	if(Clock.getElapsedTime().asSeconds() > static_cast<float>(EXPIRE_TIME) - 0.5f)
	{
		if((Clock.getElapsedTime() - Delta).asMilliseconds() > 50)
		{
			dontDraw = !dontDraw;
			Delta = Clock.getElapsedTime();
		}
	}
}

void Bomb::activate()
{
	isActivated = true;
}

bool Bomb::wasActivated()
{
	return isActivated;
}

bool Bomb::shouldBeRemoved()
{
	return isActivated || Clock.getElapsedTime().asSeconds() > EXPIRE_TIME;
}

const sf::FloatRect Bomb::getGlobalBounds()
{
	return Sprite.getGlobalBounds();
}

void Bomb::draw(sf::RenderTarget& Target, sf::RenderStates States) const
{
	if(!dontDraw)
	{
		Target.draw(Sprite, States);
	}
}

Bomb::~Bomb()
{
}

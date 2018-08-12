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

#include "HPIndicator.hpp"

HPIndicator::HPIndicator(const sf::IntRect& GameArea)
	: GameArea(GameArea)
{
	SpriteSheet.loadFromFile("assets/Heart.png");
	FullTexture.loadFromImage(SpriteSheet, sf::IntRect(0, 0, SIZE.x, SIZE.y));
	HalfTexture.loadFromImage(SpriteSheet, sf::IntRect(SIZE.x, 0, SIZE.x, SIZE.y));
}

void HPIndicator::update(unsigned int HP)
{
	if(this->HP != HP)
	{
		Hearts.clear();

		for(int i = static_cast<int>(HP); i > 0; i -= 2)
		{
			if(i == 1)
			{
				Hearts.push_back(std::make_unique<sf::Sprite>());
				(*(Hearts.end() - 1))->setTexture(HalfTexture);
			}
			else
			{
				Hearts.push_back(std::make_unique<sf::Sprite>());
				(*(Hearts.end() - 1))->setTexture(FullTexture);
			}
		}

		for(size_t i = 0; i < Hearts.size(); ++i)
		{
			Hearts[i]->setScale(0.5, 0.5);
			Hearts[i]->setPosition(GameArea.left + 5 + ((SIZE.x / 2  + 2) * i),
								   GameArea.top + GameArea.height - 15);
		}
	}
	
	this->HP = HP;
}

void HPIndicator::draw(sf::RenderTarget& Target, sf::RenderStates States) const
{
	for(auto& Heart : Hearts)
	{
		Target.draw(*Heart);
	}
}

HPIndicator::~HPIndicator()
{
}

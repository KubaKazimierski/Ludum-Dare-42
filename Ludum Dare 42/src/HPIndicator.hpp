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
#include <vector>
#include <memory>

class HPIndicator : public sf::Drawable
{
public:
	HPIndicator(const sf::IntRect& GameArea);
	void draw(sf::RenderTarget& Target, sf::RenderStates States) const;
	void update(unsigned int HP);
	~HPIndicator();

private:
	const sf::Vector2f SIZE = { 18, 18};

	sf::Image SpriteSheet;
	sf::Texture FullTexture, HalfTexture;
	sf::IntRect GameArea;
	std::vector<std::unique_ptr<sf::Sprite>> Hearts;

	unsigned int HP;
};


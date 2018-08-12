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
#include <string>

class Menu : public sf::Drawable
{
	enum Type
	{
		MainMenu,
		AboutScreen,
		EndMenu
	};

public:

	enum Request
	{
		Null,
		Restart,
		Quit
	};

	Menu(const sf::IntRect& GameArea);
	void update();
	void updateScore(const std::string& NewScore);
	void handleInput();
	const sf::Font& getFont();
	Request getRequest();
	void draw(sf::RenderTarget& Target, sf::RenderStates States) const;
	~Menu();

private:
	static bool isFontLoaded;
	static const unsigned int OPTION_SIZE = 15,
		TIME_BETWEEN_MENU_CHANGE = 400;

	const sf::IntRect GameArea;

	unsigned int SelectedOption = 0;
	bool areTextsSet = false, drawPointer = false;

	sf::Font Font;
	sf::Sprite Sprite;
	sf::Texture Texture;
	sf::Text Pointer;
	sf::Clock MenuChangeClock, PointerClock;

	Type ActualType;
	Request ActualRequest;
	std::vector<std::unique_ptr<sf::Text>> Texts;

	void setTexts();
	void updatePointer();
	inline void restartPointer();
	
};


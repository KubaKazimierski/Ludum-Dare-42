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

#include "Menu.hpp"

bool Menu::isFontLoaded = false;

Menu::Menu(const sf::IntRect& GameArea) 
	: GameArea(GameArea), ActualType(MainMenu)
{
	Font.loadFromFile("font/LCD_Solid.ttf");
	isFontLoaded = true;

	Texture.loadFromFile("assets/Logo.png");
	Sprite.setTexture(Texture);

	Pointer.setString(">");
	Pointer.setFont(Font);
	Pointer.setCharacterSize(OPTION_SIZE);
	Pointer.setFillColor(sf::Color(139, 172, 15, 255));
	Pointer.setOutlineColor(sf::Color(155, 188, 15, 255));
}

void Menu::update()
{
	setTexts();
	updatePointer();
}

void Menu::setTexts()
{
	if(!areTextsSet)
	{
		Texts.clear();
		SelectedOption = 0;
		areTextsSet = true;

		switch(ActualType)
		{
			case MainMenu:
			{
				std::vector<std::string> MenuTexts
					= { "START", "ABOUT" };

				for(size_t i = 0; i < MenuTexts.size(); ++i)
				{
					Texts.push_back(std::make_unique<sf::Text>(
						MenuTexts[i],
						Font, OPTION_SIZE
						));

					auto ActualText = Texts.begin() + i;

					(*ActualText)->setFillColor(
						sf::Color(139, 172, 15, 255)
					);
					(*ActualText)->setOutlineColor(
						sf::Color(155, 188, 15, 255)
					);
					(*ActualText)->setPosition(
						GameArea.left + GameArea.width / 2 + (i == 1 ? 15 : -((*ActualText)->getGlobalBounds().width + 15)),
						GameArea.top + GameArea.height - 50
					);
				}

				Texts.push_back(std::make_unique<sf::Text>(
					"Copyright (C)\n2018 Jakub Kazimierski",
					Font, 8
					));

				auto ActualText = Texts.begin() + Texts.size() - 1;

				(*ActualText)->setFillColor(
					sf::Color(139, 172, 15, 255)
				);
				(*ActualText)->setOutlineColor(
					sf::Color(155, 188, 15, 255)
				);
				(*ActualText)->setPosition(
					GameArea.left + 10,
					GameArea.top + GameArea.height - 25
				);

				break;
			}

			case AboutScreen:
			{
				std::vector<std::string> MenuTexts
					= { "BACK",  "ABOUT:" };

				for(size_t i = 0; i < MenuTexts.size(); ++i)
				{
					Texts.push_back(std::make_unique<sf::Text>(
						MenuTexts[i],
						Font, OPTION_SIZE
						));

					auto ActualText = (Texts.begin() + i);

					(*ActualText)->setFillColor(
						sf::Color(139, 172, 15, 255)
					);
					(*ActualText)->setOutlineColor(
						sf::Color(155, 188, 15, 255)
					);
					(*ActualText)->setPosition(
						GameArea.left + GameArea.width / 2  -((*ActualText)->getGlobalBounds().width  / 2),
						(i == 1 ? GameArea.top + 1 : GameArea.top + GameArea.height - 20)
					);
				}

				std::string AboutText =
					"It is simple game about\n"
					"avoiding asteroids.\n"
					"You earn points every\n"
					"second. The more asteroids,\n"
					"the more points.\n"
					"You can collect bombs to\n"
					"remove half of asteroids."
					"\nControls:\n"
					"[ W, S, A, D ] - movement\n"
					"\nGame is licensed under\n"
					"GNU GPLv3";

				Texts.push_back(std::make_unique<sf::Text>(
					AboutText,
					Font, 9
					));

				auto ActualText = Texts.begin() + 2;

				(*ActualText)->setFillColor(
					sf::Color(139, 172, 15, 255)
				);
				(*ActualText)->setOutlineColor(
					sf::Color(155, 188, 15, 255)
				);

				(*ActualText)->setPosition(GameArea.left + 5,
					(*(Texts.begin() + 1))->getGlobalBounds().top + (*(Texts.begin() + 1))->getGlobalBounds().height + 2);

				break;
			}

			case EndMenu:
			{
				std::vector<std::string> MenuTexts
					= { "RESTART", "QUIT" };

				for(size_t i = 0; i < MenuTexts.size(); ++i)
				{
					Texts.push_back(std::make_unique<sf::Text>(
						MenuTexts[i],
						Font, OPTION_SIZE
						));

					auto ActualText = Texts.begin() + i;

					(*ActualText)->setFillColor(
						sf::Color(139, 172, 15, 255)
					);
					(*ActualText)->setOutlineColor(
						sf::Color(155, 188, 15, 255)
					);
					(*ActualText)->setPosition(
						GameArea.left + GameArea.width / 2 + (i == 1 ? 30 : -((*ActualText)->getGlobalBounds().width)),
						GameArea.top + GameArea.height - 20
					);

				}

				Texts.push_back(std::make_unique<sf::Text>(
					"YOUR SCORE: 000000", Font, 12));

				auto ActualText = Texts.begin() + 2;

				(*ActualText)->setFillColor(
					sf::Color(139, 172, 15, 255)
				);
				(*ActualText)->setOutlineColor(
					sf::Color(155, 188, 15, 255)
				);
				(*ActualText)->setPosition(
					GameArea.left + GameArea.width / 2 - (*ActualText)->getGlobalBounds().width / 2 - 3,
					GameArea.top + GameArea.height / 2 + (*ActualText)->getGlobalBounds().height / 2 + 15
				);

				break;
			}
		}
	}
}

void Menu::updatePointer()
{
	if(areTextsSet)
	{
		sf::Vector2f SelectedOptionPosition
			= Texts[SelectedOption]->getPosition();

		Pointer.setPosition(SelectedOptionPosition.x - 10,
							SelectedOptionPosition.y);

		if(PointerClock.getElapsedTime().asMilliseconds() > 500)
		{
			drawPointer = !drawPointer;
			PointerClock.restart();
		}
	}
}

void Menu::restartPointer()
{
	drawPointer = true;
	PointerClock.restart();
}

void Menu::updateScore(const std::string& NewScore)
{
	(*(Texts.end() - 1))->setString(
		std::string("YOUR SCORE: ") + NewScore
	);
}

void Menu::handleInput()
{
	switch(ActualType)
	{
		case AboutScreen:
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)
			   && MenuChangeClock.getElapsedTime().asMilliseconds() > TIME_BETWEEN_MENU_CHANGE)
			{
				ActualType = MainMenu;
				areTextsSet = false;
				MenuChangeClock.restart();
			}

			break;
		}

		case MainMenu:
		case EndMenu:
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)
			   && SelectedOption != 1)
			{
				SelectedOption = 1;
				restartPointer();
			}

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)
			   && SelectedOption != 0)
			{
				SelectedOption = 0;
				restartPointer();
			}

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)
			   && SelectedOption == 1
			   && MenuChangeClock.getElapsedTime().asMilliseconds() > TIME_BETWEEN_MENU_CHANGE)
			{
				if(ActualType == MainMenu)
				{
					ActualType = AboutScreen;
					areTextsSet = false;
					MenuChangeClock.restart();
				}
				else
				{
					ActualRequest = Quit;
				}

			}

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)
			   && SelectedOption == 0
			   && MenuChangeClock.getElapsedTime().asMilliseconds() > TIME_BETWEEN_MENU_CHANGE)
			{
				ActualRequest = Restart;
				ActualType = EndMenu;
				areTextsSet = false;
				SelectedOption = 0;

				Texture.loadFromFile("assets/Gameover.png");
				Sprite.setTexture(Texture);
			}

			break;
		}
	}
}

const sf::Font& Menu::getFont()
{
	return Font;
}

Menu::Request Menu::getRequest()
{
	Request Output = ActualRequest;
	ActualRequest = Null;
	return Output;
}

void Menu::draw(sf::RenderTarget& Target, sf::RenderStates States) const
{
	if(ActualType != AboutScreen)
	{
		Target.draw(Sprite, States);
	}

	for(auto& Text : Texts)
	{
		Target.draw(*Text, States);
	}

	if(drawPointer)
	{
		Target.draw(Pointer);
	}
}

Menu::~Menu()
{
}

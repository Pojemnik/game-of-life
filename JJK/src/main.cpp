#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <set>
#include <Windows.h>
#include <cstdlib>
#include <ctime>

#include "button.h"
#include "checkbox.h"
#include "cellmatrix.h"

sf::Font font;

class Label
{

private:
	sf::Text* txt;

public:
	Label(sf::Vector2f pos, std::vector<sf::Drawable*> &vect, std::wstring text, unsigned int size)
	{
		txt = new sf::Text(text, font, size);
		txt->setFillColor(sf::Color(0, 0, 0, 255));
		txt->setStyle(sf::Text::Bold);
		txt->setPosition(pos);
		vect.push_back(txt);
	}

	void setString(std::wstring s)
	{
		txt->setString(s);
	}
};

int main()
{
	bool go = false;
	int speed = 10;
	std::vector<sf::Drawable*> vect;
	font.loadFromFile("../lucida.ttf");
	sf::RenderWindow window(sf::VideoMode(800, 450), "Game of life");
	CellMatrix matrix = CellMatrix(sf::Vector2f(400, 60), 10, 10);
	std::vector<Button> buttons;
	std::vector<Checkbox> check_boxes;
	sf::Clock clock;
	Label speed_text = Label(sf::Vector2f(10, 120), vect, L"SZYBKOŚĆ: " + std::to_wstring(speed), 20);
	Label rules_text = Label(sf::Vector2f(10, 190), vect, L"ZASADY:", 20);
	Label life_text = Label(sf::Vector2f(10, 220), vect, L"ŻYCIE:", 15);
	Label resp_text = Label(sf::Vector2f(10, 270), vect, L"ODRADZANIE:", 15);
	Label color_text = Label(sf::Vector2f(10, 350), vect, L"KOLOR:", 20);
	check_boxes.push_back(Checkbox(sf::Vector2f(10, 380), false, "color"));
	Label gen_text = Label(sf::Vector2f(10, 320), vect, L"POKOLENIE: " + std::to_wstring(matrix.gen), 15);
	buttons.push_back(Button(sf::Vector2f(400, 10), sf::Vector2f(30, 30), L"-", "removeX", font, sf::Vector2i(-1, -5)));
	buttons.push_back(Button(sf::Vector2f(460, 10), sf::Vector2f(30, 30), L"+", "addX", font, sf::Vector2i(-1, 0)));
	buttons.push_back(Button(sf::Vector2f(350, 60), sf::Vector2f(30, 30), L"-", "removeY", font, sf::Vector2i(-1, -5)));
	buttons.push_back(Button(sf::Vector2f(350, 120), sf::Vector2f(30, 30), L"+", "addY", font, sf::Vector2i(-1, 0)));
	buttons.push_back(Button(sf::Vector2f(10, 150), sf::Vector2f(30, 30), L"-", "remove_speed", font, sf::Vector2i(-1, -5)));
	buttons.push_back(Button(sf::Vector2f(70, 150), sf::Vector2f(30, 30), L"+", "add_speed", font, sf::Vector2i(-1, 0)));
	buttons.push_back(Button(sf::Vector2f(10, 10), sf::Vector2f(40, 40), L"▶", "start", font));
	buttons.push_back(Button(sf::Vector2f(50, 10), sf::Vector2f(40, 40), L"■", "stop", font, sf::Vector2i(-1, 0)));
	buttons.push_back(Button(sf::Vector2f(90, 10), sf::Vector2f(40, 40), L"▌▌", "pause", font, sf::Vector2i(7, 9)));
	buttons.push_back(Button(sf::Vector2f(130, 10), sf::Vector2f(40, 40), L"▶▌", "next", font, sf::Vector2i(5, 10)));
	buttons.push_back(Button(sf::Vector2f(10, 60), sf::Vector2f(120, 40), L"LOSUJ", "rand", font, sf::Vector2i(-3, 3)));
	for (int i = 0; i <= 8; i++)
	{
		check_boxes.push_back(Checkbox(sf::Vector2f(10 + i * 15, 255), false, "life" + std::to_string(i)));
		Label(sf::Vector2f(13 + i * 15, 240), vect, std::to_wstring(i), 14);
		if (i == 2 || i == 3)
			check_boxes.back().click();
	}
	for (int i = 0; i <= 8; i++)
	{
		check_boxes.push_back(Checkbox(sf::Vector2f(10 + i * 15, 305), false, "resp" + std::to_string(i)));
		Label(sf::Vector2f(13 + i * 15, 290), vect, std::to_wstring(i), 14);
		if (i == 3)
			check_boxes.back().click();
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				for (std::vector<Button>::iterator it = buttons.begin(); it != buttons.end(); it++)
				{
					if (event.mouseButton.x > it->position.x && event.mouseButton.x < it->position.x + it->size.x &&
						event.mouseButton.y > it->position.y && event.mouseButton.y < it->position.y + it->size.y)
					{
						std::string name = it->name;
						it->click();
						if (name == "removeX")
						{
							matrix.change_size(-1, 0);
						}
						else if (name == "addX")
						{
							matrix.change_size(1, 0);
						}
						else if (name == "addY")
						{
							matrix.change_size(0, 1);
						}
						else if (name == "removeY")
						{
							matrix.change_size(0, -1);
						}
						else if (name == "start")
						{
							go = true;
						}
						else if (name == "pause")
						{
							go = false;
						}
						else if (name == "stop")
						{
							go = false;
							matrix.clear();
							gen_text.setString(L"POKOLENIE: " + std::to_wstring(matrix.gen));
						}
						else if (name == "rand")
						{
							matrix.random();
						}
						else if (name == "add_speed")
						{
							if (++speed > 20)
								speed--;
							speed_text.setString(L"SZYBKOŚĆ: " + std::to_wstring(speed));
						}
						else if (name == "remove_speed")
						{
							if (--speed < 1)
								speed++;
							speed_text.setString(L"SZYBKOŚĆ: " + std::to_wstring(speed));
						}
						else if (name == "next")
						{
							if (!matrix.step())
								go = 0;
							gen_text.setString(L"POKOLENIE: " + std::to_wstring(matrix.gen));
						}
					}
				}
				for (std::vector<Checkbox>::iterator it = check_boxes.begin(); it != check_boxes.end(); it++)
				{
					if (event.mouseButton.x > it->position.x && event.mouseButton.x < it->position.x + it->size.x &&
						event.mouseButton.y > it->position.y && event.mouseButton.y < it->position.y + it->size.y)
					{
						if (it->click())
						{
							if (it->name.substr(0, 4) == "life")
								matrix.live.insert(it->name[4] - '0');
							else if (it->name.substr(0, 4) == "resp")
								matrix.resp.insert(it->name[4] - '0');
							else if (it->name == "color")
							{
								matrix.colors(true);
							}
						}
						else
						{
							if (it->name.substr(0, 4) == "life")
								matrix.live.erase(it->name[4] - '0');
							else if (it->name.substr(0, 4) == "resp")
								matrix.resp.erase(it->name[4] - '0');
							else if (it->name == "color")
							{
								matrix.colors(false);
							}
						}
					}
				}
				matrix.check_clicked(event.mouseButton);
			}
		}
		if (go && clock.getElapsedTime().asMilliseconds() >= 1000 / speed)
		{
			if (!matrix.step())
				go = 0;
			gen_text.setString(L"POKOLENIE: " + std::to_wstring(matrix.gen));
			clock.restart();
		}
		window.clear(sf::Color(255, 255, 255, 0));
		window.draw(matrix, sf::RenderStates());
		for (std::vector<sf::Drawable*>::iterator it = vect.begin(); it != vect.end(); it++)
		{
			window.draw(**it);
		}
		for (const auto& it : buttons)
		{
			it.draw(window, sf::RenderStates());
		}
		for (const auto& it : check_boxes)
		{
			it.draw(window, sf::RenderStates());
		}
		window.display();
		for (std::vector<Button>::iterator it = buttons.begin(); it != buttons.end(); it++)
		{
			it->reset();
		}
	}
	return 0;
}
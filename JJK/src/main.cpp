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
#include "label.h"

const int DEFAULT_SPEED = 10;
const sf::Vector2u WINDOW_SIZE = sf::Vector2u(640, 450);

int main()
{
	sf::Font font;
	bool go = false;
	int speed = DEFAULT_SPEED;
	font.loadFromFile("../lucida.ttf");
	sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "Game of life");
	std::vector<const sf::Drawable*> drawables;
	std::vector<Button> buttons;
	std::vector<Checkbox> check_boxes;
	sf::Clock clock;
	CellMatrix matrix = CellMatrix(sf::Vector2f(250, 60), sf::Vector2i(10, 10));
	drawables.push_back(&matrix);
	Label speed_text = Label(sf::Vector2f(10, 115), "Speed: " + std::to_string(speed), 15, font);
	drawables.push_back(&speed_text);
	Label rules_text = Label(sf::Vector2f(10, 190), "Rules:", 20, font);
	drawables.push_back(&rules_text);
	Label life_text = Label(sf::Vector2f(10, 220), "Living:", 15, font);
	drawables.push_back(&life_text);
	Label resp_text = Label(sf::Vector2f(10, 280), "Spawn:", 15, font);
	drawables.push_back(&resp_text);
	Label color_text = Label(sf::Vector2f(10, 390), "Color:", 20, font);
	drawables.push_back(&color_text);
	Label gen_text = Label(sf::Vector2f(10, 350), "Generation: " + std::to_string(matrix.generation_counter), 15, font);
	drawables.push_back(&gen_text);
	check_boxes.push_back(Checkbox(sf::Vector2f(80, 395), false, "color"));
	buttons.push_back(Button(sf::Vector2f(250, 10), sf::Vector2f(30, 30), L"-", "removeX", font, sf::Vector2i(-1, -3)));
	buttons.push_back(Button(sf::Vector2f(310, 10), sf::Vector2f(30, 30), L"+", "addX", font, sf::Vector2i(-1, 0)));
	buttons.push_back(Button(sf::Vector2f(200, 60), sf::Vector2f(30, 30), L"-", "removeY", font, sf::Vector2i(-1, -3)));
	buttons.push_back(Button(sf::Vector2f(200, 120), sf::Vector2f(30, 30), L"+", "addY", font, sf::Vector2i(-1, 0)));
	buttons.push_back(Button(sf::Vector2f(10, 140), sf::Vector2f(30, 30), L"-", "remove_speed", font, sf::Vector2i(-1, -3)));
	buttons.push_back(Button(sf::Vector2f(70, 140), sf::Vector2f(30, 30), L"+", "add_speed", font, sf::Vector2i(-1, 0)));
	buttons.push_back(Button(sf::Vector2f(10, 10), sf::Vector2f(40, 40), L"▶", "start", font));
	buttons.push_back(Button(sf::Vector2f(50, 10), sf::Vector2f(40, 40), L"■", "stop", font, sf::Vector2i(-1, 0)));
	buttons.push_back(Button(sf::Vector2f(90, 10), sf::Vector2f(40, 40), L"▌▌", "pause", font, sf::Vector2i(7, 9)));
	buttons.push_back(Button(sf::Vector2f(130, 10), sf::Vector2f(40, 40), L"▶▌", "next", font, sf::Vector2i(5, 10)));
	buttons.push_back(Button(sf::Vector2f(10, 60), sf::Vector2f(120, 40), L"Random", "rand", font, sf::Vector2i(-3, 3)));
	const int LINE_X = 180;
	sf::VertexBuffer line(sf::PrimitiveType::Lines, sf::VertexBuffer::Usage::Static);
	line.create(2);
	std::vector<sf::Vertex> vertices = { sf::Vertex(sf::Vector2f(LINE_X, 0), sf::Color::Black),  sf::Vertex(sf::Vector2f(LINE_X, WINDOW_SIZE.y), sf::Color::Black), };
	line.update(vertices.data());
	drawables.push_back(&line);
	for (int i = 0; i <= 8; i++)
	{
		check_boxes.push_back(Checkbox(sf::Vector2f(10 + i * 15, 255), false, "life" + std::to_string(i)));
		drawables.push_back(new Label(sf::Vector2f(13 + i * 15, 240), std::to_string(i), 14, font));
		if (i == 2 || i == 3)
			check_boxes.back().click();
	}
	for (int i = 0; i <= 8; i++)
	{
		check_boxes.push_back(Checkbox(sf::Vector2f(10 + i * 15, 315), false, "resp" + std::to_string(i)));
		drawables.push_back(new Label(sf::Vector2f(13 + i * 15, 300), std::to_string(i), 14, font));
		if (i == 3)
			check_boxes.back().click();
	}
	for (const auto& it : buttons)
	{
		drawables.push_back(&it);
	}
	for (const auto& it : check_boxes)
	{
		drawables.push_back(&it);
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
							matrix.change_size(sf::Vector2i(-1, 0));
						}
						else if (name == "addX")
						{
							matrix.change_size(sf::Vector2i(1, 0));
						}
						else if (name == "addY")
						{
							matrix.change_size(sf::Vector2i(0, 1));
						}
						else if (name == "removeY")
						{
							matrix.change_size(sf::Vector2i(0, -1));
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
							gen_text.set_string("Generation: " + std::to_string(matrix.generation_counter));
						}
						else if (name == "rand")
						{
							matrix.random();
						}
						else if (name == "add_speed")
						{
							if (++speed > 20)
								speed--;
							speed_text.set_string("Speed: " + std::to_string(speed));
						}
						else if (name == "remove_speed")
						{
							if (--speed < 1)
								speed++;
							speed_text.set_string("Speed: " + std::to_string(speed));
						}
						else if (name == "next")
						{
							if (!matrix.step())
								go = 0;
							gen_text.set_string("Generation: " + std::to_string(matrix.generation_counter));
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
								matrix.life.insert(it->name[4] - '0');
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
								matrix.life.erase(it->name[4] - '0');
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
			gen_text.set_string("Generation: " + std::to_string(matrix.generation_counter));
			clock.restart();
		}
		window.clear(sf::Color::White);
		//Drawing
		for (const auto& it : drawables)
		{
			window.draw(*it, sf::RenderStates());
		}
		window.display();
		for (std::vector<Button>::iterator it = buttons.begin(); it != buttons.end(); it++)
		{
			it->reset();
		}
	}
	return 0;
}
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
const int MIN_SPEED = 1;
const int MAX_SPEED = 20;

void change_speed(int* speed, Label* speed_label, int speed_change)
{
	*speed += speed_change;
	if (*speed > MAX_SPEED)
	{
		*speed = MAX_SPEED;
	}
	if (*speed < MIN_SPEED)
	{
		*speed = MIN_SPEED;
	}
	speed_label->set_string("Speed: " + std::to_string(*speed));
}

void stop(bool* go, CellMatrix* matrix, Label* generation_label)
{
	*go = false;
	matrix->clear();
	generation_label->set_string("Generation: 0");
}

void step(bool* go, CellMatrix* matrix, Label* generation_label)
{
	if (!matrix->step())
		*go = 0;
	generation_label->set_string("Generation: " + std::to_string(matrix->get_generation_number()));
}

void start(bool* go, sf::Clock* clock)
{
	*go = true;
	clock->restart();
}

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
	float time_sum = 0;
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
	Label gen_text = Label(sf::Vector2f(10, 350), "Generation: 0", 15, font);
	drawables.push_back(&gen_text);
	check_boxes.push_back(Checkbox(sf::Vector2f(80, 395), false));
	check_boxes.back().add_switch_listener([&matrix](bool state) {matrix.colors(state); });
	buttons.reserve(12);
	buttons.emplace_back(sf::Vector2f(250, 10), sf::Vector2f(30, 30), L"-", font, sf::Vector2f(-1, -3));
	buttons.back().add_click_listener(std::bind([&matrix]() {matrix.change_size(sf::Vector2i(-1, 0)); }));
	buttons.emplace_back(sf::Vector2f(310, 10), sf::Vector2f(30, 30), L"+", font, sf::Vector2f(-1, 0));
	buttons.back().add_click_listener(std::bind([&matrix]() {matrix.change_size(sf::Vector2i(1, 0)); }));
	buttons.emplace_back(sf::Vector2f(200, 60), sf::Vector2f(30, 30), L"-", font, sf::Vector2f(-1, -3));
	buttons.back().add_click_listener(std::bind([&matrix]() {matrix.change_size(sf::Vector2i(0, -1)); }));
	buttons.emplace_back(sf::Vector2f(200, 120), sf::Vector2f(30, 30), L"+", font, sf::Vector2f(-1, 0));
	buttons.back().add_click_listener(std::bind([&matrix]() {matrix.change_size(sf::Vector2i(0, 1)); }));
	buttons.emplace_back(sf::Vector2f(10, 140), sf::Vector2f(30, 30), L"-", font, sf::Vector2f(-1, -3));
	buttons.back().add_click_listener(std::bind(&change_speed, &speed, &speed_text, -1));
	buttons.emplace_back(sf::Vector2f(70, 140), sf::Vector2f(30, 30), L"+", font, sf::Vector2f(-1, 0));
	buttons.back().add_click_listener(std::bind(&change_speed, &speed, &speed_text, 1));
	buttons.emplace_back(sf::Vector2f(10, 10), sf::Vector2f(40, 40), L"▶", font);
	buttons.back().add_click_listener(std::bind(&start, &go, &clock));
	buttons.emplace_back(sf::Vector2f(50, 10), sf::Vector2f(40, 40), L"■", font, sf::Vector2f(-1, 0));
	buttons.back().add_click_listener(std::bind(&stop, &go, &matrix, &gen_text));
	buttons.emplace_back(sf::Vector2f(90, 10), sf::Vector2f(40, 40), L"▌▌", font, sf::Vector2f(7, 9));
	buttons.back().add_click_listener(std::bind([&go]() {go = false; }));
	buttons.emplace_back(sf::Vector2f(130, 10), sf::Vector2f(40, 40), L"▶▌", font, sf::Vector2f(5, 10));
	buttons.back().add_click_listener(std::bind(&step, &go, &matrix, &gen_text));
	buttons.emplace_back(sf::Vector2f(10, 60), sf::Vector2f(120, 40), L"Random", font, sf::Vector2f(-3, 3));
	buttons.back().add_click_listener(std::bind([&matrix]() {matrix.random(); }));
	const int LINE_X = 180;
	sf::VertexBuffer line(sf::PrimitiveType::Lines, sf::VertexBuffer::Usage::Static);
	line.create(2);
	std::vector<sf::Vertex> vertices = { sf::Vertex(sf::Vector2f(LINE_X, 0), sf::Color::Black),  sf::Vertex(sf::Vector2f(LINE_X, WINDOW_SIZE.y), sf::Color::Black), };
	line.update(vertices.data());
	drawables.push_back(&line);
	/*for (int i = 0; i <= 8; i++)
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
	}*/
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
				sf::Vector2i mouse_pos = { event.mouseButton.x, event.mouseButton.y };
				for (auto& it : buttons)
				{
					it.click(mouse_pos);
				}
				for (auto& it : check_boxes)
				{
					it.click(mouse_pos);
					/*if (event.mouseButton.x > it->position.x && event.mouseButton.x < it->position.x + it->size.x &&
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
					}*/
				}
				matrix.check_clicked(event.mouseButton);
			}
		}
		if (go)
		{
			time_sum += clock.getElapsedTime().asMicroseconds() / 1000.0;
			if (time_sum >= 1000.f / speed)
			{
				if (!matrix.step())
					go = 0;
				gen_text.set_string("Generation: " + std::to_string(matrix.get_generation_number()));
				time_sum -= 1000.f / speed;
			}
		}
		for (auto& it : buttons)
		{
			it.update(clock.getElapsedTime().asMicroseconds() / 1000000.0, sf::Mouse::getPosition(window));
		}
		clock.restart();
		//Drawing
		window.clear(sf::Color::White);
		for (const auto& it : drawables)
		{
			window.draw(*it, sf::RenderStates());
		}
		window.display();
	}
	return 0;
}
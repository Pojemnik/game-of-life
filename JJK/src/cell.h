#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <string>

class Cell
{

private:
	static const std::array<sf::Color, 12> colors;

public:
	sf::RectangleShape rect;
	bool state = false;

	Cell(sf::Vector2f pos);
	void click();
	void set_color(int color);
};

bool operator ==(const Cell& a, const Cell& b);
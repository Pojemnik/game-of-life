#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <string>

class Cell
{

private:
	static const std::array<sf::Color, 12> colors;
	std::array<sf::Color, 12>::const_iterator current_color;

public:
	sf::RectangleShape rect;
	bool state = false;

	Cell(sf::Vector2f pos);
	void click();
	void next_color();
	void reset_color();
};

bool operator ==(const Cell& a, const Cell& b);
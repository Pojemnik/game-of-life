#include "cell.h"

const std::array<sf::Color, 12> Cell::colors = { sf::Color(255,0,0,255), sf::Color(255,128,0,255), sf::Color(255,255,0,255),
	sf::Color(128,255,0,255), sf::Color(0,255,0,255), sf::Color(0, 255, 128,255), sf::Color(0,255,255,255),
	sf::Color(0, 128, 255,255), sf::Color(0, 0, 255,255), sf::Color(128, 0, 255,255), sf::Color(255, 0, 255,255),
	sf::Color(255, 0, 128,255) };

Cell::Cell(sf::Vector2f pos)
{
	rect.setPosition(pos);
	rect.setSize(sf::Vector2f(10, 10));
	rect.setFillColor(sf::Color::White);
	rect.setOutlineThickness(-1);
	rect.setOutlineColor(sf::Color::Black);
}

void Cell::click()
{
	state = !state;
	if (state)
	{
		rect.setFillColor(sf::Color::Black);
	}
	else
	{
		rect.setFillColor(sf::Color::White);
		rect.setOutlineThickness(-1);
		rect.setOutlineColor(sf::Color::Black);
	}
}

void Cell::set_color(int color)
{
	if (color == -1)
		rect.setFillColor(sf::Color::Black);
	else
	{
		if (color > -1 && color < 12)
			rect.setFillColor(colors[color]);
	}
}

bool operator ==(const Cell& a, const Cell& b)
{
	if (b.state == a.state)
		return true;
	return false;
}
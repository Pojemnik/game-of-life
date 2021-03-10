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
	current_color = colors.begin();
}

void Cell::change_state()
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

void Cell::next_color()
{
	if (++current_color == colors.end())
	{
		current_color = colors.begin();
	}
	rect.setFillColor(*current_color);
}

sf::FloatRect Cell::get_rect() const
{
	return rect.getGlobalBounds();
}

void Cell::reset_color()
{
	current_color = colors.begin();
	rect.setFillColor(sf::Color::Black);
}

bool Cell::get_state() const
{
	return state;
}

void Cell::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rect, states);
}

bool operator ==(const Cell& a, const Cell& b)
{
	if (b.get_state() == a.get_state())
		return true;
	return false;
}
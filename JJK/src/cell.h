#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <string>

class Cell : public sf::Drawable
{

private:
	static const std::array<sf::Color, 12> colors;

	std::array<sf::Color, 12>::const_iterator current_color;
	bool state = false;
	sf::RectangleShape rect;

public:
	Cell(sf::Vector2f pos);
	void change_state();
	void next_color();
	bool get_state() const;
	sf::FloatRect get_rect() const;
	void reset_color();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

bool operator ==(const Cell& a, const Cell& b);
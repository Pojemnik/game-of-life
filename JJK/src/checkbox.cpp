#include "checkbox.h"

Checkbox::Checkbox(sf::Vector2f position_, bool checked_, std::string name_) : position(position_), checked(checked_), name(name_)
{
	rect.setPosition(position);
	rect.setSize(sf::Vector2f(15, 15));
	size = sf::Vector2f(15, 15);
	if (checked_)
	{
		rect.setFillColor(sf::Color(0, 0, 0, 255));
	}
	else
	{
		rect.setFillColor(sf::Color(255, 255, 255, 255));
		rect.setOutlineThickness(-1);
		rect.setOutlineColor(sf::Color(0, 0, 0, 255));
	}
}

bool Checkbox::click()
{
	checked = !checked;
	if (checked)
	{
		rect.setFillColor(sf::Color(0, 0, 0, 255));
	}
	else
	{
		rect.setFillColor(sf::Color(255, 255, 255, 255));
		rect.setOutlineThickness(-1);
		rect.setOutlineColor(sf::Color(0, 0, 0, 255));
	}
	return checked;
}

void Checkbox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rect, states);
}
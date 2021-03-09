#include "checkbox.h"

const sf::Vector2f Checkbox::SIZE = sf::Vector2f(15, 15);
const sf::Color Checkbox::CHECKED_COLOR = sf::Color::Black;
const sf::Color Checkbox::HIGHLIGHTED_COLOR = sf::Color(200, 200, 200, 255);
const sf::Color Checkbox::UNCHECKED_COLOR = sf::Color::White;

void Checkbox::set_checked_state_color(bool new_state)
{
	if (new_state)
	{
		rect.setFillColor(CHECKED_COLOR);
	}
	else
	{
		rect.setFillColor(UNCHECKED_COLOR);
	}
}

Checkbox::Checkbox(sf::Vector2f position_, bool default_state) : Clickable(position_, SIZE), checked(default_state)
{
	rect.setPosition(position_);
	rect.setSize(SIZE);
	rect.setOutlineThickness(-1);
	rect.setOutlineColor(sf::Color::Black);
	set_checked_state_color(default_state);
}

void Checkbox::click(sf::Vector2i mouse_pos)
{
	if (active_zone_contains_point(mouse_pos))
	{
		checked = !checked;
		set_checked_state_color(checked);
		broadcast_swich(checked);
	}
}

void Checkbox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rect, states);
}

bool Checkbox::get_check_state() const
{
	return checked;
}

void Checkbox::update(float dt, sf::Vector2i mouse_pos)
{
	if (active_zone_contains_point(mouse_pos))
	{
		if (state == Clickable_state::DEFAULT)
		{
			state = Clickable_state::HIGHLIGHTED;
			rect.setFillColor(HIGHLIGHTED_COLOR);
		}
	}
	else
	{
		if (state == Clickable_state::HIGHLIGHTED)
		{
			state = Clickable_state::DEFAULT;
			set_checked_state_color(checked);
		}
	}
}

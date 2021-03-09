#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "clickable.h"

class Checkbox : public sf::Drawable, public Clickable
{
private:
	const static sf::Vector2f SIZE;
	const static sf::Color CHECKED_COLOR;
	const static sf::Color HIGHLIGHTED_COLOR;
	const static sf::Color UNCHECKED_COLOR;

	sf::RectangleShape rect;
	bool checked;
	Clickable_state state = Clickable_state::DEFAULT;

	void set_checked_state_color(bool new_state);

public:

	Checkbox(sf::Vector2f position_, bool default_state);
	void click(sf::Vector2i mouse_pos);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void update(float dt, sf::Vector2i mouse_pos);
	bool get_check_state() const;
};
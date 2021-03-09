#pragma once
#include <SFML/Graphics.hpp>
#include <string>

#include "clickable.h"

class Button : public sf::Drawable, public Clickable
{
private:
	sf::RectangleShape rect;
	sf::Text text;
	Clickable_state state = Clickable_state::DEFAULT;
	float timer = 0;
	static const float CLICK_TIME;
	static const sf::Color CLICK_COLOR;
	static const sf::Color HIGHLIGHT_COLOR;
	void on_click();

public:
	Button(sf::Vector2f position_, sf::Vector2f size_, std::wstring content,
		sf::Font& font, sf::Vector2f text_offset = sf::Vector2f(0, 0));
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void update(float dt, sf::Vector2i mouse_pos);
};
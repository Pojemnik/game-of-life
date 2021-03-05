#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button : sf::Drawable
{
private:
	sf::RectangleShape rect;
	sf::Text text;
	sf::Clock clock;
	bool clicked = false;
	const sf::Color CLICK_COLOR = sf::Color(102, 178, 255, 255);

public:
	sf::Vector2f size;
	sf::Vector2f position;
	std::string name;

	Button(sf::Vector2f position_, sf::Vector2f size_, std::wstring content,
		std::string name_, sf::Font& font, sf::Vector2i text_offset = sf::Vector2i(0, 0));
	void click();
	void reset();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
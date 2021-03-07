#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Label : public sf::Drawable
{

private:
	sf::Text text;

public:
	Label(sf::Vector2f pos, std::string text_, unsigned int size, sf::Font& font);
	void set_string(std::string s);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

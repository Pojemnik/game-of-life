#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Checkbox : sf::Drawable
{
private:
	sf::RectangleShape rect;

public:
	bool checked;
	sf::Vector2f position;
	std::string name;
	sf::Vector2f size;

	Checkbox(sf::Vector2f position_, bool checked_, std::string name_);
	bool click();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
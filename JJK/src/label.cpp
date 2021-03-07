#include "label.h"

Label::Label(sf::Vector2f pos, std::string content, unsigned int font_size, sf::Font& font) : text(content, font, font_size)
{
	text.setFillColor(sf::Color::Black);
	text.setStyle(sf::Text::Bold);
	text.setPosition(pos);
}

void Label::set_string(std::string s)
{
	text.setString(s);
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(text, states);
}

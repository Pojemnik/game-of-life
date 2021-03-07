#include "button.h"

Button::Button(sf::Vector2f position_, sf::Vector2f size_, std::wstring content, std::string name_,
	sf::Font& font, sf::Vector2i text_offset) : position(position_), size(size_), name(name_),
	text(content, font, 25)
{
	rect.setPosition(position);
	rect.setSize(size);
	rect.setFillColor(sf::Color::White);
	rect.setOutlineThickness(1.0f);
	rect.setOutlineColor(sf::Color::Black);
	sf::FloatRect bounds = text.getLocalBounds();
	text.setPosition(position.x + (size.x - bounds.width - bounds.left) / 2 + text_offset.x, position.y + (size.y - bounds.height - bounds.top) / 2 - 5 + text_offset.y);
	text.setFillColor(sf::Color::Black);
	text.setStyle(sf::Text::Bold);
	if (content == L"▌▌" || content == L"▶▌")
		text.setCharacterSize(20);
}

void Button::click()
{
	rect.setFillColor(CLICK_COLOR);
	text.setFillColor(sf::Color::White);
	clock.restart();
	clicked = true;
}

void Button::reset()
{
	if (clicked && clock.getElapsedTime().asMilliseconds() >= 50)
	{
		rect.setFillColor(sf::Color::White);
		text.setFillColor(sf::Color::Black);
		clicked = false;
	}
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rect, states);
	target.draw(text, states);
}
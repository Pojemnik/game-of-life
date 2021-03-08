#include "button.h"

void Button::on_click()
{
	state = Clickable_state::CLICKING;
	timer = 0.f;
	rect.setFillColor(CLICK_COLOR);
	text.setFillColor(sf::Color::White);
}

Button::Button(sf::Vector2f position_, sf::Vector2f size_, std::wstring content,
	sf::Font& font, sf::Vector2f text_offset) :	text(content, font, 25), Clickable(position_, size_)
{
	rect.setPosition(position_);
	rect.setSize(size_);
	rect.setFillColor(sf::Color::White);
	rect.setOutlineThickness(1.0f);
	rect.setOutlineColor(sf::Color::Black);
	sf::FloatRect bounds = text.getLocalBounds();
	sf::Vector2f centering_offset = sf::Vector2f(size_.x - bounds.width - bounds.left, size_.y - bounds.height - bounds.top) / 2.f;
	sf::Vector2f position = position_ + centering_offset + text_offset;
	position.y -= 5;
	text.setPosition(position);
	text.setFillColor(sf::Color::Black);
	text.setStyle(sf::Text::Bold);
	if (content == L"▌▌" || content == L"▶▌")
		text.setCharacterSize(20);
	add_click_listener(std::bind(&Button::on_click, this));
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rect, states);
	target.draw(text, states);
}

void Button::update(float dt, sf::Vector2i mouse_pos)
{
	if (active_zone_contains_point(mouse_pos))
	{
		if (state == Clickable_state::DEFAULT)
		{
			state = Clickable_state::HIGHLIGHTED;
			rect.setFillColor(HIGHLIGHT_COLOR);
		}
	}
	else
	{
		if (state == Clickable_state::HIGHLIGHTED)
		{
			state = Clickable_state::DEFAULT;
			rect.setFillColor(sf::Color::White);
		}
	}
	if (state == Clickable_state::CLICKING)
	{
		timer += dt;
		if (timer >= CLICK_TIME)
		{
			timer = 0;
			state = Clickable_state::DEFAULT;
		}
	}
}

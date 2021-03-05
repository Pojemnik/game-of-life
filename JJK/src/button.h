#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <set>
#include <Windows.h>
#include <cstdlib>
#include <ctime>

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
	sf::Vector2f pos;
	std::string name;

	Button(sf::Vector2f p, sf::Vector2f s, std::wstring txt, std::string n, 
		sf::Font* font, sf::Vector2i text_offset = sf::Vector2i(0, 0)) : pos(p), size(s), name(n),
		text(txt, *font, 30), rect()
	{
		rect.setPosition(pos);
		rect.setSize(size);
		rect.setFillColor(sf::Color::White);
		rect.setOutlineThickness(1.0f);
		rect.setOutlineColor(sf::Color::Black);
		sf::FloatRect bounds = text.getLocalBounds();
		text.setPosition(pos.x + (size.x - bounds.width - bounds.left) / 2 + text_offset.x, pos.y + (size.y - bounds.height - bounds.top) / 2 - 5 + text_offset.y);
		text.setFillColor(sf::Color::Black);
		text.setStyle(sf::Text::Bold);
		if (txt == L"▌▌" || txt == L"▶▌")
			text.setCharacterSize(20);
	}

	void click()
	{
		rect.setFillColor(CLICK_COLOR);
		text.setFillColor(sf::Color::White);
		clock.restart();
		clicked = true;
	}

	void reset()
	{
		if (clicked && clock.getElapsedTime().asMilliseconds() >= 50)
		{
			rect.setFillColor(sf::Color::White);
			text.setFillColor(sf::Color::Black);
			clicked = false;
		}
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(rect, states);
		target.draw(text, states);
	}
};
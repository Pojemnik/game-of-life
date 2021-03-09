#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class Clickable
{
	sf::FloatRect active_zone;
	std::vector<std::function<void()>> click_listeners;
	std::vector<std::function<void(bool)>> switch_listeners;

protected:
	bool active_zone_contains_point(sf::Vector2i point) const;
	void broadcast_click();
	void broadcast_swich(bool state);

public:
	Clickable(sf::FloatRect active_zone_);
	Clickable(sf::Vector2f position, sf::Vector2f size);
	virtual void add_click_listener(std::function<void()> listener);
	virtual void add_switch_listener(std::function<void(bool)> listener);
	virtual void click(sf::Vector2i mouse_pos);
	virtual void update(float dt, sf::Vector2i mouse_pos) = 0;

	enum class Clickable_state
	{
		DEFAULT,
		HIGHLIGHTED,
		CLICKING
	};
};
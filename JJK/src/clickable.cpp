#include "clickable.h"

bool Clickable::active_zone_contains_point(sf::Vector2i point) const
{
	sf::FloatRect mouse_rect = sf::FloatRect(static_cast<sf::Vector2f>(point), { 1,1 });
	return active_zone.intersects(mouse_rect);
}

Clickable::Clickable(sf::FloatRect active_zone_) : active_zone(active_zone_) {}

Clickable::Clickable(sf::Vector2f position, sf::Vector2f size) : active_zone(position, size) {}

void Clickable::add_click_listener(std::function<void()> listener)
{
	click_listeners.push_back(listener);
}

void Clickable::click(sf::Vector2i mouse_pos)
{
	if (active_zone_contains_point(mouse_pos))
	{
		for (const auto& it : click_listeners)
		{
			it();
		}
	}
}

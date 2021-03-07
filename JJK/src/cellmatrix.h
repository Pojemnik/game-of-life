#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <set>

#include "cell.h"

class CellMatrix : public sf::Drawable
{
private:
	std::vector<std::vector<Cell>> cells;
	std::vector<std::vector<Cell>> last_cells;
	sf::Vector2f pos;
	std::vector<std::vector<int>> lifespan;
	bool color;
	const sf::Vector2i MAX_SIZE = sf::Vector2i(35, 35);
	const sf::Vector2i MIN_SIZE = sf::Vector2i(5, 5);

public:
	std::set<int> resp;
	std::set<int> live;
	int generation;

	CellMatrix(sf::Vector2f p, int x, int y);
	void change_size(sf::Vector2i diff);
	void reset_lifespan();
	void check_clicked(sf::Event::MouseButtonEvent mouse);
	bool step();
	void clear();
	void random();
	void colors(bool state);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
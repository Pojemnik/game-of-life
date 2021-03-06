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
	int** lifespan;
	bool color;

public:
	std::set<int> resp;
	std::set<int> live;
	int gen;

	CellMatrix(sf::Vector2f p, int x, int y);
	void change_size(int x, int y);
	void check_clicked(sf::Event::MouseButtonEvent mouse);
	bool step();
	void clear();
	void random();
	void colors(bool state);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
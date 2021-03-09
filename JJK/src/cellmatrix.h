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
	sf::Vector2f position;
	std::vector<std::vector<int>> lifespan; //currently unused
	bool color;
	const sf::Vector2i MAX_SIZE = sf::Vector2i(35, 35);
	const sf::Vector2i MIN_SIZE = sf::Vector2i(5, 5);
	const int DEFAULT_RESP_VALUES[1] = { 3 };
	const int DEFAULT_LIFE_VALUES[2] = { 2,3 };
	int generation_counter;
	std::array<std::set<int>, 2> rules;

	std::vector<std::vector<int>> calculate_neighbours();

public:
	enum class Simulation_rule
	{
		RESPAWN = 0,
		LIVING
	};

	CellMatrix(sf::Vector2f position_, sf::Vector2i size);
	int get_generation_number() const;
	void change_size(sf::Vector2i diff);
	void reset_lifespan();
	void check_clicked(sf::Event::MouseButtonEvent mouse);
	bool step();
	void clear();
	void random();
	void colors(bool state);
	void set_simulation_rule(Simulation_rule rule, int neighbours, bool value);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
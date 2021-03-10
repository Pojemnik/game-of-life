#include "cellmatrix.h"

std::vector<std::vector<int>> CellMatrix::calculate_neighbours()
{
	std::vector<std::vector<int>> neighbours;
	std::vector<int> temp(cells[0].size(), 0);
	for (int i = 0; i < cells.size(); i++)
	{
		neighbours.push_back(temp);
	}
	const int delta_x[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	const int delta_y[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	for (int i = 0; i < cells.size(); i++)
	{
		for (int j = 0; j < cells[0].size(); j++)
		{
			if (!cells[i][j].get_state())
			{
				continue;
			}
			for (int k = 0; k < 8; k++)
			{
				int x = i + delta_x[k];
				int y = j + delta_y[k];
				if (x < 0 || x >= cells.size())
					continue;
				if (y < 0 || y >= cells[0].size())
					continue;
				neighbours[x][y]++;
			}
		}
	}
	return neighbours;
}

CellMatrix::CellMatrix(sf::Vector2f position_, sf::Vector2i size) : position(position_),
	generation_counter(0), color(false), cells(size.x)
{
	for (const auto& it : DEFAULT_RESP_VALUES)
	{
		set_simulation_rule(Simulation_rule::RESPAWN, it, true);
	}
	for (const auto& it : DEFAULT_LIFE_VALUES)
	{
		set_simulation_rule(Simulation_rule::LIVING, it, true);
	}
	std::vector<int> temp(size.y, 0);
	for (int i = 0; i < size.x; i++)
	{
		lifespan.push_back(temp);
		for (int j = 0; j < size.y; j++)
		{
			cells[i].push_back(Cell(sf::Vector2f(position.x + 11 * i, position.y + 11 * j)));
		}
	}
	update_rect();
}

int CellMatrix::get_generation_number() const
{
	return generation_counter;
}

void CellMatrix::change_size(sf::Vector2i diff)
{
	if (cells.size() + diff.x > MAX_SIZE.x || cells[0].size() + diff.y > MAX_SIZE.y ||
		cells.size() + diff.x < MIN_SIZE.x || cells[0].size() + diff.y < MIN_SIZE.y)
		return;
	while (diff.x > 0)
	{
		std::vector<Cell> temp;
		for (int i = 0; i < cells[0].size(); i++)
			temp.push_back(Cell(sf::Vector2f(position.x + cells.size() * 11, position.y + 11 * i)));
		cells.push_back(temp);
		diff.x--;
	}
	while (diff.x < 0)
	{
		cells.pop_back();
		diff.x++;
	}
	while (diff.y < 0)
	{
		for (auto& it : cells)
		{
			it.pop_back();
		}
		diff.y++;
	}
	while (diff.y > 0)
	{
		for (int i = 0; i < cells.size(); i++)
			cells[i].push_back(Cell(sf::Vector2f(position.x + i * 11, position.y + 11 * cells[i].size())));
		diff.y--;
	}
	reset_lifespan();
	update_rect();
}

void CellMatrix::update_rect()
{
	const sf::Vector2f position = { cells[0][0].get_rect().left, cells[0][0].get_rect().top };
	const sf::FloatRect lower_right_cell = cells.back().back().get_rect();
	const sf::Vector2f size = sf::Vector2f(lower_right_cell.left, lower_right_cell.top) +
		sf::Vector2f(lower_right_cell.width, lower_right_cell.height) - position;
	rect = sf::FloatRect(position, size);
}

void CellMatrix::reset_lifespan()
{
	lifespan.clear();
	std::vector<int> temp(cells[0].size(), 0);
	for (int i = 0; i < cells.size(); i++)
	{
		lifespan.push_back(temp);
	}
}

void CellMatrix::check_clicked(sf::Event::MouseButtonEvent mouse)
{
	sf::FloatRect mouse_rect = sf::FloatRect(sf::Vector2f(mouse.x, mouse.y), { 0.1f, 0.1f });
	if (rect.intersects(mouse_rect))
	{
		for (auto& row : cells)
		{
			for (auto& it : row)
			{
				const sf::FloatRect rect = it.get_rect();
				const sf::Vector2f left_up = sf::Vector2f(rect.left, rect.top);
				const sf::Vector2f right_bottom = sf::Vector2f(rect.width, rect.height) + left_up;
				if (mouse.x > left_up.x && mouse.x < right_bottom.x && mouse.y > left_up.y && mouse.y < right_bottom.y)
					it.change_state();
			}
		}
	}
}

bool CellMatrix::step()
{
	std::vector<std::vector<int>> neighbours = calculate_neighbours();
	int changed_cells = 0;
	for (int i = 0; i < cells.size(); i++)
	{
		for (int j = 0; j < cells[0].size(); j++)
		{
			if (cells[i][j].get_state())
			{
				if (!rules[static_cast<int>(Simulation_rule::LIVING)].count(neighbours[i][j]))
				{
					cells[i][j].change_state();
					lifespan[i][j] = 0;
					changed_cells++;
					continue;
				}
				lifespan[i][j]++;
				if (color)
				{
					cells[i][j].next_color();
				}
			}
			else
			{
				if (rules[static_cast<int>(Simulation_rule::RESPAWN)].count(neighbours[i][j]))
				{
					cells[i][j].change_state();
					changed_cells++;
				}
			}
		}
	}
	generation_counter++;
	return (changed_cells != 0);
}

void CellMatrix::clear()
{
	generation_counter = 0;
	for (auto& row : cells)
	{
		for (auto& it : row)
		{
			if (it.get_state())
				it.change_state();
		}
	}
	reset_lifespan();
}

void CellMatrix::random()
{
	srand(time(NULL));
	clear();
	for (auto& row : cells)
	{
		for (auto& it : row)
		{
			if (rand() % 2)
			{
				it.change_state();
			}
		}
	}
}

void CellMatrix::colors(bool state)
{
	color = state;
	if (!state)
	{
		for (auto& row : cells)
		{
			for (auto& it : row)
			{
				if (it.get_state())
					it.reset_color();
			}
		}
	}
}

void CellMatrix::set_simulation_rule(Simulation_rule rule, int neighbours, bool value)
{
	if (value)
	{
		rules[static_cast<int>(rule)].insert(neighbours);
	}
	else
	{
		rules[static_cast<int>(rule)].erase(neighbours);
	}
}

void CellMatrix::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& row : cells)
	{
		for (auto& it : row)
		{
			it.draw(target, states);
		}
	}
}
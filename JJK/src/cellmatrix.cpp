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
			if (!cells[i][j].state)
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
		resp.insert(it);
	}
	for (const auto& it : DEFAULT_LIFE_VALUES)
	{
		life.insert(it);
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
		int x = cells.size();
		for (int i = 0; i < x; i++)
		{
			cells[i].pop_back();
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
	if (mouse.x > cells[0][0].rect.getPosition().x && mouse.y > cells[0][0].rect.getPosition().y &&
		mouse.x < cells[cells.size() - 1][cells[0].size() - 1].rect.getPosition().x + cells[cells.size() - 1][cells[0].size() - 1].rect.getSize().x &&
		mouse.y < cells[cells.size() - 1][cells[0].size() - 1].rect.getPosition().y + cells[cells.size() - 1][cells[0].size() - 1].rect.getSize().y)
	{
		for (int i = 0; i < cells.size(); i++)
		{
			for (int j = 0; j < cells[i].size(); j++)
			{
				sf::Vector2f p = cells[i][j].rect.getPosition();
				sf::Vector2f s = cells[i][j].rect.getSize();
				if (mouse.x > p.x && mouse.x < p.x + s.x && mouse.y > p.y && mouse.y < p.y + s.y)
					cells[i][j].click();
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
			if (cells[i][j].state)
			{
				if (!life.count(neighbours[i][j]))
				{
					cells[i][j].click();
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
				if (resp.count(neighbours[i][j]))
				{
					cells[i][j].click();
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
	for (int i = 0; i < cells.size(); i++)
	{
		for (int j = 0; j < cells[0].size(); j++)
		{
			if (cells[i][j].state)
				cells[i][j].click();
		}
	}
	reset_lifespan();
}

void CellMatrix::random()
{
	srand(time(NULL));
	clear();
	for (int i = 0; i < cells.size(); i++)
	{
		for (int j = 0; j < cells[0].size(); j++)
		{
			if (rand() % 2)
			{
				cells[i][j].click();
			}
		}
	}
}

void CellMatrix::colors(bool state)
{
	color = state;
	if (!state)
	{
		for (int i = 0; i < cells.size(); i++)
		{
			for (int j = 0; j < cells[0].size(); j++)
			{
				if (cells[i][j].state)
					cells[i][j].reset_color();
			}
		}
	}
}

void CellMatrix::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& it : cells)
	{
		for (const auto& it2 : it)
		{
			target.draw(it2.rect, states);
		}
	}
}
#include "cellmatrix.h"

CellMatrix::CellMatrix(sf::Vector2f p, int x, int y) : pos(p)
{
	color = false;
	resp.insert(3);
	live.insert(2);
	live.insert(3);
	cells.resize(x);
	lifespan = new int* [x];
	for (int i = 0; i < x; i++)
	{
		lifespan[i] = new int[y];
		for (int j = 0; j < y; j++)
		{
			cells[i].push_back(Cell(sf::Vector2f(pos.x + 11 * i, pos.y + 11 * j)));
			lifespan[i][j] = 0;
		}
	}
	gen = 0;
}

void CellMatrix::change_size(int x, int y)
{
	if (cells.size() + x > 35 || cells[0].size() + y > 35 ||
		cells.size() + x < 5 || cells[0].size() + y < 5)
		return;
	delete[] lifespan;
	int a = x;
	int b = y;
	while (a > 0)
	{
		std::vector<Cell> temp;
		for (int i = 0; i < cells[0].size(); i++)
			temp.push_back(Cell(sf::Vector2f(pos.x + cells.size() * 11, pos.y + 11 * i)));
		cells.push_back(temp);
		a--;
	}
	/*while (a < 0)
	{
		int x = cells[0].size();
		for (int i = 0; i < x; i++)
		{
			std::vector<sf::Drawable*>::iterator it = vect.begin();
			while (it != vect.end())
			{
				if (*it == cells[cells.size() - 1][i].rect)
				{
					vect.erase(it);
					break;
				}
				it++;
			}
		}
		cells.pop_back();
		a++;
	}*/
	/*while (b < 0)
	{
		int x = cells.size();
		for (int i = 0; i < x; i++)
		{
			std::vector<sf::Drawable*>::iterator it = vect.begin();
			while (it != vect.end())
			{
				if (*it == cells[i][cells[i].size() - 1].rect)
				{
					vect.erase(it);
					break;
				}
				it++;
			}
			cells[i].pop_back();
		}
		b++;
	}*/
	while (b > 0)
	{
		for (int i = 0; i < cells.size(); i++)
			cells[i].push_back(Cell(sf::Vector2f(pos.x + i * 11, pos.y + 11 * cells[i].size())));
		b--;
	}
	lifespan = new int* [cells.size()];
	for (int i = 0; i < cells.size(); i++)
	{
		lifespan[i] = new int[cells[0].size()];
		for (int j = 0; j < cells[0].size(); j++)
			lifespan[i][j] = 0;
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
	int** n;
	n = new int* [cells.size()];
	for (int i = 0; i < cells.size(); i++)
	{
		n[i] = new int[cells[0].size()];
		for (int j = 0; j < cells[0].size(); j++)
			n[i][j] = 0;
	}
	for (int i = 0; i < cells.size(); i++)
	{
		for (int j = 0; j < cells[0].size(); j++)
		{
			if (cells[i][j].state)
			{
				if (i < cells.size() - 1)
				{
					n[i + 1][j]++;
				}
				if (j < cells[0].size() - 1)
				{
					n[i][j + 1]++;
					if (i < cells.size() - 1)
					{
						n[i + 1][j + 1]++;
					}
				}
				if (i - 1 >= 0)
				{
					n[i - 1][j]++;
				}
				if (j - 1 >= 0)
				{
					n[i][j - 1]++;
					if (i - 1 >= 0)
					{
						n[i - 1][j - 1]++;
					}
				}
				if (i < cells.size() - 1 && j - 1 >= 0)
				{
					n[i + 1][j - 1]++;
				}
				if (j < cells[0].size() - 1 && i - 1 >= 0)
				{
					n[i - 1][j + 1]++;
				}
			}
		}
	}
	for (int i = 0; i < cells.size(); i++)
	{
		for (int j = 0; j < cells[0].size(); j++)
		{
			if (cells[i][j].state)
			{
				lifespan[i][j]++;
				if (color)
					cells[i][j].set_color((lifespan[i][j] - 1) % 12);
				if (!live.count(n[i][j]))
					cells[i][j].click();
			}
			else
			{
				lifespan[i][j] = 0;
				if (resp.count(n[i][j]))
					cells[i][j].click();
			}
		}
	}
	if (last_cells == cells)
		return 0;
	last_cells = cells;
	delete[] n;
	gen++;
	return 1;
}

void CellMatrix::clear()
{
	delete[] lifespan;
	gen = 0;
	for (int i = 0; i < cells.size(); i++)
	{
		for (int j = 0; j < cells[0].size(); j++)
		{
			if (cells[i][j].state)
				cells[i][j].click();
		}
	}
	lifespan = new int* [cells.size()];
	for (int i = 0; i < cells.size(); i++)
	{
		lifespan[i] = new int[cells[0].size()];
		for (int j = 0; j < cells[0].size(); j++)
			lifespan[i][j] = 0;
	}
}

void CellMatrix::random()
{
	srand(time(NULL));
	this->clear();
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
					cells[i][j].set_color(-1);
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
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <set>
#include <Windows.h>
#include <cstdlib>
#include <ctime>

#include "button.h"
#include "checkbox.h"

sf::Font font;

class Cell
{

private:
	sf::Color colors[12] = { sf::Color(255,0,0,255), sf::Color(255,128,0,255), sf::Color(255,255,0,255),
	sf::Color(128,255,0,255), sf::Color(0,255,0,255), sf::Color(0, 255, 128,255), sf::Color(0,255,255,255),
	sf::Color(0, 128, 255,255), sf::Color(0, 0, 255,255), sf::Color(128, 0, 255,255), sf::Color(255, 0, 255,255),
	sf::Color(255, 0, 128,255) };

public:
	sf::RectangleShape* rec;
	bool state = false;

	Cell(sf::Vector2f pos, std::vector<sf::Drawable*> &vect)
	{
		rec = new sf::RectangleShape();
		rec->setPosition(pos);
		rec->setSize(sf::Vector2f(10, 10));
		rec->setFillColor(sf::Color(255, 255, 255, 255));
		rec->setOutlineThickness(-1);
		rec->setOutlineColor(sf::Color(0, 0, 0, 255));
		vect.push_back(rec);
	}

	void click()
	{
		state = !state;
		if (state)
		{
			rec->setFillColor(sf::Color(0, 0, 0, 255));
		}
		else
		{
			rec->setFillColor(sf::Color(255, 255, 255, 255));
			rec->setOutlineThickness(-1);
			rec->setOutlineColor(sf::Color(0, 0, 0, 255));
		}
	}

	void set_color(int color)
	{
		if (color == -1)
			rec->setFillColor(sf::Color(0, 0, 0, 255));
		else
		{
			if (color > -1 && color < 12)
				rec->setFillColor(colors[color]);
		}
	}
};

class CellMatrix
{
private:
	std::vector<std::vector<Cell> > cells;
	std::vector<std::vector<Cell> > last_cells;
	sf::Vector2f pos;
	std::vector<sf::Drawable*>* v;
	int** lifespan;
	bool color;

public:
	std::set<int> resp;
	std::set<int> live;
	int gen;

	CellMatrix(sf::Vector2f p, int x, int y, std::vector<sf::Drawable*> &vect) : pos(p), v(&vect)
	{
		color = false;
		resp.insert(3);
		live.insert(2);
		live.insert(3);
		cells.resize(x);
		lifespan = new int*[x];
		for (int i = 0; i < x; i++)
		{
			lifespan[i] = new int[y];
			for (int j = 0; j < y; j++)
			{
				cells[i].push_back(Cell(sf::Vector2f(pos.x + 11 * i, pos.y + 11 * j), vect));
				lifespan[i][j] = 0;
			}
		}
		gen = 0;
	}

	void change_size(int x, int y, std::vector<sf::Drawable*> &vect)
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
				temp.push_back(Cell(sf::Vector2f(pos.x + cells.size() * 11, pos.y + 11 * i), *v));
			cells.push_back(temp);
			a--;
		}
		while (a < 0)
		{
			int x = cells[0].size();
			for (int i = 0; i < x; i++)
			{
				std::vector<sf::Drawable*>::iterator it = vect.begin();
				while (it != vect.end())
				{
					if (*it == cells[cells.size() - 1][i].rec)
					{
						vect.erase(it);
						break;
					}
					it++;
				}
			}
			cells.pop_back();
			a++;
		}
		while (b < 0)
		{
			int x = cells.size();
			for (int i = 0; i < x; i++)
			{
				std::vector<sf::Drawable*>::iterator it = vect.begin();
				while (it != vect.end())
				{
					if (*it == cells[i][cells[i].size() - 1].rec)
					{
						vect.erase(it);
						break;
					}
					it++;
				}
				cells[i].pop_back();
			}
			b++;
		}
		while (b > 0)
		{
			for (int i = 0; i < cells.size(); i++)
				cells[i].push_back(Cell(sf::Vector2f(pos.x + i * 11, pos.y + 11 * cells[i].size()), *v));
			b--;
		}
		lifespan = new int*[cells.size()];
		for (int i = 0; i < cells.size(); i++)
		{
			lifespan[i] = new int[cells[0].size()];
			for (int j = 0; j < cells[0].size(); j++)
				lifespan[i][j] = 0;
		}
	}

	void check_clicked(sf::Event::MouseButtonEvent mouse)
	{
		if (mouse.x > cells[0][0].rec->getPosition().x && mouse.y > cells[0][0].rec->getPosition().y &&
			mouse.x < cells[cells.size() - 1][cells[0].size() - 1].rec->getPosition().x + cells[cells.size() - 1][cells[0].size() - 1].rec->getSize().x &&
			mouse.y < cells[cells.size() - 1][cells[0].size() - 1].rec->getPosition().y + cells[cells.size() - 1][cells[0].size() - 1].rec->getSize().y)
		{
			for (int i = 0; i < cells.size(); i++)
			{
				for (int j = 0; j < cells[i].size(); j++)
				{
					sf::Vector2f p = cells[i][j].rec->getPosition();
					sf::Vector2f s = cells[i][j].rec->getSize();
					if (mouse.x > p.x && mouse.x < p.x + s.x && mouse.y > p.y && mouse.y < p.y + s.y)
						cells[i][j].click();
				}
			}
		}
	}

	bool step()
	{
		int** n;
		n = new int*[cells.size()];
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

	void clear()
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
		lifespan = new int*[cells.size()];
		for (int i = 0; i < cells.size(); i++)
		{
			lifespan[i] = new int[cells[0].size()];
			for (int j = 0; j < cells[0].size(); j++)
				lifespan[i][j] = 0;
		}
	}

	void random()
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

	void colors(bool state)
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
};

class Label
{

private:
	sf::Text* txt;

public:
	Label(sf::Vector2f pos, std::vector<sf::Drawable*> &vect, std::wstring text, unsigned int size)
	{
		txt = new sf::Text(text, font, size);
		txt->setFillColor(sf::Color(0, 0, 0, 255));
		txt->setStyle(sf::Text::Bold);
		txt->setPosition(pos);
		vect.push_back(txt);
	}

	void setString(std::wstring s)
	{
		txt->setString(s);
	}
};

bool operator ==(const Cell &a, const Cell &b)
{
	if (b.state == a.state)
		return true;
	return false;
}

int main()
{
	bool go = false;
	int speed = 10;
	std::vector<sf::Drawable*> vect;
	font.loadFromFile("../lucida.ttf");
	sf::RenderWindow window(sf::VideoMode(800, 450), "Game of life");
	CellMatrix matrix = CellMatrix(sf::Vector2f(400, 60), 10, 10, vect);
	std::vector<Button> buttons;
	std::vector<Checkbox> check_boxes;
	sf::Clock clock;
	Label speed_text = Label(sf::Vector2f(10, 120), vect, L"SZYBKOŚĆ: " + std::to_wstring(speed), 20);
	Label rules_text = Label(sf::Vector2f(10, 190), vect, L"ZASADY:", 20);
	Label life_text = Label(sf::Vector2f(10, 220), vect, L"ŻYCIE:", 15);
	Label resp_text = Label(sf::Vector2f(10, 270), vect, L"ODRADZANIE:", 15);
	Label color_text = Label(sf::Vector2f(10, 350), vect, L"KOLOR:", 20);
	check_boxes.push_back(Checkbox(sf::Vector2f(10, 380), false, "color"));
	Label gen_text = Label(sf::Vector2f(10, 320), vect, L"POKOLENIE: " + std::to_wstring(matrix.gen), 15);
	buttons.push_back(Button(sf::Vector2f(400, 10), sf::Vector2f(30, 30), L"-", "removeX", font, sf::Vector2i(-1, -5)));
	buttons.push_back(Button(sf::Vector2f(460, 10), sf::Vector2f(30, 30), L"+", "addX", font, sf::Vector2i(-1, 0)));
	buttons.push_back(Button(sf::Vector2f(350, 60), sf::Vector2f(30, 30), L"-", "removeY", font, sf::Vector2i(-1, -5)));
	buttons.push_back(Button(sf::Vector2f(350, 120), sf::Vector2f(30, 30), L"+", "addY", font, sf::Vector2i(-1, 0)));
	buttons.push_back(Button(sf::Vector2f(10, 150), sf::Vector2f(30, 30), L"-", "remove_speed", font, sf::Vector2i(-1, -5)));
	buttons.push_back(Button(sf::Vector2f(70, 150), sf::Vector2f(30, 30), L"+", "add_speed", font, sf::Vector2i(-1, 0)));
	buttons.push_back(Button(sf::Vector2f(10, 10), sf::Vector2f(40, 40), L"▶", "start", font));
	buttons.push_back(Button(sf::Vector2f(50, 10), sf::Vector2f(40, 40), L"■", "stop", font, sf::Vector2i(-1, 0)));
	buttons.push_back(Button(sf::Vector2f(90, 10), sf::Vector2f(40, 40), L"▌▌", "pause", font, sf::Vector2i(7, 9)));
	buttons.push_back(Button(sf::Vector2f(130, 10), sf::Vector2f(40, 40), L"▶▌", "next", font, sf::Vector2i(5, 10)));
	buttons.push_back(Button(sf::Vector2f(10, 60), sf::Vector2f(120, 40), L"LOSUJ", "rand", font, sf::Vector2i(-3, 3)));
	for (int i = 0; i <= 8; i++)
	{
		check_boxes.push_back(Checkbox(sf::Vector2f(10 + i * 15, 255), false, "life" + std::to_string(i)));
		Label(sf::Vector2f(13 + i * 15, 240), vect, std::to_wstring(i), 14);
		if (i == 2 || i == 3)
			check_boxes.back().click();
	}
	for (int i = 0; i <= 8; i++)
	{
		check_boxes.push_back(Checkbox(sf::Vector2f(10 + i * 15, 305), false, "resp" + std::to_string(i)));
		Label(sf::Vector2f(13 + i * 15, 290), vect, std::to_wstring(i), 14);
		if (i == 3)
			check_boxes.back().click();
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				for (std::vector<Button>::iterator it = buttons.begin(); it != buttons.end(); it++)
				{
					if (event.mouseButton.x > it->position.x && event.mouseButton.x < it->position.x + it->size.x &&
						event.mouseButton.y > it->position.y && event.mouseButton.y < it->position.y + it->size.y)
					{
						std::string name = it->name;
						it->click();
						if (name == "removeX")
						{
							matrix.change_size(-1, 0, vect);
						}
						else if (name == "addX")
						{
							matrix.change_size(1, 0, vect);
						}
						else if (name == "addY")
						{
							matrix.change_size(0, 1, vect);
						}
						else if (name == "removeY")
						{
							matrix.change_size(0, -1, vect);
						}
						else if (name == "start")
						{
							go = true;
						}
						else if (name == "pause")
						{
							go = false;
						}
						else if (name == "stop")
						{
							go = false;
							matrix.clear();
							gen_text.setString(L"POKOLENIE: " + std::to_wstring(matrix.gen));
						}
						else if (name == "rand")
						{
							matrix.random();
						}
						else if (name == "add_speed")
						{
							if (++speed > 20)
								speed--;
							speed_text.setString(L"SZYBKOŚĆ: " + std::to_wstring(speed));
						}
						else if (name == "remove_speed")
						{
							if (--speed < 1)
								speed++;
							speed_text.setString(L"SZYBKOŚĆ: " + std::to_wstring(speed));
						}
						else if (name == "next")
						{
							if (!matrix.step())
								go = 0;
							gen_text.setString(L"POKOLENIE: " + std::to_wstring(matrix.gen));
						}
					}
				}
				for (std::vector<Checkbox>::iterator it = check_boxes.begin(); it != check_boxes.end(); it++)
				{
					if (event.mouseButton.x > it->position.x && event.mouseButton.x < it->position.x + it->size.x &&
						event.mouseButton.y > it->position.y && event.mouseButton.y < it->position.y + it->size.y)
					{
						if (it->click())
						{
							if (it->name.substr(0, 4) == "life")
								matrix.live.insert(it->name[4] - '0');
							else if (it->name.substr(0, 4) == "resp")
								matrix.resp.insert(it->name[4] - '0');
							else if (it->name == "color")
							{
								matrix.colors(true);
							}
						}
						else
						{
							if (it->name.substr(0, 4) == "life")
								matrix.live.erase(it->name[4] - '0');
							else if (it->name.substr(0, 4) == "resp")
								matrix.resp.erase(it->name[4] - '0');
							else if (it->name == "color")
							{
								matrix.colors(false);
							}
						}
					}
				}
				matrix.check_clicked(event.mouseButton);
			}
		}
		if (go && clock.getElapsedTime().asMilliseconds() >= 1000 / speed)
		{
			if (!matrix.step())
				go = 0;
			gen_text.setString(L"POKOLENIE: " + std::to_wstring(matrix.gen));
			clock.restart();
		}
		window.clear(sf::Color(255, 255, 255, 0));
		for (std::vector<sf::Drawable*>::iterator it = vect.begin(); it != vect.end(); it++)
		{
			window.draw(**it);
		}
		for (const auto& it : buttons)
		{
			it.draw(window, sf::RenderStates());
		}
		for (const auto& it : check_boxes)
		{
			it.draw(window, sf::RenderStates());
		}
		window.display();
		for (std::vector<Button>::iterator it = buttons.begin(); it != buttons.end(); it++)
		{
			it->reset();
		}
	}
	return 0;
}
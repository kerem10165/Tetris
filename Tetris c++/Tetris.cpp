#include "Tetris.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using std::cout;
using std::endl;

Tetris::Tetris(const float uz_gen, const sf::Color& renk, const std::array<std::array<int, 2>, 5>& dizi)
	:sf::RectangleShape(sf::Vector2f(uz_gen, uz_gen))
{
	sf::RectangleShape::setOrigin(10, 10);
	diziOlustur(dizi);
	sf::RectangleShape::setFillColor(renk);
}

const Tetris& Tetris::operator=(const Tetris&& a) noexcept
{
	diziOlustur(a.dizi);
	RectangleShape::setFillColor(a.getFillColor());

	return *this;
}

void Tetris::setCizKarePos(const int i)
{
	sf::RectangleShape::setPosition(sf::Vector2f(dizi[i][0], dizi[i][1]));
}

void Tetris::diziOlustur(const std::array<std::array<int, 2>, 5>& a)
{
	srand(time(NULL));
	int h = rand() % 10;

	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			if (j == 0)
			{
				dizi[i][j] = a[i][j] + (h*20);
			}

			else
			{
				dizi[i][j] = a[i][j];
			}
		}
	}
}

void Tetris::diziOlustur(const int a[5][2])
{

	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			if (j == 0)
			{
				dizi[i][j] = a[i][j];
			}

			else
			{
				dizi[i][j] = a[i][j];
			}
		}
	}
}

void Tetris::asagiHareket()
{
	for (size_t i = 0; i < 5; i++)
	{
		dizi[i][1] += 20;
	}
}

void Tetris::sagSolHareket(int a)
{
	if (a == 0)
	{
		for (size_t i = 0; i < 5; i++)
		{
			dizi[i][0] -= 20;
		}
	}

	if (a == 1)
	{
		for (size_t i = 0; i < 5; i++)
		{
			dizi[i][0] += 20;
		}
	}
}


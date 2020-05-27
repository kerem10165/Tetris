#include <SFML/Graphics.hpp>
#include <array>
#pragma once

class Tetris :public sf::RectangleShape
{
public:
	Tetris(const float uz_gen, const sf::Color& renk, const std::array<std::array<int, 2>, 5>& dizi);

	const Tetris& operator=(const Tetris&& a) noexcept;
	int dizi[5][2];

	void setCizKarePos(const int i);
	void diziOlustur(const std::array<std::array<int, 2>, 5>& a);
	void diziOlustur(const int a[5][2]);
	void asagiHareket();
	void sagSolHareket(int a);
private:
	
};


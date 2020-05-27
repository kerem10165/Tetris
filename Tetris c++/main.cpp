#include "SFML-2.5.1/include/SFML/Graphics.hpp"
#include <iostream>
#include <array>
#include <time.h>
#include <future>
#include <mutex>
#include <map>
#include <cstdlib>
#include <ctime>
#include "Tetris.h";

using namespace sf;
using std::cout;
using std::endl;

const int yukseklik = 20;
const int genislik = 15;
const float kare = 20;

std::array<std::array<std::array<int, 2>, 5>, 7> a
{
	30,-50, 30,-30 ,30,-10, 10,-10 ,30,-30, //L
	10+60,-50 ,10+60,-30 ,10+60,-10 ,30+60,-10 ,10+60,-30,//Duvara Yasli L
	10,-70 ,10,-50 ,10,-30 ,10,-10 ,10,-50,//I
	10,-10, 30,-10 ,30,-30 ,50,-10 ,30,-10, //garip sekil
	10,-30, 10,-10 ,30,-30 ,30,-10 ,20,-20, //kare
	10,-30, 30,-30 ,30,-10 ,50,-10 ,30,-30, //z
	50,-30, 30,-30 ,30,-10 ,10,-10 ,30,-30  //ters z
};

std::map<int, Color> harita =
{
	{0,Color::Red},
	{1,Color::Green},
	{2,Color::Blue},
	{3,Color::Magenta},
	{4,Color::Cyan},
	{5,Color::Yellow}
};


short int cntrl[yukseklik+5][genislik];

Color cntrl_renk[yukseklik+5][genislik];


void cevir(Tetris& tet);
void ciz(RenderWindow& pencere,RectangleShape& dik, RectangleShape& yatay);
bool kontrol(Tetris& tet);
bool zeroKontrol(Tetris& tet);
void uret(Tetris& tet);
bool cevirmeKontrol(Tetris& tet,int genislik,int yukseklik);
void sil();


int main()
{
	srand(time(NULL));
	
	RenderWindow pencere(VideoMode((genislik * kare), (yukseklik * kare)), "Tetris");

	//pencere.setFramerateLimit(60);

	std::future<void> t1;

	RectangleShape dik;
	RectangleShape yatay;
	dik.setSize(sf::Vector2f(1, yukseklik * kare));
	yatay.setSize(Vector2f(genislik * kare, 1));

	Tetris s(kare, sf::Color::Red, a[6]);

	RectangleShape cizim;
	cizim.setSize(Vector2f(kare,kare));
	cizim.setFillColor(Color::Red);



	for (size_t i = 0; i < yukseklik; i++)
	{
		for (size_t j = 0; j < genislik; j++)
		{
			cntrl[i][j] = 0;
		}
	}

	float dt;
	Clock sayac;
	sf::Clock saat;
	Clock asagi_kontrol;
	Clock sag_sol;
	Clock asagi_klavye;
	asagi_klavye.restart();
	sag_sol.restart();
	asagi_kontrol.restart();
	sayac.restart();

	while (pencere.isOpen())
	{
		Event olay;

		while (pencere.pollEvent(olay))
		{
			if (olay.type == sf::Event::Closed)
			{
				pencere.close();
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Up) && sayac.getElapsedTime().asMilliseconds() > 300)
		{
			cevir(s);
			cevirmeKontrol(s, genislik, yukseklik);
			sayac.restart();
		}



		if (Keyboard::isKeyPressed(Keyboard::Left) && sag_sol.getElapsedTime().asMilliseconds() > 100)
		{
			auto lamda =  [](Tetris& tet, short int dizi[25][15]) -> bool
			{
				for (size_t i = 0; i < 4; i++)
				{
					if (tet.dizi[i][0] - 10 == 0)
					{
						++i;
					}
					if (dizi[(tet.dizi[i][1]-10)/20][((tet.dizi[i][0]-10)/20)-1] == 1)
					{
						return false;
					}
				}
				return true;
			};

			if (lamda(s, cntrl))
			{
				bool h = true;
				for (size_t i = 0; i < 4; i++)
				{
					if (s.dizi[i][0] + 10 <= (20))
					{
						h = false;
					}
				}
				if (h)
				{
					s.sagSolHareket(0);
				}
			}

			sag_sol.restart();
		}

		if (Keyboard::isKeyPressed(Keyboard::Right) && sag_sol.getElapsedTime().asMilliseconds() > 100)
		{
			auto lamda = [](Tetris& tet, short int dizi[25][15],int genislik) -> bool
			{
				
				for (size_t i = 0; i < 4; i++)
				{
					if (tet.dizi[i][0] - 10 == (genislik*20)-20)
					{
						++i;
					}
					if (dizi[(tet.dizi[i][1] - 10) / 20][((tet.dizi[i][0] - 10) / 20) +1 ])
					{
						return false;
					}
				}
				return true;
			};

			if (lamda(s, cntrl,genislik))
			{
				bool h = true;
				for (size_t i = 0; i < 4; i++)
				{
					if (s.dizi[i][0]+10 >= (genislik*20))
					{
						h = false;
					}
				}
				if (h)
				{
					s.sagSolHareket(1);
				}
			}
			sag_sol.restart();
		}

		if (Keyboard::isKeyPressed(Keyboard::Down) && asagi_klavye.getElapsedTime().asMilliseconds() > 100)
		{
			if (!kontrol(s))
			{
				if (!zeroKontrol(s))
				{
					s.asagiHareket();
				}

				else
				{
					uret(s);
				}
			}
			
			else
			{
				uret(s);
			}

			asagi_klavye.restart();
		}

		dt = saat.restart().asSeconds();
		
		if (asagi_kontrol.getElapsedTime().asSeconds() > 0.2)
		{
			if (!kontrol(s))
			{
				if (!zeroKontrol(s))
				{
					s.asagiHareket();
				}

				else
				{
					uret(s);
				}
			}

			else
			{
				uret(s);
			}

			asagi_kontrol.restart();
		}

		sil();

		pencere.clear();

		for (size_t i = 0; i < 5; i++)
		{
			s.setCizKarePos(i); 
			pencere.draw(s);
		}

		for (size_t i = 0; i < yukseklik; i++)
		{
			for (size_t j = 0; j < genislik; j++)
			{
				if (cntrl[i][j] == 1)
				{
					cizim.setPosition(j*20 , i*20);
					cizim.setFillColor(cntrl_renk[i][j]);
					pencere.draw(cizim);
				}
			}
		}

		ciz(pencere, dik, yatay);

		pencere.display();
	}

	return 0;
}


void cevir(Tetris& tet)
{
	int x, y;

	for (int i = 0; i < 4; i++)
	{

		x = tet.dizi[i][0] - tet.dizi[4][0];
		y = tet.dizi[i][1] - tet.dizi[4][1];

		tet.dizi[i][0] = -y + tet.dizi[4][0];
		tet.dizi[i][1] = x + tet.dizi[4][1];
	}
}

void ciz(RenderWindow& pencere, RectangleShape& dik, RectangleShape& yatay)
{
	for (int i = 0; i < genislik+1; i++)
	{
		pencere.draw(dik);
		if (i < genislik-1)
		{
			dik.move(20, 0);
		}
		else
		{
			dik.move(19.4, 0);
		}
	}

	for (int i = 0; i < yukseklik; i++)
	{
		pencere.draw(yatay);
		if (i < yukseklik-1)
		{
			yatay.move(0, 20);
		}
		else
		{
			yatay.move(0, 19.4);
		}
	}

	dik.setPosition(0, 0);
	yatay.setPosition(0, 0);
}

bool kontrol(Tetris& tet)
{
	for (int i = 0; i < 4; ++i)
	{
		if ((cntrl[((tet.dizi[i][1]-10)/20)+1][((tet.dizi[i][0]-10)/20)] == 1))
		{
			for (size_t j = 0; j < 4; ++j)
			{
				cntrl[(tet.dizi[j][1] - 10) / 20][((tet.dizi[j][0] - 10) / 20)] = 1;
				cntrl_renk[(tet.dizi[j][1] - 10) / 20][((tet.dizi[j][0] - 10) / 20)] = tet.getFillColor();
			}
			return true;
		}
	}

	return false;
}

bool zeroKontrol(Tetris& tet)
{
	for (size_t i = 0; i < 4; ++i)
	{
		if (tet.dizi[i][1] - 10 == yukseklik*kare - kare)
		{
			for (size_t j = 0; j < 4; j++)
			{
				cntrl[(tet.dizi[j][1] - 10) / 20][((tet.dizi[j][0] - 10) / 20)] = 1;
				cntrl_renk[(tet.dizi[j][1] - 10) / 20][((tet.dizi[j][0] - 10) / 20)] = tet.getFillColor();
			}
			return true;
		}
	}

	return false;
}

void uret(Tetris& tet)
{
	tet = Tetris(kare, harita.at(rand() % 6), a[rand() % 7]);
}

bool cevirmeKontrol(Tetris& tet, int genislik,int yükseklik)
{
	for (size_t i = 0; i < 4; i++)
	{
		if (tet.dizi[i][1]+10 > yukseklik *20)
		{
			cevir(tet);
			cevir(tet);
			cevir(tet);
			return false;
		}
	}

	for (size_t i = 0; i < 4; i++)
	{
		if (tet.dizi[i][0] + 10 > genislik*20 || tet.dizi[i][0] -10 < 0)
		{
			cevir(tet);
			cevir(tet);
			cevir(tet);
			return false;
		}
	}


	for (size_t i = 0; i < 4; i++)
	{
		if (cntrl[(tet.dizi[i][1] - 10) / 20][(tet.dizi[i][0] - 10) / 20] == 1)
		{
			cout << "hata" << endl;
			cevir(tet);
			cevir(tet);
			cevir(tet);
			return false;
		}
	}

	return true;
}

void sil()
{
	int uzunluk = 0, temp = 0;
	bool flag = false , flag2 = false;

	for (size_t i = 0; i < 20; i++)
	{
		for (size_t j = 0; j < 15; j++)
		{
			if (cntrl[i][j] == 0)
			{
				break;
			}

			if (j == 14)
			{
				if (flag2 == false)
				{
					flag2 = true;
					temp = i;
					cout << i << endl;
				}

				flag = true;
			}
		}

		if (flag)
			++uzunluk;
		flag = false;
	}

	
	if (flag2)
	{
		int temp_tut = temp;
		for (int i = temp - uzunluk; i > 0; --i)
		{
			for (int j = 0; j < 15; ++j)
			{
				cntrl[temp_tut][j] = cntrl[i][j];
				cntrl_renk[temp_tut][j] = cntrl_renk[i][j];
			}

			--temp_tut;
		}
	}

	for (size_t i = 0; i < uzunluk; i++)
	{
		for (size_t j = 0; j < 15; j++)
		{
			cntrl[i][j] = 0;
		}
	}
	
}




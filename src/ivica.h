#ifndef IVICA_H
#define IVICA_H

#include <SFML/Graphics.hpp>
#include <cmath>

extern sf::Vector2f pozicija_stola, dimenzije_stola, senka_vektor;
extern sf::Color boja_senke;

class ivica
{
public:
	sf::Vector2f tacka1, tacka2, pravac;
	float k, n;
	sf::VertexArray linija, senka;
	sf::RenderWindow* prozor;

	ivica()
	{
		prozor = NULL;
		linija = sf::VertexArray(sf::Lines, 2);
		senka = sf::VertexArray(sf::Quads, 4);
	}
	void povezi_grafiku(sf::RenderWindow* prozor1) { this->prozor = prozor1; }
	void podesi(sf::Vector2f t1,sf::Vector2f t2);
	float razdaljina_od(sf::Vector2f A);
	sf::Vector2f getPravac() { return pravac; }
	void crtaj();
	void crtaj_senku();
};

#endif

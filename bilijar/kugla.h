#include <SFML/Graphics.hpp>
#include "ivica.h"
#include <cmath>

class kugla
{
	sf::Color boja;
	sf::Vector2f pozicija;
	sf::Vector2f brzina;

	float masa=1;
	float poluprecnik = 20.f;
	float trenje = 0.1f;
	//grafika
	sf::CircleShape krug;
	sf::RenderWindow* prozor;
public:
	kugla();
	void povezi_grafiku(sf::RenderWindow* prozor1) { this->prozor = prozor1; }
	void podesi(sf::Color b, sf::Vector2f p, sf::Vector2f v);
	void dodeli_brzinu(sf::Vector2f v) { brzina = v; }
	void dodeli_poziciju(sf::Vector2f p) { pozicija = p; }
	sf::Vector2f getPosition() { return pozicija; }
	void osvezi();
	bool sudar_kugli(kugla* druga);
	bool sudar_o_ivicu(ivica ivica1);
	bool sudar_o_teme(sf::Vector2f tacka);
	void crtaj();
};


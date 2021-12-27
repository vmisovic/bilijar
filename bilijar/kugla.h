#ifndef KUGLA_H
#define KUGLA_H

#include <SFML/Graphics.hpp>
#include "ivica.h"
#include <cmath>
#include <string>
#include <sstream>

class kugla
{
	//promenljive
	sf::Vector2f brzina;//vektor brzine sa vrednostima x i y
	sf::Vector2f ugaona_brzina;//vektori ugaone brzine normalne na z i x osu
	sf::Vector2f pozicija;//kordinate centra kugle x i y
    sf::Vector2f rotacija;//x: trenutni ugao s0, y: trenutni ugao t0 odredjene tacke na kugli (tacka referentnog sistema, 0,0 na teksturi)
	float ugao;//prethodni ugao vektora brzine i X ose (moram da pamtim kada se zaustavi kugla za iscrtavanje)
	
	//konstante
	float masa=1;
	float poluprecnik = 20.f;//za sve kugle isti!
	float trenje = 0.1f;

	//grafika
	sf::Image slika;
	sf::CircleShape krug, kruzic;
	sf::RenderWindow* prozor;
public:
	//funkcije za podesavanje kugle
	kugla();
	void povezi_grafiku(sf::RenderWindow* prozor1, int br)
	{ 
		this->prozor = prozor1;
		std::stringstream naziv;
		naziv << "resources/kugla";
		naziv << br << ".png\0";
		if (!slika.loadFromFile(naziv.str()))
			exit(br);
		krug.setFillColor(slika.getPixel(128,128));
		kruzic.setFillColor(slika.getPixel(30, 30));
	}
	void podesi(sf::Vector2f p, sf::Vector2f v);
	void dodeli_brzinu(sf::Vector2f v) { brzina = v; }
	void dodeli_poziciju(sf::Vector2f p) { pozicija = p; }
	void okreni() { rotacija = sf::Vector2f(3.14f, 3.14f); }
	void udarac_stapa(sf::Vector2f poz_mis, float jacina);

	//funkcije za vracanje parametara kugle
	sf::Vector2f getPosition() { return pozicija; }
	bool krece_se();

	//funkcije za tealizacije sudara i pomeranje kugli (menjaju brzine i pozicije kugle usled fizickih zakona)
	void osvezi();
	bool sudar_kugli(kugla* druga);
	bool sudar_o_ivicu(ivica ivica1);
	bool sudar_o_teme(sf::Vector2f tacka);

	//funkcije za iscrtavanje
	void crtaj();
	void crtaj_jednostavno();
	void crtaj_stap(sf::Vector2f poz_mis, float jacina);
};

#endif

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
	int br_frejmova_od_poslednjeg_s = 10;
	
	//konstante
	sf::Color boja;
	float masa=1;
	float poluprecnik = 20.f;
	float trenje = 0.1f;

	//grafika
	sf::CircleShape krug;
	sf::Image slika;
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
	}
	void podesi(sf::Color b, sf::Vector2f p, sf::Vector2f v);
	void dodeli_brzinu(sf::Vector2f v) { brzina = v; }
	void dodeli_poziciju(sf::Vector2f p) { pozicija = p; }

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
	void crtaj_2();
};
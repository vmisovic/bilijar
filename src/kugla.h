#ifndef KUGLA_H
#define KUGLA_H

#include <SFML/Graphics.hpp>
#include <sstream>
#include "ivica.h"

extern sf::Vector2f pozicija_stola, dimenzije_stola, pozicija_rupe[6], senka_vektor;

class kugla
{
	//promenljive
	sf::Vector2f brzina;//vektor brzine sa vrednostima x i y
	sf::Vector2f ugaona_brzina;//vektori ugaone brzine normalne na z i x osu
	sf::Vector2f pozicija;//kordinate centra kugle x i y
    sf::Vector2f rotacija;//x: trenutni ugao s0, y: trenutni ugao t0 odredjene tacke na kugli (tacka referentnog sistema, 0,0 na teksturi)
	float ugao;//prethodni ugao vektora brzine i X ose (moram da pamtim kada se zaustavi kugla za iscrtavanje)
	bool bio_sudar;//1 u koliko je prethodni frejm bio sudar, da se ne bi ponovno pozivale sudar_... funkcije
	bool u_igri;//1 u koliko je kugla na stolu tj. u igri je, u koliko je upala u rupu 0
	bool oznacena;//1 kako bi se iscrtavalo precrtan znak
	bool animacija;

	//konstante
	float masa=1;
	float poluprecnik = 16.f;//za sve kugle isti!
	float trenje = 0.1f;

	//grafika
	sf::Image slika;
	sf::CircleShape krug, kruzic, senka;
	sf::RenderWindow* prozor;
public:
    static int pozicija_nakon_rupe;
	//funkcije za podesavanje kugle
	kugla()
	{
		prozor = NULL;//grafika se prosledjuje grugom funkcijom, kako bi mogao da deklarisem u source.cpp-u niz kugli
		pozicija = sf::Vector2f(200.f, 200.f);
		rotacija = sf::Vector2f(3.14f, 3.14f);//da broj kugle gleda ka ekranu/igracu
		ugaona_brzina = sf::Vector2f(0.f, 0.f);
		ugao = 0.f;
		bio_sudar = 0;
		u_igri = 0;
		oznacena = 0;
		animacija = 0;
		//podesavanje za iscrtavanja
		krug.setRadius(poluprecnik);
		kruzic.setRadius(poluprecnik * 0.7f);
		senka.setRadius(poluprecnik);
		senka.setFillColor(sf::Color(0,0,0,100));

	}
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
	void podesi(sf::Vector2f p, sf::Vector2f v) { pozicija = p; brzina = v; }
	void dodeli_brzinu(sf::Vector2f v) { brzina = v; }
	void dodeli_poziciju(sf::Vector2f p) { pozicija = p; }
	void ubaci_u_igru() { u_igri = 1; }
	void okreni() { rotacija = sf::Vector2f(3.14f, 3.14f); }
	void udarac_stapa(sf::Vector2f poz_mis, float jacina);
	void highlight(bool b) { oznacena = b; }

	//funkcije za vracanje parametara kugle
	sf::Vector2f getPosition() { return pozicija; }
	float getPoluprecnik() { return poluprecnik; }
	bool krece_se();
	bool aktivna() { return u_igri; }

	//funkcije za realizacije sudara i pomeranje kugli (menjaju brzine i pozicije kugle usled fizickih zakona)
	void osvezi();
	bool provera_bio_sudar(bool uslov);
	bool provera_sudara_kugli(kugla *druga);
	bool provera_sudara_o_ivicu(ivica ivica1);
	bool provera_sudara_o_teme(sf::Vector2f tacka);
	bool sudar_kugli(kugla* druga);
	bool sudar_o_ivicu(ivica ivica1);
	bool sudar_o_teme(sf::Vector2f tacka);
	void razdvoji_kugle(kugla *druga);
	void razdvoji_kuglu_od_ivice(ivica ivica1);
	void razdvoji_kuglu_od_temena(sf::Vector2f tacka);
	int usla_u_rupu();

	//funkcije za iscrtavanje
	void crtaj();
	void crtaj_senku();
	void crtaj_jednostavno();
	void crtaj_precrtano();
	void crtaj_stap(sf::Vector2f poz_mis, float jacina);
	void crtaj_stap_jednostavno(sf::Vector2f poz_mis, float jacina);
};

#endif

#ifndef KUGLA_H
#define KUGLA_H

#include <SFML/Graphics.hpp>
#include <sstream>
#include "ivica.h"
#include "matrica.h"
#define PI 3.14159265359

extern sf::Vector2f pozicija_stola, dimenzije_stola, pozicija_rupe[6], senka_vektor;
extern sf::Color boja_stapa, boja_senke;
extern int pozicija_nakon_rupe;


float intenzitet(sf::Vector2f a);
float cos_uglaIzmedjuVektora(sf::Vector2f u, sf::Vector2f v);

class kugla
{
	//promenljive
	sf::Vector2f brzina;//vektor brzine sa vrednostima x i y
	sf::Vector2f ugaona_brzina;//vektori ugaone brzine normalne na z i x osu
	sf::Vector2f pozicija;//kordinate centra kugle x i y
	float alfa, beta, gama;
    matrica mat_rotacije, mat_drotacije, xyz;
	bool bio_sudar;//1 u koliko je prethodni frejm bio sudar, da se ne bi ponovno pozivale sudar_... funkcije
	bool u_igri;//1 u koliko je kugla na stolu tj. u igri je, u koliko je upala u rupu 0
	bool oznacena;//1 kako bi se iscrtavalo precrtan znak
	bool animacija;

	//konstante
    int red_br;
	float masa = 1;
	float poluprecnik = 16.f;//za sve kugle isti!
	float trenje = 0.1f;

	//grafika
	sf::Image slika;
	sf::CircleShape krug, kruzic, senka;
	sf::RenderWindow* prozor;
	sf::VertexArray pointmap;

public:
	//funkcije za podesavanje kugle
	kugla()
	{
        float** memorija_mat_rotacije=new float*[VELICINA_MATRICE];
        for(int i=0;i<VELICINA_MATRICE;i++) memorija_mat_rotacije[i] = new float[VELICINA_MATRICE];
        mat_rotacije.mat=memorija_mat_rotacije;

        mat_rotacije.kolona=3;
        mat_rotacije.red=3;
        mat_rotacije.mat[0][0]=1;
        mat_rotacije.mat[1][1]=1;
        mat_rotacije.mat[2][2]=1;

        float** memorija_mat_drotacije=new float*[VELICINA_MATRICE];
        for(int i=0;i<VELICINA_MATRICE;i++) memorija_mat_drotacije[i] = new float[VELICINA_MATRICE];
        mat_drotacije.mat=memorija_mat_drotacije;

        mat_drotacije.kolona=3;
        mat_drotacije.red=3;

		float** memorija_xyz=new float*[VELICINA_MATRICE];
        for(int i=0;i<VELICINA_MATRICE;i++) memorija_xyz[i] = new float[VELICINA_MATRICE];
        xyz.mat=memorija_xyz;

        red_br=0;
		prozor = NULL;//grafika se prosledjuje grugom funkcijom, kako bi mogao da deklarisem u source.cpp-u niz kugli
		pozicija = sf::Vector2f(200.f, 200.f);
		alfa = 0; beta = 0; gama = 0;
		ugaona_brzina = sf::Vector2f(0.f, 0.f);
		bio_sudar = 0;
		u_igri = 0;
		oznacena = 0;
		animacija = 0;
		//podesavanje za iscrtavanja
		krug.setRadius(poluprecnik);
		kruzic.setRadius(poluprecnik * 0.7f);
		senka.setRadius(poluprecnik);
		senka.setFillColor(sf::Color(0,0,0,100));
        pointmap = sf::VertexArray(sf::Points,(int)(4*poluprecnik*poluprecnik));
	}
	void povezi_grafiku(sf::RenderWindow* prozor1, int br)
	{
        red_br=br;
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
	void okreni() 
	{
		float ugao,c,s;
		ugao=PI/2.f; c=cosf(ugao); s=sinf(ugao);
		mat_rotacije.mat[0][0]=1;
		mat_rotacije.mat[0][1]=0;
        mat_rotacije.mat[0][2]=0;

        mat_rotacije.mat[1][0]=0;
        mat_rotacije.mat[1][1]=c;
        mat_rotacije.mat[1][2]=s;

        mat_rotacije.mat[2][0]=0;
        mat_rotacije.mat[2][1]=-s;
        mat_rotacije.mat[2][2]=c;
    }
	void udarac_stapa(sf::Vector2f poz_mis, float jacina, bool naopacke);
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
	void crtaj_stap(sf::Vector2f poz_mis, float jacina, bool naopacke);
	void crtaj_senku_stapa(sf::Vector2f poz_mis, float jacina, bool naopacke);
	void crtaj_stap_jednostavno(sf::Vector2f poz_mis, float jacina, bool naopacke);
};

#endif

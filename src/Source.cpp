#include <iostream>
#include <SFML/Graphics.hpp>
#include "init.h"
#include "kugla.h"
#include "matrica.h"
using namespace std;

bool krecu_se = 1;
bool jednostavno_crtanje = 0;
bool pomocne_linije = 0;
bool mis_cilja_naopacke = 0;
bool pauzirano = 0;
bool osetljivo = 0;
bool fiksiran_stap = 0;
int tockic = 40;
sf::Vector2f mis;//cuva kordinate strelice misa

int br_ubacenih_kugli=0;
int prethodni_br_ubacenih_kugli=0;

float** memorija_nova;

extern const int br_tacaka;
extern sf::Vector2f tacke[br_tacaka];
extern sf::Vector2f pozicija_stola, dimenzije_stola, pozicija_rupe[6], senka_vektor;

extern const int br_ivica;
extern ivica ivice[br_ivica];

sf::Color boja_stola(10,100,10), boja_stola1(10,130,10), boja_oko_rupa(75,75,75), boja_okvira(100,50,10);
sf::Color boja_senke(0,0,0,100), boja_stapa(230,150,60);
sf::RectangleShape sto, okvir, oko_rupa[6];
sf::CircleShape rupa[6];
sf::VertexArray u_okvir[6];

void update_boja()
{
	okvir.setFillColor(boja_okvira);
	sto.setFillColor(boja_stola);
	for (int i = 0; i < 6; i++)
		oko_rupa[i].setFillColor(boja_oko_rupa);
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 4; j++)
			u_okvir[i][j].color = boja_stola1;
}

void inicijalizuj_grafiku(sf::RenderWindow *prozor)
{
	okvir.setPosition(pozicija_stola - sf::Vector2f(50.f,50.f));
	okvir.setSize(dimenzije_stola + sf::Vector2f(100.f,100.f));
	okvir.setOutlineColor(sf::Color::White);
	okvir.setOutlineThickness(5.f);

	sto.setPosition(pozicija_stola - sf::Vector2f(20.f,20.f));
	sto.setSize(dimenzije_stola + sf::Vector2f(40.f,40.f));
	sto.setOutlineColor(sf::Color::Black);
	sto.setOutlineThickness(5.f);

	for (int i=0; i<6;i++)
	{
		if(i!=1&&i!=4)
		{
			rupa[i].setRadius(20.f*sqrt(2));
			oko_rupa[i].setSize(sf::Vector2f(80.f, 80.f));
		}
		else
		{
			rupa[i].setRadius(27.f);
			oko_rupa[i].setSize(sf::Vector2f(80.f, 40.f));
		}
		rupa[i].setPosition(pozicija_stola + pozicija_rupe[i] - sf::Vector2f(rupa[i].getRadius(),rupa[i].getRadius()));
		rupa[i].setFillColor(sf::Color::Black);
	}
	oko_rupa[0].setPosition(pozicija_stola + pozicija_rupe[0] - sf::Vector2f(60.f, 60.f));
	oko_rupa[1].setPosition(pozicija_stola + pozicija_rupe[1] - sf::Vector2f(40.f, 50.f));
	oko_rupa[2].setPosition(pozicija_stola + pozicija_rupe[2] - sf::Vector2f(20.f, 60.f));
	oko_rupa[3].setPosition(pozicija_stola + pozicija_rupe[3] - sf::Vector2f(20.f, 20.f));
	oko_rupa[4].setPosition(pozicija_stola + pozicija_rupe[4] - sf::Vector2f(40.f, -10.f));
	oko_rupa[5].setPosition(pozicija_stola + pozicija_rupe[5] - sf::Vector2f(60.f, 20.f));

	for (int i = 0; i < 6; i++)
	{
		u_okvir[i].setPrimitiveType(sf::Quads);
		u_okvir[i].resize(4);
	}
	u_okvir[0][0].position=tacke[1]+pozicija_stola;
	u_okvir[0][1].position=tacke[3]+pozicija_stola;
	u_okvir[0][2].position=tacke[6]+pozicija_stola;
	u_okvir[0][3].position=tacke[4]+pozicija_stola;

	u_okvir[1][0].position=tacke[5]+pozicija_stola;
	u_okvir[1][1].position=tacke[7]+pozicija_stola;
	u_okvir[1][2].position=tacke[10]+pozicija_stola;
	u_okvir[1][3].position=tacke[8]+pozicija_stola;

	u_okvir[2][0].position=tacke[9]+pozicija_stola;
	u_okvir[2][1].position=tacke[11]+pozicija_stola;
	u_okvir[2][2].position=tacke[14]+pozicija_stola;
	u_okvir[2][3].position=tacke[12]+pozicija_stola;

	u_okvir[3][0].position=tacke[13]+pozicija_stola;
	u_okvir[3][1].position=tacke[15]+pozicija_stola;
	u_okvir[3][2].position=tacke[18]+pozicija_stola;
	u_okvir[3][3].position=tacke[16]+pozicija_stola;

	u_okvir[4][0].position=tacke[17]+pozicija_stola;
	u_okvir[4][1].position=tacke[19]+pozicija_stola;
	u_okvir[4][2].position=tacke[22]+pozicija_stola;
	u_okvir[4][3].position=tacke[20]+pozicija_stola;

	u_okvir[5][0].position=tacke[21]+pozicija_stola;
	u_okvir[5][1].position=tacke[23]+pozicija_stola;
	u_okvir[5][2].position=tacke[2]+pozicija_stola;
	u_okvir[5][3].position=tacke[0]+pozicija_stola;

	for (int i = 0; i < br_kugli; i++) k[i].povezi_grafiku(prozor, i);
	for (int i = 0; i < br_ivica; i++) ivice[i].povezi_grafiku(prozor);
}

bool slobodno_mesto(sf::Vector2f uneta_poz)//vraca 1 u koliko se na uneta_poz moze postaviti kugla
{
	kugla kp;
	kp.dodeli_poziciju(uneta_poz);
	kp.ubaci_u_igru();
	bool ok_kugle = 1, ok_ivice = 1, ok_tacke = 1, ok_rupa = 1, ok_sto = 1;
	for (int i = 1; i < br_kugli; i++)
		if (kp.provera_sudara_kugli(&k[i]) == 1)
			ok_kugle = 0;
	for (int i = 0; i < br_ivica; i++)
		if (kp.provera_sudara_o_ivicu(ivice[i]))
				ok_ivice = 0;
	for (int i = 0; i < br_tacaka; i++)
		if (kp.provera_sudara_o_teme(tacke[i]) == 1)
			ok_tacke = 0;
	if (kp.usla_u_rupu() != -1)
		ok_rupa = 0;
	if (uneta_poz.x < (-20.f) || uneta_poz.y < (-20.f) || uneta_poz.x > dimenzije_stola.x + 20.f || uneta_poz.y > dimenzije_stola.y + 20.f)
		ok_sto = 0;
	return (ok_kugle && ok_ivice && ok_tacke && ok_rupa && ok_sto);
}

void crtaj_pomocne_linije(sf::Vector2f poz_mis, sf::RenderWindow *prozor)
{
	sf::Vector2f d = pozicija_stola + k[0].getPosition() - poz_mis;
	if (intenzitet(d) == 0) d = sf::Vector2f(1.f, 1.f);
	d /= intenzitet(d);
	if (mis_cilja_naopacke) d *= (-1.f);
	sf::Vector2f poz_provere = k[0].getPosition();
	float pomeraj = k[0].getPoluprecnik();
	while (pomeraj > 2)
	{
		while (slobodno_mesto(poz_provere))
			poz_provere += d * pomeraj;
		pomeraj /= 2;
		while (!slobodno_mesto(poz_provere))
			poz_provere -= d * pomeraj;
		pomeraj /= 2;
	}

	sf::VertexArray linija_pravca(sf::Lines, 2);
	linija_pravca[0].position=pozicija_stola + k[0].getPosition();
	linija_pravca[1].position=pozicija_stola + poz_provere;
	prozor->draw(linija_pravca);

	sf::CircleShape krug_sudara;
	krug_sudara.setRadius(k[0].getPoluprecnik());
	krug_sudara.setPosition(pozicija_stola + poz_provere - sf::Vector2f(k[0].getPoluprecnik(),k[0].getPoluprecnik()));
	krug_sudara.setFillColor(sf::Color::Transparent);
	krug_sudara.setOutlineThickness(1.f);
	prozor->draw(krug_sudara);
}

void crtaj_sto(sf::RenderWindow* prozor)
{
	//iscrtavanje okvira i podloge stola
	prozor->draw(okvir);
	prozor->draw(sto);
	//iscrtavanje senki
	if (!jednostavno_crtanje)
	{
		for (int i = 0; i < br_kugli; i++)
			k[i].crtaj_senku();//senke kugli
		for (int i = 0; i < br_ivica; i++)
			ivice[i].crtaj_senku();//senke ivica
	}
	//iscrtavanje rupa
	for (int i=0; i < 6; i++)
	{
		prozor->draw(oko_rupa[i]);
		prozor->draw(rupa[i]);
	}
	for (int i = 0; i < 6; i++)
		prozor->draw(u_okvir[i]);
	//iscrtavanje ivica
	for (int i = 0; i < br_ivica; i++)
		ivice[i].crtaj();

	//iscrtavanje kugli
	if (jednostavno_crtanje)
	   for (int i = 1; i < br_kugli; i++)
			k[i].crtaj_jednostavno();
	else
	   for (int i = 1; i < br_kugli; i++)
			k[i].crtaj();
	//crtanje znaka u koliko su oznacene
	for (int i = 1; i < br_kugli; i++)
		k[i].crtaj_precrtano();

	//isctravanje stapa u koliko su se kugle zaustavile
	if (!krecu_se && k[0].aktivna())
	{
		if (!jednostavno_crtanje)
		{
			k[0].crtaj_senku_stapa(mis, (float)tockic, mis_cilja_naopacke);
			k[0].crtaj_stap(mis, (float)tockic, mis_cilja_naopacke);
		}
		else
			k[0].crtaj_stap_jednostavno(mis, (float)tockic, mis_cilja_naopacke);

		if (pomocne_linije)
			crtaj_pomocne_linije(mis, prozor);
	}

	//crtanje bele na kraju
	if (jednostavno_crtanje)
		k[0].crtaj_jednostavno();
	else
		k[0].crtaj();
	k[0].crtaj_precrtano();
}

int main()
{
	sf::RenderWindow prozor(sf::VideoMode(1400, 850), "Bilijar", sf::Style::Close);
	prozor.setFramerateLimit(60);

	memorija_nova=inicijalizuj_memoriju();
	inicijalizuj_tacke();
	inicijalizuj_ivice();
	inicijalizuj_kugle();
	inicijalizuj_grafiku(&prozor);
	update_boja();
	int brojacfrejma = 0;

	while (prozor.isOpen())
	{
		//provera da li je korisnik zatvorio prozor
		sf::Event event;
		while (prozor.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				prozor.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Q || event.key.code == sf::Keyboard::Escape)
					prozor.close();
				if (event.key.code == sf::Keyboard::T)
					jednostavno_crtanje = !jednostavno_crtanje;
				if (event.key.code == sf::Keyboard::L)
					pomocne_linije = !pomocne_linije;
				if (event.key.code == sf::Keyboard::N)
					mis_cilja_naopacke = !mis_cilja_naopacke;
				if (event.key.code == sf::Keyboard::A)
				{
					inicijalizuj_kugle();
					for (int i = 0; i < br_kugli; i++)
						k[i].okreni();
					br_ubacenih_kugli=0;
				}
				if (event.key.code == sf::Keyboard::S)
					for (int i = 0; i < br_kugli; i++)
						k[i].dodeli_brzinu(sf::Vector2f(0.f, 0.f));
				if (event.key.code == sf::Keyboard::P)
					pauzirano = !pauzirano;
				if (event.key.code == sf::Keyboard::O)
					osetljivo = !osetljivo;
				if (event.key.code == sf::Keyboard::R)
					for (int i = 0; i < br_kugli; i++)
						k[i].okreni();
				if (event.key.code == sf::Keyboard::G)
				{
					//zelena kombinacija
					boja_stola = sf::Color(10,100,10);
					boja_stola1 = sf::Color(10,130,10);
					boja_okvira = sf::Color(100,50,10);
					boja_oko_rupa = sf::Color(75,75,75);
					update_boja();
				}
				if (event.key.code == sf::Keyboard::B)
				{
					//plava kombinacija
					boja_stola = sf::Color(30,70,170);
					boja_stola1 = sf::Color(70,100,180);
					boja_okvira = sf::Color(100,50,30);
					boja_oko_rupa = sf::Color(80,100,100);
					update_boja();
				}
				if (event.key.code == sf::Keyboard::U)
				{
					for (int i = 0; i < br_kugli; i++)
						k[i].vrati();
					br_ubacenih_kugli=prethodni_br_ubacenih_kugli;
					k[0].highlight(false);
				}
			}
			if (event.type == sf::Event::MouseWheelScrolled && !krecu_se && k[0].aktivna())
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
				{
					if (osetljivo)
						tockic -= (int)event.mouseWheelScroll.delta;
					else
						tockic -= (int)event.mouseWheelScroll.delta * 5l;
					if (tockic < 0)
						tockic = 0;
					if (tockic > 100)
						tockic = 100;
				}
			if (event.type == sf::Event::MouseMoved && !fiksiran_stap)
				mis = sf::Vector2f((float)event.mouseMove.x, (float)event.mouseMove.y);
			if (event.type == sf::Event::MouseMoved && !krecu_se && !k[0].aktivna())
			{
				mis = sf::Vector2f((float)event.mouseMove.x, (float)event.mouseMove.y);
				k[0].dodeli_poziciju(mis - pozicija_stola);
				k[0].highlight(!slobodno_mesto(mis - pozicija_stola));
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Right && !krecu_se && k[0].aktivna())
				{
					mis = sf::Vector2f((float)event.mouseButton.x, (float)event.mouseButton.y);
					fiksiran_stap = !fiksiran_stap;
				}
				if (event.mouseButton.button == sf::Mouse::Left && !krecu_se && k[0].aktivna())
				{
					k[0].udarac_stapa(mis, (float)tockic, mis_cilja_naopacke);
					krecu_se = 1;
					fiksiran_stap = 0;
					tockic = 40;
					prethodni_br_ubacenih_kugli=br_ubacenih_kugli;
					for (int i = 0; i < br_kugli; i++) k[i].sacuvaj_poziciju();
				}
				if (event.mouseButton.button == sf::Mouse::Right && !krecu_se && !k[0].aktivna())
				{
					mis = sf::Vector2f((float)event.mouseButton.x, (float)event.mouseButton.y);
					k[0].dodeli_poziciju(mis - pozicija_stola);
					k[0].highlight(!slobodno_mesto(mis - pozicija_stola));
					if (slobodno_mesto(mis - pozicija_stola)) k[0].ubaci_u_igru();
				}
				if (event.mouseButton.button == sf::Mouse::Middle)
					osetljivo = !osetljivo;
			}
		}

		if (krecu_se && !pauzirano)
		{
			//pomeranje kugli u koliko se krecu (u koliko se ne krecu nema potrebe da izracunavamo novu brzinu)
			for (int i = 0; i < br_kugli; i++)
				if(k[i].krece_se())
					k[i].osvezi();

			//provera kontakta kugli i realizacija sudara
			for (int i = 0; i < br_kugli - 1; i++)
				for (int j = i + 1; j < br_kugli; j++)
					k[i].sudar_kugli(&k[j]);

			//provera kontakta kugle i zida i realizacija sudara
			for (int i = 0; i < br_kugli; i++)
			{
				bool udar_o_ivicu = 0;
				for (int j = 0; j < br_ivica; j++)
					if(k[i].sudar_o_ivicu(ivice[j]))
						udar_o_ivicu = 1;
				if (!udar_o_ivicu)//u koliko kugla nije udarila u neko od ivica
					for (int l = 0; l < br_tacaka; l++)
						k[i].sudar_o_teme(tacke[l]);//proveri da li je udarila u neko teme
			}

			//deo za ponovno ispitivanje da li je sistem u mirovanju
			krecu_se = 0;
			for (int i = 0; i < br_kugli; i++) if (k[i].krece_se()) krecu_se = 1;

			//razdvajanje kugli ako su slucajno ostale slepljene
			for (int i = 0; i < br_kugli - 1; i++) for (int j = i + 1; j < br_kugli; j++) k[i].razdvoji_kugle(&k[j]);

			for (int i = 0; i < br_kugli; i++)
			{
				//razdvajanje kugli od ivica
				for (int j = 0; j < br_ivica; j++) k[i].razdvoji_kuglu_od_ivice(ivice[j]);
				//razdvajanje kugli od temena
				for (int j = 0; j < br_tacaka; j++) k[i].razdvoji_kuglu_od_temena(tacke[j]);
				//rupa f-ja
				k[i].usla_u_rupu();
			}
		}

		if(!brojacfrejma%10)
		{
			brojacfrejma = 0;
			//brisanje prethodnog frejma
			prozor.clear();

			crtaj_sto(&prozor);

			//prikazivanje frejma
			prozor.display();
		}
	}
	return 0;
}

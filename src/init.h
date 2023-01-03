#ifndef INIT_H
#define INIT_H

#include <SFML/Graphics.hpp>
#include "kugla.h"
#include "matrica.h"

const int br_tacaka = 24;
sf::Vector2f tacke[br_tacaka];
sf::Vector2f pozicija_stola(200.f, 180.f), dimenzije_stola(1000.f, 500.f), pozicija_rupe[6], senka_vektor(5.f, 5.f);

const int br_ivica = 24;
ivica ivice[br_ivica];

const int br_kugli = 16;
kugla k[br_kugli];

void inicijalizuj_tacke()
{
	pozicija_rupe[0] = sf::Vector2f(0.f, 0.f);
	pozicija_rupe[1] = sf::Vector2f(dimenzije_stola.x/2.f, -10.f);
	pozicija_rupe[2] = sf::Vector2f(dimenzije_stola.x, 0.f);
	pozicija_rupe[3] = dimenzije_stola;
	pozicija_rupe[4] = sf::Vector2f(dimenzije_stola.x/2,dimenzije_stola.y + 10.f);
	pozicija_rupe[5] = sf::Vector2f(0.f, dimenzije_stola.y);

	//deklarisanje i posesavanja temena ivica
	tacke[0] = sf::Vector2f(-20.f, 20.f) + pozicija_rupe[0];
	tacke[1] = sf::Vector2f(20.f, -20.f) + pozicija_rupe[0];
	tacke[2] = sf::Vector2f(10.f, 50.f) + pozicija_rupe[0];
	tacke[3] = sf::Vector2f(50.f, 10.f) + pozicija_rupe[0];

	tacke[4] = sf::Vector2f(-25.f, -10.f) + pozicija_rupe[1];
	tacke[5] = sf::Vector2f(25.f, -10.f) + pozicija_rupe[1];
	tacke[6] = sf::Vector2f(-45.f, 20.f) + pozicija_rupe[1];
	tacke[7] = sf::Vector2f(45.f, 20.f) + pozicija_rupe[1];

	tacke[8] = sf::Vector2f(-20.f, -20.f) + pozicija_rupe[2];
	tacke[9] = sf::Vector2f(20.f, 20.f) + pozicija_rupe[2];
	tacke[10] = sf::Vector2f(-50.f, 10.f) + pozicija_rupe[2];
	tacke[11] = sf::Vector2f(-10.f, 50.f) + pozicija_rupe[2];

	tacke[12] = sf::Vector2f(20.f, -20.f) + pozicija_rupe[3];
	tacke[13] = sf::Vector2f(-20.f, 20.f) + pozicija_rupe[3];
	tacke[14] = sf::Vector2f(-10.f, -50.f) + pozicija_rupe[3];
	tacke[15] = sf::Vector2f(-50.f, -10.f) + pozicija_rupe[3];

	tacke[16] = sf::Vector2f(25.f, 10.f) + pozicija_rupe[4];
	tacke[17] = sf::Vector2f(-25.f, 10.f) + pozicija_rupe[4];
	tacke[18] = sf::Vector2f(45.f, -20.f) + pozicija_rupe[4];
	tacke[19] = sf::Vector2f(-45.f, -20.f) + pozicija_rupe[4];

	tacke[20] = sf::Vector2f(20.f, 20.f) + pozicija_rupe[5];
	tacke[21] = sf::Vector2f(-20.f, -20.f) + pozicija_rupe[5];
	tacke[22] = sf::Vector2f(50.f, -10.f) + pozicija_rupe[5];
	tacke[23] = sf::Vector2f(10.f, -50.f) + pozicija_rupe[5];
}

void inicijalizuj_ivice()
{
	//deklarisanje i podesavanja ivica preko temena
	ivice[0].podesi(tacke[1], tacke[3]);
	ivice[1].podesi(tacke[3], tacke[6]);
	ivice[2].podesi(tacke[6], tacke[4]);
	ivice[3].podesi(tacke[5], tacke[7]);
	ivice[4].podesi(tacke[7], tacke[10]);
	ivice[5].podesi(tacke[10], tacke[8]);
	ivice[6].podesi(tacke[9], tacke[11]);
	ivice[7].podesi(tacke[11], tacke[14]);
	ivice[8].podesi(tacke[14], tacke[12]);
	ivice[9].podesi(tacke[13], tacke[15]);
	ivice[10].podesi(tacke[15], tacke[18]);
	ivice[11].podesi(tacke[18], tacke[16]);
	ivice[12].podesi(tacke[17], tacke[19]);
	ivice[13].podesi(tacke[19], tacke[22]);
	ivice[14].podesi(tacke[22], tacke[20]);
	ivice[15].podesi(tacke[21], tacke[23]);
	ivice[16].podesi(tacke[23], tacke[2]);
	ivice[17].podesi(tacke[0], tacke[2]);

	ivice[18].podesi(tacke[1],tacke[4]);
	ivice[19].podesi(tacke[5],tacke[8]);
	ivice[20].podesi(tacke[9],tacke[12]);
	ivice[21].podesi(tacke[13],tacke[16]);
	ivice[22].podesi(tacke[17],tacke[20]);
	ivice[23].podesi(tacke[21],tacke[0]);
}

void inicijalizuj_kugle()
{
	//deklarisanje i podesavanja vrednosti kugli
	for (int i = 0; i < br_kugli; i++)
	{
		k[i].ubaci_u_igru();
		k[i].dodeli_brzinu(sf::Vector2f(0.f,0.f));
		k[i].okreni();
	}
	k[0].dodeli_poziciju(sf::Vector2f(dimenzije_stola.x/4, dimenzije_stola.y/2));

	int br = 1;
	for (int i = 1; i < 6; i++)//i broj reda (npr. 1 je 1.red 2,3 su 2.red ...)
		for (int j = 0; j < i; j++)//j je broj kugle u redu
		{
			k[br].dodeli_poziciju(sf::Vector2f(dimenzije_stola.x*0.65f+i*k[0].getPoluprecnik()*sqrt(3),
						dimenzije_stola.y/2.f+(i-1)*k[0].getPoluprecnik()-j*2.f*k[0].getPoluprecnik()));
			br++;//sledeca kugla
		}
}

#endif

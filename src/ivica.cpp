#include "ivica.h"

void ivica::podesi(sf::Vector2f t1, sf::Vector2f t2)
{
	tacka1 = t1;
	tacka2 = t2;
	pravac = tacka2 - tacka1;
}

float ivica::razdaljina_od(sf::Vector2f A)
{
	float k, n;
	if (pravac.x == 0)
		return fabs(tacka1.x - A.x);
	if (pravac.y == 0)
		return fabs(tacka1.y - A.y);
	k = pravac.y / pravac.x;
	n = tacka1.y - k * tacka1.x;
	return fabs(k*A.x-A.y+n)/sqrt(k*k+1);
}

void ivica::crtaj()
{
	sf::Vertex line[] =
	{
		sf::Vertex(pozicija_stola + tacka1),
		sf::Vertex(pozicija_stola + tacka2)
	};

	prozor->draw(line, 2, sf::Lines);
}

void ivica::crtaj_senku()
{
	sf::VertexArray senka(sf::Quads, 4);
	senka[0].position=pozicija_stola + tacka1;
	senka[1].position=pozicija_stola + tacka2;
	senka[2].position=pozicija_stola + tacka2 + senka_vektor;
	senka[3].position=pozicija_stola + tacka1 + senka_vektor;
	for (int i=0;i<4;i++) senka[i].color=boja_senke;

	prozor->draw(senka);
}

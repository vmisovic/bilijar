#include "kugla.h"

float intenzitet(sf::Vector2f a)//vraca vrednost intenziteta vektora
{
	return sqrt(a.x * a.x + a.y * a.y);
}

float cos_uglaIzmedjuVektora(sf::Vector2f u, sf::Vector2f v)//cos fi=u*v/(|u|*|v|) gde je u*v skalarni proizvod vektora, fi ugao izmedju u i v
{
	if ((u.x * v.x + u.y * v.y) == 0)return 0;
	if((intenzitet(u)==0 || intenzitet(v))==0)return 1;
	return (u.x * v.x + u.y * v.y) * (1 / intenzitet(u) / intenzitet(v));
}

void zameni(float* a, float* b)
{
	float pom = *a;
	*a = *b;
	*b = pom;
}

sf::Vector2f rotiraj(sf::Vector2f a, float alfa)//trenutno ne sluzi nicemu
{
	sf::Vector2f p;
	p.x = a.x * cos(alfa) + a.y * sin(alfa);
	p.y = -a.x * sin(alfa) + a.y * cos(alfa);
	return p;
}

kugla::kugla()
{
	prozor = NULL;//grafika se prosledjuje grugom funkcijom, kako bi mogao da deklarisem u source.cpp-u niz kugli
	boja = sf::Color::Red;
	pozicija = sf::Vector2f(200.f, 200.f);
	rotacija = sf::Vector2f(2.f, 0.f);
	ugaona_brzina = sf::Vector2f(0.f,0.f);
	//podesavanje za iscrtavanja
	krug.setRadius(poluprecnik);
	krug.setFillColor(boja);
}

void kugla::podesi(sf::Color b, sf::Vector2f p, sf::Vector2f v)//dodeljuje pocetne vrednosti boje, pozicije i vektora brzine
{
	boja = b;
	pozicija = p;
	brzina = v;
	//podesavanje za iscrtavanja
	krug.setRadius(poluprecnik);
	krug.setFillColor(boja);
}

void kugla::osvezi()//glupa funkcija pomeranja kugli, treba temeljne izmene
{
	if (pozicija.x < 0 + poluprecnik)
		brzina.x = fabs(brzina.x) * 0.9f;
	if (pozicija.y < 0 + poluprecnik)
		brzina.y = fabs(brzina.y) * 0.9f;
	if (pozicija.x > prozor->getSize().x - poluprecnik)
		brzina.x = -fabs(brzina.x) * 0.9f;
	if (pozicija.y > prozor->getSize().y - poluprecnik)
		brzina.y = -fabs(brzina.y) * 0.9f;
	sf::Vector2f nova_brzina = brzina * (1.f - 9.81f * trenje/120);
	pozicija += (brzina+nova_brzina)/(2.f*120.f);
	brzina = nova_brzina * (0.f + (intenzitet(brzina) > 2.8f));
	ugaona_brzina.x = intenzitet(brzina) / poluprecnik;
	rotacija += ugaona_brzina * (1.f / 120.f);
}

bool kugla::sudar_kugli(kugla* druga)//dodeljuje nove vektore brzine kuglama u koliko je doslo do sudara
{
	sf::Vector2f d = druga->pozicija - this->pozicija;
	if (intenzitet(d) > 2 * poluprecnik || br_frejmova_od_poslednjeg_s < 10)
	{
		br_frejmova_od_poslednjeg_s++;
		return 0;
	}
	float cos_u[2];
	cos_u[0] = cos_uglaIzmedjuVektora(brzina, d);
	cos_u[1] = cos_uglaIzmedjuVektora(druga->brzina, d);
	sf::Vector2f normalna[2], paralelna[2];
	normalna[0] = d * (intenzitet(brzina) * cos_u[0] / intenzitet(d));
	normalna[1] = d * (intenzitet(druga->brzina) * cos_u[1] / intenzitet(d));
	paralelna[0] = brzina - normalna[0];
	paralelna[1] = druga->brzina - normalna[1];
	brzina = paralelna[0] + (normalna[0] * (masa - druga->masa) + normalna[1] * (2 * druga->masa)) / (masa + druga->masa);
	druga->brzina = paralelna[1] + (normalna[0] * (2 * masa) + normalna[1] * (druga->masa - masa)) / (masa + druga->masa);
	br_frejmova_od_poslednjeg_s = 0;
	return 1;
}

bool kugla::sudar_o_ivicu(ivica ivica1)//dodeljuje novi vektor brzine kugli u koliko je doslo do udara o ivicu
{
	if (ivica1.razdaljina_od(pozicija) > poluprecnik ||
		cos_uglaIzmedjuVektora(ivica1.pravac, pozicija - ivica1.tacka1) <= 0 ||
		cos_uglaIzmedjuVektora(pozicija - ivica1.tacka2, -ivica1.pravac) <= 0 ||
		br_frejmova_od_poslednjeg_s < 10)
	{
		br_frejmova_od_poslednjeg_s++;
		return 0;
	}
	sf::Vector2f normalna, paralelna;
	float cos_u1 = cos_uglaIzmedjuVektora(brzina, ivica1.pravac);
	paralelna =  ivica1.pravac * (intenzitet(brzina) * cos_u1 / intenzitet(ivica1.pravac));
	normalna = brzina - paralelna;
	brzina = paralelna - normalna;
	br_frejmova_od_poslednjeg_s = 0;
	return 1;
}

bool kugla::sudar_o_teme(sf::Vector2f tacka)
{
	sf::Vector2f d = pozicija - tacka, normalna, paralelna;
	if (intenzitet(d) > poluprecnik||br_frejmova_od_poslednjeg_s<10)
	{
		br_frejmova_od_poslednjeg_s++;
		return 0;
	}
	float cos_u = cos_uglaIzmedjuVektora(brzina, d);
	normalna = d * (intenzitet(brzina) * cos_u / intenzitet(d));
	paralelna = brzina - normalna;
	brzina = paralelna - normalna;
	br_frejmova_od_poslednjeg_s = 0;
	return 1;
}

bool kugla::krece_se()
{
	return (intenzitet(brzina) > 2.f);
}

void kugla::crtaj()//iscrtavanje
{
	krug.setPosition(pozicija.x - poluprecnik, pozicija.y - poluprecnik);
	if (rotacija.x >= 2.f * 3.14f)
		rotacija.x -= 2.f * 3.14f;
	if (rotacija.x < 0)
		rotacija.x += 2.f * 3.14f;
	
	sf::Vertex line[] =
	{
		sf::Vertex(pozicija),
		sf::Vertex(pozicija + brzina*(sin(rotacija.x)*poluprecnik/intenzitet(brzina)))
	};

	prozor->draw(krug);
	line[0].color = boja;
	if (rotacija.x > 1.57f && rotacija.x < 3.14f * 2 - 1.5f)
		line[1].color = sf::Color::Black;
	prozor->draw(line, 2, sf::Lines);
}

void kugla::crtaj_2()
{
	sf::VertexArray pointmap(sf::Points, 4*poluprecnik*poluprecnik);
	float d, sin_t, sin_s, cos_s, s, t;
	for (int x = -poluprecnik; x < poluprecnik; x++)
		for (int y = -poluprecnik; y < poluprecnik; y++)
		{
			d = sqrt(x * x + y * y);
			if (poluprecnik >= d)
			{
				pointmap[x + poluprecnik + (y + poluprecnik) * poluprecnik * 2].position = sf::Vector2f(x, y) + pozicija;
				sin_t = d / poluprecnik;
				t = asin(sin_t);
				if (d != 0)
				{
					cos_s = x / d;
					sin_s = y / d;
					if (y >= 0)
						s = acos(cos_s);
					else if (x >= 0)
						s = 2.f * 3.14f + asin(sin_s);
					else
						s = 3.14f + acos(cos_s);
				}
				pointmap[x + poluprecnik + (y + poluprecnik) * poluprecnik * 2].color = slika.getPixel(s*(slika.getSize().x-1)/(2.f*3.14f), (sin_t * (slika.getSize().y/2.f - 1)));
			}
		}
	prozor->draw(pointmap);
}

#include "kugla.h"

float intenzitet(sf::Vector2f a)//vraca vrednost intenziteta vektora
{
	return sqrt(a.x * a.x + a.y * a.y);
}

float cos_uglaIzmedjuVektora(sf::Vector2f u, sf::Vector2f v)//cos fi=u*v/(|u|*|v|) gde je u*v skalarni proizvod vektora, fi ugao izmedju u i v
{
	float pom = u.x * v.x + u.y * v.y;
	if (pom == 0)return 0;
	if((intenzitet(u)==0 || intenzitet(v))==0)return 1;
	return pom * (1 / intenzitet(u) / intenzitet(v));
}

sf::Vector2f rotiraj(sf::Vector2f a, float alfa)
{
	sf::Vector2f p;
	p.x = a.x * cos(alfa) + a.y * sin(alfa);
	p.y = -a.x * sin(alfa) + a.y * cos(alfa);
	return p;
}

kugla::kugla()
{
	prozor = NULL;//grafika se prosledjuje grugom funkcijom, kako bi mogao da deklarisem u source.cpp-u niz kugli
	pozicija = sf::Vector2f(200.f, 200.f);
	rotacija = sf::Vector2f(3.14f, 3.14f);//da broj kugle gleda ka ekranu/igracu
	ugaona_brzina = sf::Vector2f(0.f,0.f);
	ugao = 0.f;
	//podesavanje za iscrtavanja
	krug.setRadius(poluprecnik);
	kruzic.setRadius(poluprecnik * 0.7f);
}

void kugla::podesi(sf::Vector2f p, sf::Vector2f v)//dodeljuje pocetne vrednosti boje, pozicije i vektora brzine
{
	pozicija = p;
	brzina = v;
}

void kugla::osvezi()//glupa funkcija pomeranja kugli, treba temeljne izmene
{
	//u koliko je udarila u ivicu prozora/ekrana
	if (pozicija.x < 0 + poluprecnik)
		brzina.x = fabs(brzina.x) * 0.9f;
	if (pozicija.y < 0 + poluprecnik)
		brzina.y = fabs(brzina.y) * 0.9f;
	if (pozicija.x > prozor->getSize().x - poluprecnik)
		brzina.x = -fabs(brzina.x) * 0.9f;
	if (pozicija.y > prozor->getSize().y - poluprecnik)
		brzina.y = -fabs(brzina.y) * 0.9f;
	sf::Vector2f nova_brzina = brzina * (1.f - 9.81f * trenje/60);
	pozicija += (brzina+nova_brzina)/(2.f*120.f);
	brzina = nova_brzina * (0.f + (intenzitet(brzina) > 5.f));
	if (intenzitet(brzina) != 0)
		ugao = acosf(cos_uglaIzmedjuVektora(sf::Vector2f(-10.f, 0.f), brzina));
	ugaona_brzina = brzina / poluprecnik;
	rotacija += ugaona_brzina * (1.f / 120.f);
}

bool kugla::sudar_kugli(kugla* druga)//dodeljuje nove vektore brzine kuglama u koliko je doslo do sudara
{
	sf::Vector2f d = druga->pozicija - this->pozicija;
	if (intenzitet(d) > 2 * poluprecnik)//provera sudara
		return 0;
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
	return 1;
}

bool kugla::sudar_o_ivicu(ivica ivica1)//dodeljuje novi vektor brzine kugli u koliko je doslo do udara o ivicu
{
	if (ivica1.razdaljina_od(pozicija) > poluprecnik ||
		cos_uglaIzmedjuVektora(ivica1.pravac, pozicija - ivica1.tacka1) <= 0 ||
		cos_uglaIzmedjuVektora(pozicija - ivica1.tacka2, -ivica1.pravac) <= 0)//provera sudara
		return 0;
	sf::Vector2f normalna, paralelna;
	float cos_u1 = cos_uglaIzmedjuVektora(brzina, ivica1.pravac);
	paralelna =  ivica1.pravac * (intenzitet(brzina) * cos_u1 / intenzitet(ivica1.pravac));
	normalna = brzina - paralelna;
	brzina = paralelna - normalna;
	return 1;
}

bool kugla::sudar_o_teme(sf::Vector2f tacka)//dodeljuje novi vektor brzine kugli u koliko je doslo do udara o teme
{
	sf::Vector2f d = pozicija - tacka, normalna, paralelna;
	if (intenzitet(d) > poluprecnik)//provera sudara
		return 0;
	float cos_u = cos_uglaIzmedjuVektora(brzina, d);
	normalna = d * (intenzitet(brzina) * cos_u / intenzitet(d));
	paralelna = brzina - normalna;
	brzina = paralelna - normalna;
	return 1;
}

bool kugla::krece_se()//vraca vrednost 1 ako se kugla krece, u suprotnom 0
{
	return (intenzitet(brzina)!=0);
}

void kugla::udarac_stapa(sf::Vector2f poz_mis, float jacina)
{
	sf::Vector2f pravac_stapa = pozicija - poz_mis;
	brzina = (pravac_stapa) / intenzitet(pravac_stapa) * (jacina+20) * 15.f;
}

void kugla::crtaj()//iscrtavanje
{
	//rotacija.x pripada skupu od 0 do 2pi
	if (rotacija.x > 2.f * 3.14f)
		rotacija.x -= 2.f * 3.14f;
	if (rotacija.x < 0)
		rotacija.x += 2.f * 3.14f;
	//rotacija.y pripada skupu od 0 do pi 
	if (rotacija.y > 3.14f)
		rotacija.y -= 3.14f;
	if (rotacija.y < 0)
		rotacija.y += 3.14f;

	//deklarisanje i podesavanje niza piksela (pozicija i boja)
	sf::VertexArray pointmap(sf::Points, (int)(4*poluprecnik*poluprecnik));
	float d, sin_s, sin_t,s,t;
	int kx, ky;
	for (int x = -(int)poluprecnik; x < (int)poluprecnik; x++)
		for (int y = -(int)poluprecnik; y < (int)poluprecnik; y++)
		{
			d = (float)sqrt(x * x + y * y);
			if (poluprecnik >= d)
			{
				int rd_br = (int)(x + poluprecnik + (y + poluprecnik) * poluprecnik * 2.f);
				pointmap[rd_br].position = rotiraj(sf::Vector2f((float)x, (float)y), 0.f) + pozicija;

				sin_s = (float)x / poluprecnik;
				sin_t = (float)y / poluprecnik;
				s = asinf(sin_s) + 3.14f / 2.f + rotacija.x;
				t = asinf(sin_t) + 3.14f / 2.f + rotacija.y;

				kx = (int)((s / 2.f / 3.14f) * slika.getSize().x) % slika.getSize().x;
				ky = (int)((t /3.14f) * slika.getSize().y) % slika.getSize().y;
				pointmap[rd_br].color = slika.getPixel(kx,ky);
			}
		}
	//iscrtavanje na prozor/ekran
	prozor->draw(pointmap);
}

void kugla::crtaj_jednostavno()//jednostavno iscrtavanje
{
	if (rotacija.x >= 2.f * 3.14f)
		rotacija.x -= 2.f * 3.14f;
	if (rotacija.x < 0)
		rotacija.x += 2.f * 3.14f;

	krug.setPosition(pozicija.x - poluprecnik, pozicija.y - poluprecnik);
	prozor->draw(krug);
	kruzic.setPosition(pozicija.x - kruzic.getRadius(), pozicija.y - kruzic.getRadius());
	prozor->draw(kruzic);

	sf::Vertex line[] =
	{
		sf::Vertex(pozicija),
		sf::Vertex(pozicija + brzina * (fsin(rotacija.x) * poluprecnik / intenzitet(brzina)))
	};

	line[0].color = sf::Color::White;
	line[1].color = sf::Color::White;
	if (rotacija.x > 1.57f && rotacija.x < 3.14f * 2 - 1.5f)
	{
		line[0].color = sf::Color::Black;
		line[1].color = sf::Color::Black;
	}
	prozor->draw(line, 2, sf::Lines);
}

void kugla::crtaj_stap(sf::Vector2f poz_mis,float jacina)
{
	sf::Vector2f pravac_stapa = pozicija - poz_mis;
	sf::Vertex line[] =
	{
		sf::Vertex(pozicija - (pravac_stapa) / intenzitet(pravac_stapa) * (jacina/2.f + poluprecnik + 5.f)),
		sf::Vertex(pozicija - (pravac_stapa) / intenzitet(pravac_stapa) * (jacina/2.f + poluprecnik + 200.f))
	};
	prozor->draw(line, 2, sf::Lines);
}


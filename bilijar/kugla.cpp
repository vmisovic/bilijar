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

sf::Vector2f rotiraj90(sf::Vector2f a)//trenutno ne sluzi nicemu
{
	zameni(&a.x, &a.y);
	a.x*= (-1.f);
	return a;
}

kugla::kugla()
{
	prozor = NULL;//grafika se prosledjuje grugom funkcijom, kako bi mogao da deklarisem u source.cpp-u niz kugli
	boja = sf::Color::Red;
	pozicija = sf::Vector2f(200.f, 200.f);
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

void kugla::osvezi(sf::Time vreme)//glupa funkcija pomeranja kugli, treba temeljne izmene
{
	if (pozicija.x < 0 + poluprecnik)
		brzina.x = fabs(brzina.x) * 0.9f;
	if (pozicija.y < 0 + poluprecnik)
		brzina.y = fabs(brzina.y) * 0.9f;
	if (pozicija.x > prozor->getSize().x - poluprecnik)
		brzina.x = -fabs(brzina.x) * 0.9f;
	if (pozicija.y > prozor->getSize().y - poluprecnik)
		brzina.y = -fabs(brzina.y) * 0.9f;
	brzina *= (1 - trenje * vreme.asMilliseconds() * 0.01f) * !(intenzitet(brzina) < 2.8f);
	pozicija += brzina*(vreme.asMilliseconds()*0.01f);
}

bool kugla::sudar_kugli(kugla* druga)//dodeljuje nove vektore brzine kuglama u koliko je doslo do sudara
{
	sf::Vector2f d = druga->pozicija - this->pozicija;
	if (intenzitet(d) > 2 * poluprecnik)
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

bool kugla::sudar_o_ivicu(ivica ivica1)
{
	if (ivica1.razdaljina_od(pozicija) > poluprecnik)//dodeljuje novi vektor brzine kugli u koliko je doslo do udara o ivicu
		return 0;
	sf::Vector2f normalna, paralelna;
	float cos_u1 = cos_uglaIzmedjuVektora(brzina, ivica1.pravac);
	paralelna =  ivica1.pravac * (intenzitet(brzina) * cos_u1 / intenzitet(ivica1.pravac));
	normalna = brzina - paralelna;
	brzina = paralelna - normalna;
	return 1;
}

void kugla::crtaj()//iscrtavanje
{
	krug.setPosition(pozicija.x - poluprecnik, pozicija.y - poluprecnik);
	prozor->draw(krug);
}

/*
	sf::Vector2f d = druga->pozicija - this->pozicija;
	if (intenzitet(d) > 2 * poluprecnik)
		return 0;
	sf::Vector2f pomocna_brzina[3], krajnja_brzina1, krajnja_brzina2;
	float cos_u1;
	pomocna_brzina[0] = brzina - druga->brzina;
	cos_u1 = cos_uglaIzmedjuVektora(pomocna_brzina[0], d);
	pomocna_brzina[2] = d * (intenzitet(pomocna_brzina[0]) / intenzitet(d) * cos_u1);
	krajnja_brzina1 = -pomocna_brzina[2] + brzina;
	krajnja_brzina2 = pomocna_brzina[2] + druga->brzina;
	brzina = krajnja_brzina1;
	druga->brzina = krajnja_brzina2;
*/
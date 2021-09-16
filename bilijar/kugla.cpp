#include "kugla.h"

float intenzitet(sf::Vector2f a)
{
	return sqrt(a.x * a.x + a.y * a.y);
}

float cos_uglaIzmedjuVektora(sf::Vector2f u, sf::Vector2f v)
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

sf::Vector2f rotiraj90(sf::Vector2f a)
{
	zameni(&a.x, &a.y);
	a.x* (-1.f);
	return a;
}

kugla::kugla()
{
	prozor = NULL;
	boja = sf::Color::Red;
	pozicija = sf::Vector2f(200.f, 200.f);

	krug.setRadius(poluprecnik);
	krug.setFillColor(boja);
}

void kugla::podesi(sf::Color b, sf::Vector2f p, sf::Vector2f v)
{
	boja = b;
	pozicija = p;
	brzina = v;

	krug.setRadius(poluprecnik);
	krug.setFillColor(boja);
}

void kugla::osvezi(sf::Time vreme)
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

bool kugla::sudar(kugla* druga)
{
	sf::Vector2f d = druga->pozicija - this->pozicija;
	if (intenzitet(d) > 2 * poluprecnik)
		return 0;
	sf::Vector2f pomocna_brzina0, pomocna_brzina1, pomocna_brzina2, krajnja_brzina1, krajnja_brzina2;
	float cos_u1;
	pomocna_brzina0 = brzina - druga->brzina;
	cos_u1 = cos_uglaIzmedjuVektora(pomocna_brzina0, d);
	pomocna_brzina2 = d * (intenzitet(pomocna_brzina0) / intenzitet(d) * cos_u1);
	krajnja_brzina1 = -pomocna_brzina2 + brzina;
	krajnja_brzina2 = pomocna_brzina2 + druga->brzina;
	brzina = krajnja_brzina1;
	druga->brzina = krajnja_brzina2;
	return 1;
}

void kugla::crtaj()
{
	krug.setPosition(pozicija.x - poluprecnik, pozicija.y - poluprecnik);
	prozor->draw(krug);
}

/*
		pomocna_brzina0 = brzina - druga->brzina;
		cos_u1 = cos_uglaIzmedjuVektora(pomocna_brzina0, d);
		pomocna_brzina2 = d * (intenzitet(pomocna_brzina0) / intenzitet(d) * cos_u1);
		krajnja_brzina1 = -pomocna_brzina2 + brzina;
		krajnja_brzina2 = pomocna_brzina2 + druga->brzina;
		brzina = krajnja_brzina1;
		druga->brzina = krajnja_brzina2;
*/
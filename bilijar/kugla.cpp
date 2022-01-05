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

bool kugla::krece_se()//vraca vrednost 1 ako se kugla krece, u suprotnom 0
{
	return (intenzitet(brzina) != 0 && u_igri);
}

void kugla::udarac_stapa(sf::Vector2f poz_mis, float jacina)
{
	sf::Vector2f pravac_stapa = pozicija_stola + pozicija - poz_mis;
	if (intenzitet(pravac_stapa) == 0)
		pravac_stapa = sf::Vector2f(1.f,1.f);
	brzina = (pravac_stapa) / intenzitet(pravac_stapa) * (jacina+10) * 25.f;
}

void kugla::osvezi()//glupa funkcija pomeranja kugli, treba temeljne izmene
{
	float usporenje=0.9;
	if (u_igri && !animacija)
	{
		//u koliko je udarila u ivicu prozora/ekrana
		if (pozicija_stola.x +  pozicija.x < 0+ poluprecnik)
			brzina.x = fabs(brzina.x) * usporenje;
		if (pozicija_stola.y + pozicija.y < 0 + poluprecnik)
			brzina.y = fabs(brzina.y) * usporenje;
		if (pozicija_stola.x + pozicija.x > prozor->getSize().x - poluprecnik)
			brzina.x = -fabs(brzina.x) * usporenje;
		if (pozicija_stola.y + pozicija.y > prozor->getSize().y - poluprecnik)
			brzina.y = -fabs(brzina.y) * usporenje;
		sf::Vector2f nova_brzina = brzina * (1.f - 9.81f * trenje/60);
		pozicija += (brzina+nova_brzina)/(2.f*120.f);
		brzina = nova_brzina * (0.f + (intenzitet(brzina) > 5.f));
		if (intenzitet(brzina) != 0)
			ugao = acosf(cos_uglaIzmedjuVektora(sf::Vector2f(-10.f, 0.f), brzina));
		ugaona_brzina = brzina / poluprecnik;
		rotacija += ugaona_brzina * (1.f / 120.f);
	}
}

bool kugla::provera_bio_sudar(bool ispunjen_uslov)
{
	if(ispunjen_uslov == 0)
	{
		//ako se kugle ne dodiruju
		bio_sudar = 0;//prestaje sudar
		return 0;//ne izvrsavaj sudar_kugli ponovo
	}
	//kugle se dodiruju
	if (bio_sudar)
		return 0;//ne izvrsavaj sudar_kugli ponovo
	bio_sudar = 1;
	return 1;//izvrsi sudar_kugli
}

bool kugla::provera_sudara_kugli(kugla *druga)
{
	if (u_igri == 0 || druga->u_igri == 0) return 0;
	return provera_bio_sudar(intenzitet(druga->pozicija - this->pozicija) < 2.f * poluprecnik);
}

int greska_poluprecnik=2;
float greska_ugao=0.05;

bool kugla::provera_sudara_o_ivicu(ivica ivica1)
{
	if (u_igri == 0) return 0;
	bool uslov = (bio_sudar ||ivica1.razdaljina_od(pozicija) >= (poluprecnik + greska_poluprecnik) ||
		cos_uglaIzmedjuVektora(ivica1.pravac, pozicija - ivica1.tacka1) <= (0+greska_ugao) ||
		cos_uglaIzmedjuVektora(pozicija - ivica1.tacka2, -ivica1.pravac) <= (0+greska_ugao));
	return provera_bio_sudar(!uslov);
}

bool kugla::provera_sudara_o_teme(sf::Vector2f tacka)
{
	if (u_igri == 0) return 0;
	sf::Vector2f d = pozicija - tacka;
	return provera_bio_sudar(intenzitet(d) <= poluprecnik + greska_poluprecnik);
}

bool kugla::sudar_kugli(kugla* druga)//dodeljuje nove vektore brzine kuglama u koliko je doslo do sudara
{
	if(!provera_sudara_kugli(druga)) return 0;

	sf::Vector2f d = druga->pozicija - this->pozicija;
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
	if(!provera_sudara_o_ivicu(ivica1)) return 0;
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
	if (!provera_sudara_o_teme(tacka))//provera sudara
		return 0;	
	float cos_u = cos_uglaIzmedjuVektora(brzina, d);
	normalna = d * (intenzitet(brzina) * cos_u / intenzitet(d));
	paralelna = brzina - normalna;
	brzina = paralelna - normalna;
	return 1;
}

void kugla::razdvoji_kuglu_od_ivice(ivica ivica1)
{
    // kada sve kugle budu bile na stolu, promeniti ovu funkciju tako da ne stavlja kugle van stola
    if(provera_sudara_o_ivicu(ivica1))
    {
		sf::Vector2f d = dimenzije_stola/2.f - pozicija;
		pozicija += d / intenzitet(d) * 4.f;
    }
}

void kugla::razdvoji_kugle(kugla *druga)
{
	if(provera_sudara_kugli(druga))
    {
		if(pozicija.x>druga->pozicija.x) pozicija=sf::Vector2f(pozicija.x+2,pozicija.y);
		else pozicija=sf::Vector2f(pozicija.x-2,pozicija.y);
		if(pozicija.y>druga->pozicija.y) pozicija=sf::Vector2f(pozicija.x,pozicija.y+2);
		else pozicija=sf::Vector2f(pozicija.x,pozicija.y-2);
    }
}

void kugla::razdvoji_kuglu_od_temena(sf::Vector2f tacka)
{
	if(provera_sudara_o_teme(tacka))
	{
		sf::Vector2f d = pozicija - tacka;
		pozicija+=d/intenzitet(d)*4.f;
	}
}

int kugla::usla_u_rupu()//vraca br. rupe u koju je upala, u suprotnom -1 (i pomera kuglu)
{
	sf::Vector2f d;
	for (int i = 0; i < 6; i++)
	{
		d=pozicija_rupe[i] - pozicija;
		if (intenzitet(d) <= 28.f)
		{
			if (intenzitet(pozicija_rupe[i]-pozicija) <= 5.f)
			{
				dodeli_poziciju(sf::Vector2f(100.f,-50.f));
				u_igri = 0;
				animacija = 0;
				brzina = sf::Vector2f(0.f, 0.f);
				return i;
			}
			pozicija += d * 0.5f;
			animacija = 1;
			return -2;
		}
	}
	return -1;
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
				pointmap[rd_br].position = rotiraj(sf::Vector2f((float)x, (float)y), 0.f) + pozicija + pozicija_stola;

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

void kugla::crtaj_senku()
{
	senka.setPosition(pozicija_stola + pozicija + senka_vektor - sf::Vector2f(poluprecnik, poluprecnik));
	prozor->draw(senka);
}

void kugla::crtaj_jednostavno()//jednostavno iscrtavanje
{
	if (rotacija.x >= 2.f * 3.14f)
		rotacija.x -= 2.f * 3.14f;
	if (rotacija.x < 0)
		rotacija.x += 2.f * 3.14f;

	krug.setPosition(pozicija_stola.x + pozicija.x - poluprecnik, pozicija_stola.y + pozicija.y - poluprecnik);
	prozor->draw(krug);
	kruzic.setPosition(pozicija_stola.x + pozicija.x - kruzic.getRadius(), pozicija_stola.y + pozicija.y - kruzic.getRadius());
	prozor->draw(kruzic);

	sf::Vertex line[] =
	{
		sf::Vertex(pozicija_stola + pozicija),
		sf::Vertex(pozicija_stola + pozicija + brzina * (float)(sin(rotacija.x) * poluprecnik / intenzitet(brzina)))
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

void kugla::crtaj_precrtano()
{
	if (oznacena)
	{
		sf::CircleShape maska;
		maska.setRadius(poluprecnik);
		maska.setFillColor(sf::Color(255,0,0,150));
		maska.setOutlineColor(sf::Color::Red);
		maska.setOutlineThickness(3.f);
		maska.setPosition(pozicija_stola + pozicija - sf::Vector2f(poluprecnik, poluprecnik));
		sf::VertexArray crta(sf::Quads,4); 
		for (int i = 0; i < 4; i++)
			crta[i].color = sf::Color::Red;	
		crta[0].position = pozicija_stola + pozicija + sf::Vector2f(-poluprecnik-2.f,poluprecnik);
		crta[1].position = pozicija_stola + pozicija + sf::Vector2f(-poluprecnik,poluprecnik+2.f);
		crta[2].position = pozicija_stola + pozicija + sf::Vector2f(poluprecnik+2.f,-poluprecnik);
		crta[3].position = pozicija_stola + pozicija + sf::Vector2f(poluprecnik,-poluprecnik-2.f);
	
		prozor->draw(crta);
		prozor->draw(maska);
	}
}

void kugla::crtaj_stap(sf::Vector2f poz_mis,float jacina)
{
	sf::Vector2f pravac_stapa = pozicija_stola + pozicija - poz_mis;
	if (intenzitet(pravac_stapa) == 0)
		pravac_stapa = sf::Vector2f(1.f,1.f);
	sf::Vertex line[] =
	{
		sf::Vertex(pozicija_stola + pozicija - (pravac_stapa) / intenzitet(pravac_stapa) * (jacina/2.f + poluprecnik + 5.f)),
		sf::Vertex(pozicija_stola + pozicija - (pravac_stapa) / intenzitet(pravac_stapa) * (jacina/2.f + poluprecnik + 200.f))
	};
	sf::Vertex line_senka[] =
	{
		sf::Vertex(pozicija_stola + pozicija - (pravac_stapa) / intenzitet(pravac_stapa) * (jacina/2.f + poluprecnik + 5.f) + senka_vektor),
		sf::Vertex(pozicija_stola + pozicija - (pravac_stapa) / intenzitet(pravac_stapa) * (jacina/2.f + poluprecnik + 200.f) + senka_vektor)
	};		
	line_senka[0].color = sf::Color(0,0,0,100);
	line_senka[1].color = sf::Color(0,0,0,100);
	prozor->draw(line_senka, 2, sf::Lines);
	prozor->draw(line, 2, sf::Lines);
}


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

void kugla::udarac_stapa(sf::Vector2f poz_mis, float jacina, bool naopacke)
{
	sf::Vector2f pravac_stapa = pozicija_stola + pozicija - poz_mis;
	if (intenzitet(pravac_stapa) == 0)
		pravac_stapa = sf::Vector2f(1.f,1.f);
	if (naopacke) pravac_stapa *= (-1.f);
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
		ugaona_brzina = brzina / poluprecnik;
		
		gama=0.01f;
		alfa=0.001f;
		beta=0.005f;
		float cosa=cosf(alfa),sina=sinf(alfa);
		float cosb=cosf(beta),sinb=sinf(beta);
		float cosg=cosf(gama),sing=sinf(gama);

        mat_drotacije.mat[0][0]=cosg*cosa-cosb*sina*sing;
        mat_drotacije.mat[0][1]=cosg*sina+cosb*cosa*sing;
        mat_drotacije.mat[0][2]=sing*sinb;
        mat_drotacije.mat[1][0]=-sing*cosa-cosb*sina*cosg;
        mat_drotacije.mat[1][1]=-sing*sina+cosb*cosa*cosg;
        mat_drotacije.mat[1][2]=cosg*sinb;
        mat_drotacije.mat[2][0]=sinb*sina;
        mat_drotacije.mat[2][1]=-sinb*cosa;
        mat_drotacije.mat[2][2]=cosb;

        mat_rotacije=mat_drotacije*mat_rotacije;
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
float greska_ugao=0.05f;

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
	return provera_bio_sudar(intenzitet(d) < poluprecnik + greska_poluprecnik);
}

bool kugla::sudar_kugli(kugla* druga)//dodeljuje nove vektore brzine kuglama u koliko je doslo do sudara
{
	if(!provera_sudara_kugli(druga)) return 0;
	
	sf::Vector2f pv1=brzina/intenzitet(brzina), pv2=druga->brzina/intenzitet(druga->brzina);
	while(provera_sudara_kugli(druga))
	{
		pozicija -= pv1;
		druga->pozicija-=pv2;	
	}

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
	sf::Vector2f pv=brzina/intenzitet(brzina);
	while(provera_sudara_o_ivicu(ivica1))
		pozicija-=pv;
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
	if (!provera_sudara_o_teme(tacka)) return 0;	
	sf::Vector2f pv=brzina/intenzitet(brzina);
	while(provera_sudara_o_teme(tacka))
		pozicija-=pv;
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
		sf::Vector2f d = druga->pozicija-pozicija;
		d *= (poluprecnik*2.f/intenzitet(d)-1.f);
		druga->pozicija += d*0.5f;
		pozicija -= d*0.5f;
    }
}

void kugla::razdvoji_kuglu_od_temena(sf::Vector2f tacka)
{
	if(provera_sudara_o_teme(tacka))
	{
		sf::Vector2f d = tacka - pozicija;
		pozicija += d*(poluprecnik/intenzitet(d)-1.f);
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
				if(red_br!=0) dodeli_poziciju(sf::Vector2f(100.f+(pozicija_nakon_rupe++)*40,-100.f));
                okreni();
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
	//deklarisanje i podesavanje niza piksela (pozicija i boja)
	int kx,ky,rd_br,x,y,r=poluprecnik;
	float s=0,t,d,x1=1,y1=1,z1=1,z;
	for (x = -r; x < r; x++)
		for (y = -r; y < r; y++)
		{
			d = (float)sqrt(x * x + y * y);
			if (poluprecnik >= d)
			{
				rd_br = (int)(x + poluprecnik + (y + poluprecnik) * poluprecnik * 2.f);
				pointmap[rd_br].position = rotiraj(sf::Vector2f((float)x, (float)y), 0.f) + pozicija + pozicija_stola;
				
				z=sqrtf(poluprecnik*poluprecnik-x*x-y*y);

                xyz.mat[0][0]=x;
                xyz.mat[1][0]=y;
                xyz.mat[2][0]=z;

                xyz=mat_rotacije*xyz;

                x1=xyz.mat[0][0];
                y1=xyz.mat[1][0];
                z1=xyz.mat[2][0];

				t=acosf(z1/poluprecnik);

				if(x1>0) s=atanf(y1/x1);
				if(x1<0) s=atanf(y1/x1)+PI;
				if(x1==0) s=PI/2.f;
				s=PI*2.f-s;

				kx = (int)((s / 2.f / PI) * slika.getSize().x) % slika.getSize().x;
				ky = (int)((t /PI) * slika.getSize().y) % slika.getSize().y;
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
	krug.setPosition(pozicija_stola.x + pozicija.x - poluprecnik, pozicija_stola.y + pozicija.y - poluprecnik);
	prozor->draw(krug);
	kruzic.setPosition(pozicija_stola.x + pozicija.x - kruzic.getRadius(), pozicija_stola.y + pozicija.y - kruzic.getRadius());
	prozor->draw(kruzic);
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

void kugla::crtaj_stap(sf::Vector2f poz_mis, float jacina, bool naopacke)
{
	sf::Vector2f pravac_stapa = pozicija_stola + pozicija - poz_mis, normala;
	if (intenzitet(pravac_stapa) == 0) pravac_stapa = sf::Vector2f(1.f,1.f);
	if (naopacke) pravac_stapa *= (-1.f);
	pravac_stapa /= intenzitet(pravac_stapa);
	normala = rotiraj(pravac_stapa, PI / 2.f);
	sf::VertexArray oblik(sf::Quads, 4), trougao(sf::Triangles, 3);
	oblik[0].position=pozicija_stola + pozicija - pravac_stapa * (jacina/2.f + poluprecnik + 5.f) + normala * 2.f;
	oblik[1].position=pozicija_stola + pozicija - pravac_stapa * (jacina/2.f + poluprecnik + 5.f) - normala * 2.f;
	oblik[2].position=pozicija_stola + pozicija - pravac_stapa * (jacina/2.f + poluprecnik + 300.f) - normala * 4.f;
	oblik[3].position=pozicija_stola + pozicija - pravac_stapa * (jacina/2.f + poluprecnik + 300.f) + normala * 4.f;
	trougao[0].position=pozicija_stola + pozicija - pravac_stapa * (jacina/2.f + poluprecnik + 130.f);
	trougao[1].position=pozicija_stola + pozicija - pravac_stapa * (jacina/2.f + poluprecnik + 300.f) - normala * 5.f;
	trougao[2].position=pozicija_stola + pozicija - pravac_stapa * (jacina/2.f + poluprecnik + 300.f) + normala * 5.f;
	for (int i=0;i<2;i++) oblik[i].color=boja_stapa;
	for (int i=2;i<4;i++) oblik[i].color=sf::Color::White;
	for (int i=0;i<3;i++) trougao[i].color=sf::Color::Black;
	prozor->draw(oblik);
	prozor->draw(trougao);
}

void kugla::crtaj_senku_stapa(sf::Vector2f poz_mis, float jacina, bool naopacke)
{
	sf::Vector2f pravac_stapa = pozicija_stola + pozicija - poz_mis, normala;
	if (intenzitet(pravac_stapa) == 0) pravac_stapa = sf::Vector2f(1.f,1.f);
	if (naopacke) pravac_stapa *= (-1.f);
	pravac_stapa /= intenzitet(pravac_stapa);
	normala = rotiraj(pravac_stapa, PI / 2.f);
	sf::VertexArray senka(sf::Quads, 4);
	senka[0].position=pozicija_stola + pozicija + senka_vektor - pravac_stapa * (jacina/2.f + poluprecnik + 5.f) + normala * 2.f;
	senka[1].position=pozicija_stola + pozicija + senka_vektor - pravac_stapa * (jacina/2.f + poluprecnik + 5.f) - normala * 2.f;
	senka[2].position=pozicija_stola + pozicija + senka_vektor * 2.f - pravac_stapa * (jacina/2.f + poluprecnik + 300.f) - normala * 4.f;
	senka[3].position=pozicija_stola + pozicija + senka_vektor * 2.f - pravac_stapa * (jacina/2.f + poluprecnik + 300.f) + normala * 4.f;
	for (int i=0;i<4;i++) senka[i].color=boja_senke;

	prozor->draw(senka);
}

void kugla::crtaj_stap_jednostavno(sf::Vector2f poz_mis, float jacina, bool naopacke)
{
	sf::Vector2f pravac_stapa = pozicija_stola + pozicija - poz_mis;
	if (intenzitet(pravac_stapa) == 0)
		pravac_stapa = sf::Vector2f(1.f,1.f);
	sf::Vertex line[] =
	{
		sf::Vertex(pozicija_stola + pozicija - (pravac_stapa) / intenzitet(pravac_stapa) * (jacina/2.f + poluprecnik + 5.f)),
		sf::Vertex(pozicija_stola + pozicija - (pravac_stapa) / intenzitet(pravac_stapa) * (jacina/2.f + poluprecnik + 200.f))
	};
	prozor->draw(line, 2, sf::Lines);
}

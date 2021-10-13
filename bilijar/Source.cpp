#include <iostream>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include "kugla.h"
using namespace std;
int main()
{
    sf::RenderWindow prozor(sf::VideoMode(1000,600), "SFML & openGL works!");
    prozor.setFramerateLimit(30);
    sf::Vector2f pozicija_stola(100.f, 100.f), pozicija_rupe[6];
    pozicija_rupe[0] = sf::Vector2f(0.f, 0.f) + pozicija_stola;
    pozicija_rupe[1] = sf::Vector2f(400.f, 0.f) + pozicija_stola;
    pozicija_rupe[2] = sf::Vector2f(800.f, 0.f) + pozicija_stola;
    pozicija_rupe[3] = sf::Vector2f(800.f, 400.f) + pozicija_stola;
    pozicija_rupe[4] = sf::Vector2f(400.f, 400.f) + pozicija_stola;
    pozicija_rupe[5] = sf::Vector2f(0.f, 400.f) + pozicija_stola;

    //glEnable(GL_TEXTURE_2D);//da proveris dal radi opengl, necu koristiti opengl tako da je svejedno
    
    //deklarisanje i posesavanja temena ivica
    int br_tacaka = 24;
    sf::Vector2f tacke[24];
    {
        tacke[0] = sf::Vector2f(-20.f, 20.f) + pozicija_rupe[0];
        tacke[1] = sf::Vector2f(20.f, -20.f) + pozicija_rupe[0];
        tacke[2] = sf::Vector2f(0.f, 40.f) + pozicija_rupe[0];
        tacke[3] = sf::Vector2f(40.f, 0.f) + pozicija_rupe[0];

        tacke[4] = sf::Vector2f(-28.f, -20.f) + pozicija_rupe[1];
        tacke[5] = sf::Vector2f(28.f, -20.f) + pozicija_rupe[1];
        tacke[6] = sf::Vector2f(-40.f, 0.f) + pozicija_rupe[1];
        tacke[7] = sf::Vector2f(40.f, 0.f) + pozicija_rupe[1];

        tacke[8] = sf::Vector2f(-20.f, -20.f) + pozicija_rupe[2];
        tacke[9] = sf::Vector2f(20.f, 20.f) + pozicija_rupe[2];
        tacke[11] = sf::Vector2f(0.f, 40.f) + pozicija_rupe[2];
        tacke[10] = sf::Vector2f(-40.f, 0.f) + pozicija_rupe[2];

        tacke[12] = sf::Vector2f(20.f, -20.f) + pozicija_rupe[3];
        tacke[13] = sf::Vector2f(-20.f, 20.f) + pozicija_rupe[3];
        tacke[14] = sf::Vector2f(0.f, -40.f) + pozicija_rupe[3];
        tacke[15] = sf::Vector2f(-40.f, 0.f) + pozicija_rupe[3];

        tacke[16] = sf::Vector2f(28.f, 20.f) + pozicija_rupe[4];
        tacke[17] = sf::Vector2f(-28.f, 20.f) + pozicija_rupe[4];
        tacke[18] = sf::Vector2f(40.f, 0.f) + pozicija_rupe[4];
        tacke[19] = sf::Vector2f(-40.f, 0.f) + pozicija_rupe[4];

        tacke[20] = sf::Vector2f(20.f, 20.f) + pozicija_rupe[5];
        tacke[21] = sf::Vector2f(-20.f, -20.f) + pozicija_rupe[5];
        tacke[22] = sf::Vector2f(40.f, 0.f) + pozicija_rupe[5];
        tacke[23] = sf::Vector2f(0.f, -40.f) + pozicija_rupe[5];
    }

    //deklarisanje i podesavanja ivica preko temena
    int br_ivica = 18;
    ivica ivice[18];
    for (int i = 0; i < br_ivica; i++)
        ivice[i].povezi_grafiku(&prozor);
    {
        ivice[0].podesi(tacke[0], tacke[2]);
        ivice[1].podesi(tacke[1], tacke[3]);
        ivice[2].podesi(tacke[4], tacke[6]);
        ivice[3].podesi(tacke[5], tacke[7]);
        ivice[4].podesi(tacke[8], tacke[10]);
        ivice[5].podesi(tacke[9], tacke[11]);
        ivice[6].podesi(tacke[12], tacke[14]);
        ivice[7].podesi(tacke[13], tacke[15]);
        ivice[8].podesi(tacke[16], tacke[18]);
        ivice[9].podesi(tacke[17], tacke[19]);
        ivice[10].podesi(tacke[20], tacke[22]);
        ivice[11].podesi(tacke[21], tacke[23]);
        ivice[12].podesi(tacke[3], tacke[6]);
        ivice[13].podesi(tacke[7], tacke[10]);
        ivice[14].podesi(tacke[11], tacke[14]);
        ivice[15].podesi(tacke[15], tacke[18]);
        ivice[16].podesi(tacke[19], tacke[22]);
        ivice[17].podesi(tacke[23], tacke[2]);
    }

    //deklarisanje i podesavanja vrednosti kugli
    int br_kugli = 16;
    kugla k[16];
    for (int i = 0; i < br_kugli; i++)
        k[i].povezi_grafiku(&prozor,i);
    for (int i = 2; i < br_kugli; i++)
        k[i].dodeli_poziciju(sf::Vector2f(30.f + 45.f * i, 30.f));
    k[0].podesi(sf::Vector2f(500.f, 300.f), sf::Vector2f(0.f, 0.f));
    k[11].podesi(sf::Vector2f(600.f, 400.f), sf::Vector2f(1000.f, 0.f));
    k[2].podesi(sf::Vector2f(400.f, 320.f), sf::Vector2f(-350.f, -310.f));
    k[3].podesi(sf::Vector2f(500.f, 200.f), sf::Vector2f(500.f, 600.f));
    k[4].podesi(sf::Vector2f(500.f, 400.f), sf::Vector2f(130.f, -400.f));

    sf::RectangleShape pravougaonik;
    pravougaonik.setFillColor(sf::Color::Magenta);
    pravougaonik.setSize(sf::Vector2f(100.f, 100.f));
    bool krecu_se = 1;

    while (prozor.isOpen())
    {
        //provera da li je korisnik zatvorio prozor
        sf::Event event;
        while (prozor.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                prozor.close();
        }

        //brisanje prethodnog frejma
        prozor.clear();

        if (krecu_se)
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
                bool udar_o_teme = 0;
                for (int j = 0; j < br_tacaka || udar_o_teme == 1; j++)
                    udar_o_teme = k[i].sudar_o_teme(tacke[j]) == 1;//u koliko kugla nije udarila u neko od temena
                if (udar_o_teme == 0)
                    for (int l = 0; l < br_ivica; l++)
                        k[i].sudar_o_ivicu(ivice[l]);//proveri da li je udarila u neki od zidova
            }

            //deo za ponovno ispitivanje da li je sistem u mirovanju
            krecu_se = 0;
            for (int i = 0; i < br_kugli; i++)
                if (k[i].krece_se() == 1)
                    krecu_se = 1;
        }

        //iscrtavanje ivica
        for (int i = 0; i < br_ivica; i++)
            ivice[i].crtaj();
        //iscrtavanje kugli
        for (int i = 0; i < br_kugli; i++)
            k[i].crtaj();
        //isctravanje stapa u koliko su se kugle zaustavile
        if (krecu_se==0)
            prozor.draw(pravougaonik);

        //prikazivanje frejma
        prozor.display();
    }
    return 0;
}
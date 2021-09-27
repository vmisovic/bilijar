#include <iostream>
#include <SFML/Graphics.hpp>
#include "kugla.h"

using namespace std;
int main()
{
    sf::RenderWindow prozor(sf::VideoMode(1000,600), "SFML works!");
    prozor.setFramerateLimit(30);

    //deklarisanje i podesavanja vrednosti kugli
    kugla k[16];
    for (int i = 0; i < 16; i++)
        k[i].povezi_grafiku(&prozor);
    k[0].podesi(sf::Color::White, sf::Vector2f(300.f, 500.f), sf::Vector2f(0.f, 0.f));
    k[1].podesi(sf::Color::Green, sf::Vector2f(600.f, 500.f), sf::Vector2f(-500.f, 0.f));
    //k[2].podesi(sf::Color::Yellow, sf::Vector2f(400.f, 350.f), sf::Vector2f(0.f, 0.f));
    //k[3].podesi(sf::Color::Magenta, sf::Vector2f(500.f, 500.f), sf::Vector2f(500.f, 600.f));
    //k[4].podesi(sf::Color::Blue, sf::Vector2f(500.f, 400.f), sf::Vector2f(5000.f, 0.f));
    for (int i = 2; i < 16; i++)
        k[i].dodeli_poziciju(sf::Vector2f(30.f + 45.f * i, 30.f));
    
    //deklarisanje i posesavanja temena ivica
    sf::Vector2f tacke[20];
    tacke[0] = sf::Vector2f(460.f, 460.f);
    tacke[1] = sf::Vector2f(420.f, 500.f);
    tacke[2] = sf::Vector2f(460.f, 540.f);

    //deklarisanje i podesavanja ivica preko temena
    ivica ivice[2];
    for (int i = 0; i < 2; i++)
        ivice[i].povezi_grafiku(&prozor);
    ivice[0].podesi(tacke[0],tacke[1]);
    ivice[1].podesi(tacke[1],tacke[2]);

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

        //pomeranje kugli
        for (int i = 0; i < 16; i++)
            k[i].osvezi();
        
        //provera kontakta kugli i realizacija sudara
        for (int i = 0; i < 15; i++)
            for (int j = i + 1; j < 16; j++)
                k[i].sudar_kugli(&k[j]);

        //provera kontakta kugle i zida i realizacija sudara
        for (int i = 0; i < 16; i++)
            for (int j = 0; j < 3; j++)
                if (k[i].sudar_o_teme(tacke[j]) == 0)//u koliko kugla nije udarila u neko od temena
                    for (int j = 0; j < 2; j++)
                        k[i].sudar_o_ivicu(ivice[j]);//proveri da li je udarila u neki od zidova

        //iscrtavanje kugli
        for (int i = 0; i < 16; i++)
            k[i].crtaj();

        //iscrtavanje ivica
        for (int i = 0; i < 2; i++)
            ivice[i].crtaj();

        //prikazivanje frejma
        prozor.display();
    }
    return 0;
}

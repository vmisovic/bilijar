#include <SFML/Graphics.hpp>
#include<cmath>

class ivica
{
public:
	sf::Vector2f tacka1, tacka2, pravac;
	float k, n;
	sf::RenderWindow* prozor;

	void povezi_grafiku(sf::RenderWindow* prozor1) { this->prozor = prozor1; }
	void podesi(sf::Vector2f t1,sf::Vector2f t2);
	float razdaljina_od(sf::Vector2f A);
	void crtaj();
};


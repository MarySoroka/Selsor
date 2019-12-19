#ifndef BAR_H
#define BAR_H
 


#include <SFML/Graphics.hpp>

using namespace sf;

class Bar
{
public:
	Image image;
	Texture t;
	Sprite s;
	int max;
	RectangleShape bar;
	String name;
	float h, w;
	int shift;
	Bar(String name, float h, float w);
	void update(int k);
	void draw(RenderWindow& window, int shift);

};


#endif  
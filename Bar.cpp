#include "Bar.h"

using namespace sf;

Bar::Bar(String name, float h, float w)
{
	image.loadFromFile(name);
	t.loadFromImage(image);

	s.setTexture(t);
	s.setTextureRect(IntRect(0, 0, w, h));
	s.setScale(0.25f, 0.25f);
	bar.setFillColor(Color(0, 0, 0));
	max = 100;
}

void Bar::updateBar(int k, float type, int shiftBar)
{
	if (shiftBar == 1) {
		k /= 30;
	}
	if ((k >= 0) && (k < max))
	{
		bar.setSize(Vector2f((max - k) * type, 20));

	}
}

void Bar::draw(RenderWindow& window, int shift,int k, float type, float typeOfPower)
{
	Vector2f center = window.getView().getCenter();
	Vector2f size = window.getView().getSize();
	int l = 0;
	if (shift == 80) {
		l = 10;
	}
	if (typeOfPower == 1) {
		k /= 30;
		l = -11;
	}
	
	if ((k >= 0) && (k <= max)) {
			s.setPosition(center.x - size.x / 2 + 10, center.y - size.y / 2 + 10 + shift);
			bar.setPosition(center.x - size.x / 2 + (90 - ((50 - k) * type))-l, center.y - size.y / 2 + 25 + shift);
	}
	window.draw(s);
	window.draw(bar);
}
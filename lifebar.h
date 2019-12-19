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

	Bar(String name,float h, float w)
	{
		image.loadFromFile(name);
		t.loadFromImage(image);
	
		s.setTexture(t);
		s.setTextureRect(IntRect(0, 0, w, h));
		s.setScale(0.25f,0.25f);
		bar.setFillColor(Color(0, 0, 0));//черный прямоугольник накладывается сверху и появляется эффект отсутствия здоровья
		max = 100;
	}

	void update(int k)// k-текущее здоровье
	{
		if (k > 0)
			if (k < max)
				bar.setSize(Vector2f(10, (max - k) * 70 / max));//если не отрицательно и при этом меньше максимума, то устанавливаем новое значение (новый размер) для черного прямоугольника
	}

	void draw(RenderWindow& window, int shift)
	{
		Vector2f center = window.getView().getCenter();
		Vector2f size = window.getView().getSize();
		s.setPosition(center.x - size.x / 2 + 10, center.y - size.y / 2 + 10 + shift);//позиция на экране
		bar.setPosition(center.x - size.x / 2 + 14, center.y - size.y / 2 + 14 );

		window.draw(s);//сначала рисуем полоску здоровья
		window.draw(bar);//поверх неё уже черный прямоугольник, он как бы покрывает её
	}

};
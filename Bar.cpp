#include "Bar.h"

using namespace sf;

Bar::Bar(String name, float h, float w)
{
	image.loadFromFile(name);
	t.loadFromImage(image);

	s.setTexture(t);
	s.setTextureRect(IntRect(0, 0, w, h));
	s.setScale(0.25f, 0.25f);
	bar.setFillColor(Color(0, 0, 0));//������ ������������� ������������� ������ � ���������� ������ ���������� ��������
	max = 100;
}

void Bar::update(int k)// k-������� ��������
{
	if (k > 0)
		if (k < max)
			bar.setSize(Vector2f( (max-k)*1.1,20 ));//���� �� ������������ � ��� ���� ������ ���������, �� ������������� ����� �������� (����� ������) ��� ������� ��������������
	//bar.setPosition();
}

void Bar::draw(RenderWindow& window, int shift,int k )
{
	Vector2f center = window.getView().getCenter();
	Vector2f size = window.getView().getSize();
	if (k > 0) {
		if (k < max) {
			s.setPosition(center.x - size.x / 2 + 10, center.y - size.y / 2 + 10 + shift);//������� �� ������
			bar.setPosition(center.x - size.x / 2 + (90 - ((50 - k) * 1.1)), center.y - size.y / 2 + 25 + shift);
		}
	}
	window.draw(s);//������� ������ ������� ��������
	window.draw(bar);//������ �� ��� ������ �������������, �� ��� �� ��������� �
}
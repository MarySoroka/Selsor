#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;

void setPlayerCoordinateForView(float x, float y) {
	float tempX = x; float tempY = y;

	if (x < 450) tempX = 450;
	//if (y < 240) tempY = 240;//������� �������
	if (y > 900) tempY = 900;//������ �������.��� ����� �����

	view.setCenter(tempX, tempY);
}
#ifndef VIEW_H
#define VIEW_H


#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;

void setPlayerCoordinateForView(float x, float y) {
	float tempX = x; float tempY = y;
	if (x < 1150) tempX = 1150;
	if (x > 1870) tempX = 1870;
	if (y > 1060) tempY = 1060;//нижнюю стороню.для новой карты
	view.setCenter(tempX, tempY);
}
void setPlayerCoordinateForView2(float x, float y) {
	float tempX = x; float tempY = y;
	if (y < 500) tempY = 500;//нижнюю стороню.для новой карты
	if (x < 900) tempX = 900;
	if (x > 2100) tempX = 2100;
	if (y > 1300) tempY = 1300;//нижнюю стороню.для новой карты
	view.setCenter(tempX, tempY);
}
void setPlayerCoordinateForView3(float x, float y) {
	float tempX = x; float tempY = y;
	if (y < 400) tempY = 400;//нижнюю стороню.для новой карты
	if (x < 900) tempX = 900;
	if (x > 2100) tempX = 2100;
	if (y > 1300) tempY = 1300;//нижнюю стороню.для новой карты
	view.setCenter(tempX, tempY);
}
#endif  
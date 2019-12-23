#ifndef ENTITY_H
#define ENTITY_H



#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "view.h"
#include <vector>

using namespace sf;

class Entity {
public:
	std::vector<Object> obj;
	float dx, dy, x, y, speed, moveTimer;
	int w, h, health, shift;
	bool life, isMove, isGravity, isFast;
	Texture texture;
	Sprite sprite;
	String name;
	Entity(Image& image, String Name, float X, float Y, int W, int H) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; isGravity = false; isMove = false; isFast = true;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}

	FloatRect getRect() {
		return FloatRect(x, y, w, h);
	}
	virtual void update(float time, int shift, int level) = 0;
};
#endif
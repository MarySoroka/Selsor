#include "Entity.h"

class MagicPower :public Entity {
public:
	int direction;

	MagicPower(Image& image, String Name, Level& lvl, float X, float Y, int W, int H, int dir) :Entity(image, Name, X, Y, W, H) {
		obj = lvl.GetObjects("solid");
		x = X;
		y = Y;
		direction = dir;
		speed = 0.8;
		w = h = 16;
		life = true;
	}
	void update(float time, int shift, int level)
	{
		if (direction == 0) {
			dx = -speed;
		}
		else {
			dx = speed;
		}
		dy = 0;
		x += dx * time;
		y += dy * time;

		if (x <= 0) x = 1;
		if (y <= 0) y = 1;
		sprite.setPosition(x + w / 2, y + h / 2+25);
	}
};


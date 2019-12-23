#include "Entity.h"

class MovingPlatform : public Entity {
public:
	MovingPlatform(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		sprite.setTextureRect(IntRect(0, 0, W, H));
		dx = 0.08;
	}

	void updateObject(float time, int shift, int level)
	{
		x += dx * time;
		moveTimer += time;
		if (moveTimer > 2000) { dx *= -1; moveTimer = 0; }
		sprite.setPosition(x + w / 2, y + h / 2);
	}
};


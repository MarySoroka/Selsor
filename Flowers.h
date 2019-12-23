#include "Entity.h"

class magicFlowers :public Entity {
public:
	magicFlowers(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		sprite.setTextureRect(IntRect(0, 0, W, H));
		sprite.setPosition(x + (w) / 2, y + (h) / 2);
		sprite.setScale(0.5f, 0.5f);
	}

	void update(float time, int shift, int level)
	{
		if (health <= 0) { life = false; }
	}
};
class powerFlowers :public Entity {
public:
	powerFlowers(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		sprite.setTextureRect(IntRect(0, 0, W, H));
		sprite.setPosition(x + w / 2, y + h / 2 + 20);

	};
	void update(float time, int shift, int level) { if (health <= 0) { life = false; } };
};
class healthFlowers :public Entity {
public:
	healthFlowers(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		sprite.setTextureRect(IntRect(0, 0, W, H));
		sprite.setPosition(x + w / 2, y + h / 2);
		sprite.setScale(0.7f, 0.7f);
	};
	void update(float time, int shift, int level) { if (health <= 0) { life = false; } };
};

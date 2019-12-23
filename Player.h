#include "Entity.h"
class Player :public Entity {
public:
	enum { left, right, up, down, jump, stay } state;
	int playerScore;
	bool isShoot, isMagicPower;
	int power, magic;
	float CurrentFrame = 0;
	Player(Image& image, String Name, Level& lev, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		playerScore = isShoot = isMagicPower = 0; state = stay; obj = lev.GetAllObjects();
		if (name == "Player") {
			sprite.setTextureRect(IntRect(4, 19, w, h));
			sprite.setScale(1.5f, 1.5f);
		}
		health = 50;
		power = 0;
		magic = 50;
	}

	void control(float time) {
		if (Keyboard::isKeyPressed) {
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				if (CurrentFrame > 3) CurrentFrame -= 2;
				sprite.setTextureRect(IntRect(35 * int(CurrentFrame), 20, 40, 60));
				sprite.move(-0.1 * time, 0);
				sprite.setScale(-1.5f, 1.5f);
				state = left; speed = 0.2;
				if (!isFast) {
					speed = 0.05;
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				if (CurrentFrame > 3) CurrentFrame -= 2;
				sprite.setTextureRect(IntRect(35 * int(CurrentFrame), 20, 40, 60));
				sprite.move(-0.1 * time, 0);
				sprite.setScale(1.5f, 1.5f);
				state = right; speed = 0.2;
				if (!isFast) {
					speed = 0.05;
				}
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up)) && (isGravity)) {
				sprite.setScale(-1.5f, 1.5f);
				state = jump; dy = -1; isGravity = false;
				if (!isFast) {
					dy = -0.4;
					speed = 0.05;
				}
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				sprite.setScale(-1.5f, 1.5f);
				state = down;
				if (!isFast) {
					speed = 0.05;
				}
			}


			if (Keyboard::isKeyPressed(Keyboard::Space)) {
				isShoot = true;
			}


		}
	};
	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = 0; i < obj.size(); i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "solid")
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; isGravity = true; }
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx > 0) { x = obj[i].rect.left - w; }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }
					isFast = true;
				}
				if (obj[i].name == "swamp")
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; isGravity = true; }
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx > 0) { x = obj[i].rect.left - w; }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }
					isFast = false;
				}
				if ((obj[i].name == "deathZone") || (obj[i].name == "lake"))
				{
					health = 0;
				}
			}
	}
	void update(float time, int shift, int level)
	{
		control(time);
		switch (state)
		{
		case right:dx = speed; break;
		case left:dx = -speed; break;
		case up: break;
		case down: dx = 0; break;
		case stay: break;
		}
		x += dx * time;
		checkCollisionWithMap(dx, 0);
		y += dy * time;
		checkCollisionWithMap(0, dy);
		sprite.setPosition(x + w / 2, y + h / 2);
		if (health <= 0) { life = false; }
		if (!isMove) { speed = 0; }
		if (life) {
			switch (level) {
			case 1: setPlayerCoordinateForView(x, y); break;
			case 2: setPlayerCoordinateForView2(x, y); break;
			case 3: setPlayerCoordinateForView3(x, y); break;
			case 4: setPlayerCoordinateForView3(x, y); break;
			case 5: setPlayerCoordinateForView3(x, y); break;
			}
		}
		dy = dy + 0.0015 * time;
	}


};

#include "Entity.h"

class Enemy :public Entity {
public:
	float CurrentFrame = 0;
	Enemy(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		obj = lvl.GetObjects("solidEnemy");
		if (name == "easyEnemy") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
			sprite.setScale(-2.5f, 2.5f);
			dx = 0.1;
			health = 100;
		}
		if (name == "middleEnemy") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
			sprite.setScale(-2.5f, 2.5f);
			dx = 0.1;
			health = 100;
		}
		if (name == "hardEnemy") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
			sprite.setScale(-2.0f, 2.0f);
			dx = 0.1;
			health = 100;
		}
		if (name == "boss") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
			sprite.setScale(-2.0f, 2.0f);
			dx = 0.1;
			health = 3000;
		}
	}
	void checkingMap(float time, float Dx, float Dy)
	{
		for (int i = 0; i < obj.size(); i++) {
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "solidEnemy")
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; isGravity = true; }
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx > 0) { x = obj[i].rect.left - w;  dx = -0.1; sprite.scale(-1, 1); }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; sprite.scale(-1, 1); }
				}
			}
		}
	}


	void updateObject(float time, int shift, int level)
	{
		CurrentFrame += 0.005 * time;
		if (name == "easyEnemy") {

			checkingMap(time, dx, 0);

			switch (level) {
			case 1: {
				if (CurrentFrame > 4) {
					CurrentFrame -= 4;
				}
				sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 20 + shift, 64, 60 + shift)); break; }
			case 2: {
				if (CurrentFrame > 3) {
					CurrentFrame -= 3;
				}
				sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 10+shift, 80, 80 + shift)); break; }
			case 3: {
				if (CurrentFrame > 4) {
					CurrentFrame -= 4;
				}
				sprite.setTextureRect(IntRect(60 * int(CurrentFrame), 0+shift, 65, 32 + shift)); break; }
			case 4: {
				if (CurrentFrame > 3) {
					CurrentFrame -= 2;
				}
				sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 20 + shift, 64, 60 + shift)); break; }
			case 5: {
				if (CurrentFrame > 4) {
					CurrentFrame -= 4;
				}
				sprite.setTextureRect(IntRect(60 * int(CurrentFrame), 0 + shift, 65, 32 + shift)); break; }

			}
			sprite.move(-0.1 * time, 0);
			x += dx * time;
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { life = false; }
		}
		if (name == "middleEnemy") {
			checkingMap(time, dx, 0);
			switch (level) {
			case 3: {
				if (CurrentFrame > 4) {
					CurrentFrame -= 3;
				}
				sprite.setTextureRect(IntRect(21 * int(CurrentFrame), 0, 21, 33)); break;
			}
			case 4: {
				if (health <= 50) {
					shift = 80;
				}
				if (CurrentFrame > 3) {
					CurrentFrame -= 2;
				}
				sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 10+ shift, 80, 80+shift)); break; }
			case 5: {
				if (health <= 50) {
					shift = 80;
				}
				if (CurrentFrame > 3) {
					CurrentFrame -= 2;
				}
				sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 10+shift, 80, 80+shift)); break; }
			}
			
			sprite.move(-0.1 * time, 0);
			x += dx * time;
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { life = false; }
		}
		if (name == "hardEnemy") {
			checkingMap(time, dx, 0);
			if (level == 5) {
				if (CurrentFrame > 4) {
					CurrentFrame -= 4;
				}
				sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 20 + shift, 64, 60 + shift));
			}
			else {
				if (health <= 50) {
					shift = 80;
				}
				if (CurrentFrame > 3) {
					CurrentFrame -= 3;
				}
				sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 10 + shift, 80, 80 + shift));
			}
			sprite.move(-0.1 * time, 0);
			x += dx * time;
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { life = false; }
		}
		if (name == "boss") {
			checkingMap(time, dx, 0);
			if (CurrentFrame > 4) {
				CurrentFrame -= 4;
			}
			int width = 0;
			if (health <= 1000) {
				width = 230;
				shift = 1032;
			}
			else {
				width = 160;
			}
			sprite.setTextureRect(IntRect((width * int(CurrentFrame))+shift, 0, width, 160));
			sprite.move(-0.1 * time, 0);
			x += dx * time;
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { 
				life = false;
			}
		}
	}
};

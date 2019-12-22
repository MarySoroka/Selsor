#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "view.h"
#include "mission.h"
#include "iostream"
#include "level.h"
#include <vector>
#include <list>
#include "TinyXML/tinyxml.h"
#include "Bar.h"
#include "menu.h"
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

class Player :public Entity {
public:
	enum { left, right, up, down, jump, stay} state;
	int playerScore;
	bool isShoot, isMagicPower;
	int power, magic;
	float CurrentFrame = 0;
	Player(Image& image, String Name, Level& lev, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		playerScore =isShoot = isMagicPower = 0; state = stay; obj = lev.GetAllObjects();
		if (name == "Player1") {
			sprite.setTextureRect(IntRect(4, 19, w, h));
			sprite.setScale(1.5f,1.5f);
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
				sprite.setScale(-1.5f,1.5f);
				state = left; speed = 0.2;
				if (!isFast) {
					speed = 0.05;
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				if (CurrentFrame > 3) CurrentFrame -= 2; 
				sprite.setTextureRect(IntRect(35* int(CurrentFrame), 20, 40, 60)); 
				sprite.move(-0.1 * time, 0);
				sprite.setScale(1.5f, 1.5f);
				state = right; speed = 0.2;
				if (!isFast) {
					speed = 0.05;
				}
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up)) && (isGravity)) {
				sprite.setScale(-1.5f, 1.5f);
				state = jump; dy = -0.9; isGravity = false;
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
				if ((obj[i].name == "deathZone")|| (obj[i].name == "lake"))
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
		sprite.setPosition(x + w / 2, y + h / 2);
	}
};



class MovingPlatform : public Entity {
public:
	MovingPlatform(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		sprite.setTextureRect(IntRect(0, 0, W, H));
		dx = 0.08;
	}

	void update(float time, int shift, int level)
	{
		x += dx * time;
		moveTimer += time;
		if (moveTimer > 2000) { dx *= -1; moveTimer = 0; }
		sprite.setPosition(x + w / 2, y + h / 2);
	}
};


class Enemy :public Entity {
public:
	float CurrentFrame = 0;
	Enemy(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		obj = lvl.GetObjects("solidEnemy");
		if (name == "easyEnemy") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
			sprite.setScale(-2.0f,2.0f);
			dx = 0.1;
			health = 100;
		}
		if (name == "middleEnemy") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
			sprite.setScale(-2.0f, 2.0f);
			dx = 0.1;
			health = 100;
		}
	}
	void checkCollisionWithMap(float time,float Dx, float Dy)
	{
		for (int i = 0; i < obj.size(); i++){
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "solidEnemy")
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; isGravity = true; }
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx > 0) { x = obj[i].rect.left - w;  dx = -0.1; sprite.scale(-1, 1); }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; sprite.scale(-1,1 ); }
				}
			}
			
		}
	}


	void update(float time, int shift, int level)
	{
		CurrentFrame += 0.005 * time;
		if (name == "easyEnemy"){
			
			checkCollisionWithMap(time,dx, 0);
			
			switch (level) {
			case 1: { 
				if (CurrentFrame > 4) {
					CurrentFrame -= 4;
				}
				sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 20 + shift, 64, 60 + shift)); break; }
			case 2: { 
				if (CurrentFrame > 3) {
					CurrentFrame -= 2;
				}
				sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 0, 80, 80)); break; }
			case 3: { 
				if (CurrentFrame > 4) {
					CurrentFrame -= 4;
				}
				sprite.setTextureRect(IntRect(60 * int(CurrentFrame), 0, 60, 32)); break; }
			case 4: {
				if (CurrentFrame > 4) {
					CurrentFrame -= 4;
				}
				sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 20 + shift, 64, 60 + shift)); break; }
			case 5: {
				if (CurrentFrame > 6) {
					CurrentFrame -= 6;
				}
				sprite.setTextureRect(IntRect(160 * int(CurrentFrame), 0, 160, 150)); break; }

			}
			sprite.move(-0.1 * time, 0);
			x += dx * time;
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { life = false; }
		}
		if (name == "middleEnemy") {
			checkCollisionWithMap(time, dx, 0);
			if (CurrentFrame > 4) {
					CurrentFrame -= 3;
			}
			sprite.setTextureRect(IntRect(21 * int(CurrentFrame), 0, 21, 32)); 
			sprite.move(-0.1 * time, 0);
			x += dx * time;
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { life = false; }
		}
	}
};

class magicFlowers :public Entity {
public:
	magicFlowers(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		sprite.setTextureRect(IntRect(0, 0, W, H)); 
		sprite.setPosition(x + (w) / 2, y + (h)/2);
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
		sprite.setPosition(x + w / 2, y + h / 2+20);

	};
	void update(float time, int shift, int level) { if (health <= 0) { life = false; } };
};
class healthFlowers :public Entity {
public:
	healthFlowers(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		sprite.setTextureRect(IntRect(0, 0, W, H));
		sprite.setPosition(x + w / 2, y + h  / 2);
		sprite.setScale(0.7f, 0.7f);
	};
	void update(float time,int shift, int level) { if (health <= 0) { life = false; } };
};

void nextLevel(Level& lvl, int& level) {
	switch (level)
	{ 
	case 1: 
		lvl.LoadFromFile("map/swampMap.tmx"); 
		break;
	case 2: 
		lvl.LoadFromFile("map/lakeMap.tmx");  
		break;
	case 3: 
		lvl.LoadFromFile("map/homeMap.tmx");  
		break;
	case 4: 
		lvl.LoadFromFile("forestmap.tmx");  
		break;
	case 5: 
		lvl.LoadFromFile("map/finallMap.tmx");  
		break;

	default:
		break;
	}
}

enum stateOfGame {next,reload,ex};

enum stateOfGame startGame(RenderWindow& window, int& numberLevel) {
	
	view.reset(sf::FloatRect(0, 0, 1376, 768));
	Level lvl;
	nextLevel(lvl, numberLevel);
	Image easyEnemyImage;
	Image movePlatformImage;
	Image magicFImage;
	magicFImage.loadFromFile("images/magicF.png");
	Image healthFImage;
	healthFImage.loadFromFile("images/healthF.png");
	Image powerFImage;
	powerFImage.loadFromFile("images/powerF.png");

	Music music;
	music.setVolume(25);

	SoundBuffer winBuffer;
	winBuffer.loadFromFile("sound/goodEnd.ogg");
	Sound win(winBuffer);

	SoundBuffer ghostScreamtBuffer;
	ghostScreamtBuffer.loadFromFile("images/enemies/swampGhost/ghostScream.ogg");
	Sound ghostScream(ghostScreamtBuffer);
	ghostScream.setVolume(15);


	SoundBuffer ghostDeathBuffer;
	ghostDeathBuffer.loadFromFile("images/enemies/swampGhost/ghostDeath.ogg");
	Sound ghostDeath(ghostDeathBuffer);
	ghostDeath.setVolume(15);

	SoundBuffer dogScreamtBuffer;
	dogScreamtBuffer.loadFromFile("images/enemies/swampGhost/ghostScream.ogg");
	Sound dogScream(dogScreamtBuffer);
	dogScream.setVolume(15);

	SoundBuffer dogDeathBuffer;
	dogDeathBuffer.loadFromFile("images/enemies/swampGhost/ghostDeath.ogg");
	Sound dogDeath(dogDeathBuffer);
	dogDeath.setVolume(15);


	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/power.ogg");
	Sound shoot(shootBuffer);
	shoot.setVolume(15);

	SoundBuffer jumpBuffer;
	jumpBuffer.loadFromFile("sound/jump.ogg");
	Sound jump(jumpBuffer);
	jump.setVolume(15);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.ogg");
	Sound death(deathBuffer);
	death.setVolume(15);

	SoundBuffer flowerBuffer;
	flowerBuffer.loadFromFile("sound/flowerGathering.ogg");
	Sound flower(flowerBuffer);
	flower.setVolume(15);

	Image  MagicPowerImage;
	MagicPowerImage.loadFromFile("images/MagicEffect2.png");
	String nameOfBar;
	if (numberLevel == 5) {
		nameOfBar = "images/DemonHealthBar/1.png";
	}
	else { 
		nameOfBar = "images/FlowerBar/7.png"; 
	}
	Bar lifeBarPlayer("images/HealthBar/1.png", 202, 667);
	Bar magicBarPlayer("images/MagicBar/1.png", 202, 667);
	Bar powerBarPlayer(nameOfBar, 202, 667);
	int wEnemy, Henemy;
	std::list<Entity*>  entities;
	std::list<Entity*>::iterator it;
	std::list<Entity*>::iterator it2;
	std::vector<Object> e = lvl.GetObjects("easyEnemy");

	switch (numberLevel)
	{
	   case 1: {
			easyEnemyImage.loadFromFile("images/enemies/swampGhost/ghost.png");
			movePlatformImage.loadFromFile("images/MovingPlatform.png");
			music.openFromFile("sound/swamp.ogg");
			music.play(); 
			Henemy = 80;
			wEnemy = 40;
			break;
    	}
	   case 2:
	   {
		   easyEnemyImage.loadFromFile("images/wizard.png");
		   movePlatformImage.loadFromFile("images/MovingPlatform2.png");
		   music.openFromFile("sound/lake.ogg");
		   music.play();
		   Henemy = 80;
		   wEnemy = 90;
		   break;
	    }
	   case 3: {
		   easyEnemyImage.loadFromFile("images/enemies/dog/dog.png");
		   movePlatformImage.loadFromFile("images/MovingPlatform3.png");
		   Henemy = 20;
		   wEnemy = 40;
		   music.openFromFile("sound/home.ogg");
		   music.play();
		   break;
	   }
	   case 4: {
		   easyEnemyImage.loadFromFile("images/enemies/swampGhost/ghost.png");
		   movePlatformImage.loadFromFile("images/MovingPlatform.png");
		   Henemy = 20;
		   wEnemy = 40;
		   music.openFromFile("sound/forest.ogg");
		   music.play();
		   break;
	   }
	   case 5: {
		   easyEnemyImage.loadFromFile("images/demon-attack/demon.png");
		   movePlatformImage.loadFromFile("images/MovingPlatform3.png");
		   Henemy = 140;
		   wEnemy = 150;
		   music.openFromFile("sound/cemetery.ogg");
		   music.play();
		   break;
	   }
	default:
		break;
	}
	

	Image heroImage;
	heroImage.loadFromFile("images/girlCharacter.png");
	Object player = lvl.GetObject("player");
	music.setLoop(true);


	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 60,60);
	

	for (int i = 0; i < e.size(); i++) {
		entities.push_back(new Enemy(easyEnemyImage, "easyEnemy", lvl, e[i].rect.left, e[i].rect.top, wEnemy, Henemy));
		e[i].rect.left;
		e[i].rect.top;
	}

	if (numberLevel == 3) {
		Image middleEnemyImage;
		middleEnemyImage.loadFromFile("images/enemies/skeleton/skelet.png");
		e = lvl.GetObjects("middleEnemy");
		for (int i = 0; i < e.size(); i++) {
			entities.push_back(new Enemy(middleEnemyImage, "middleEnemy", lvl, e[i].rect.left, e[i].rect.top, 40, 32));
			e[i].rect.left;
			e[i].rect.top;
		}
		
	}
	e = lvl.GetObjects("movingPlatform");
	for (int i = 0; i < e.size(); i++)
		entities.push_back(new MovingPlatform(movePlatformImage, "movingPlatform", lvl, e[i].rect.left, e[i].rect.top, 95, 22));

	e = lvl.GetObjects("magicF");
	for (int i = 0; i < e.size(); i++)
		entities.push_back(new magicFlowers(magicFImage, "magicF", lvl, e[i].rect.left, e[i].rect.top, 100, 100));

	e = lvl.GetObjects("healthF");
	for (int i = 0; i < e.size(); i++)
		entities.push_back(new healthFlowers(healthFImage, "healthF", lvl, e[i].rect.left, e[i].rect.top, 75, 101));
	e = lvl.GetObjects("powerF");
	for (int i = 0; i < e.size(); i++)
		entities.push_back(new powerFlowers(powerFImage, "powerF", lvl, e[i].rect.left, e[i].rect.top, 31, 50 ));

	int shift = 0;
	Clock clock;
	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();

		clock.restart();
		time = time / 800;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
				
			}
			if (p.magic != 0)
			{

				if (p.isShoot == true) {
					p.isShoot = false;
					entities.push_back(new  MagicPower(MagicPowerImage, "MagicPower", lvl, p.x, p.y, 100, 16, p.state));
					shoot.play();
					p.magic -= 10;
					p.isMagicPower = true;
				}
			}

		}

		for (it = entities.begin(); it != entities.end(); it++)
		{
			if (((*it)->name == "movingPlatform") && ((*it)->getRect().intersects(p.getRect())))
			{
				Entity* movPlat = *it;
				if ((p.dy > 0) || (p.isGravity == false))
					if (p.y + p.h < movPlat->y + movPlat->h)
					{
						p.y = movPlat->y - p.h + 3; p.x += movPlat->dx * time; p.dy = 0; p.isGravity = true;
					}

			}
			if (((*it)->getRect().intersects(p.getRect())) && ((*it)->name == "easyEnemy"))
			{
				if ((p.dy > 0) && (p.isGravity == false)) {
					p.dy = -0.2;
					(*it)->health -= 50;
					ghostDeath.play();
					(*it)->shift = 120;
				    (*it)->update(time,shift,numberLevel);
					
				}
				else {
					ghostScream.play();
					(*it)->shift = 60;
					p.health -= 10;	
				}
				if ((*it)->dx > 0)
				{
					(*it)->x = p.x - (*it)->w;
					(*it)->dx *= -1;
					(*it)->sprite.scale(-1, 1);
				}
				if ((*it)->dx < 0)
				{
					(*it)->x = p.x + p.w; 
					(*it)->dx *= -1;
					(*it)->sprite.scale(-1, 1);
				}
				if (p.dx < 0) {
					p.x = (*it)->x + (*it)->w;
				}
				if (p.dx > 0) {
					p.x = (*it)->x - p.w;
				}

			}
			if (((*it)->getRect().intersects(p.getRect())) && ((*it)->name == "middleEnemy"))
			{
				if ((p.dy > 0) && (p.isGravity == false)) {
					p.dy = -0.2;
					(*it)->health -= 20;
					ghostDeath.play();
					(*it)->shift = 120;
					(*it)->update(time, shift, numberLevel);

				}
				else {
					ghostScream.play();
					(*it)->shift = 60;
					p.health -= 20;
				}
				if ((*it)->dx > 0)
				{
					(*it)->x = p.x - (*it)->w;
					(*it)->dx *= -1;
					(*it)->sprite.scale(-1, 1);
				}
				if ((*it)->dx < 0)
				{
					(*it)->x = p.x + p.w; 
					(*it)->dx *= -1;
					(*it)->sprite.scale(-1, 1);
				}
				if (p.dx < 0) {
					p.x = (*it)->x + (*it)->w;
				}
				if (p.dx > 0) {
					p.x = (*it)->x - p.w;
				}

			}
			
			if ((*it)->getRect().intersects(p.getRect()))
			{
				if ((*it)->name == "EasyEnemy") {
					if ((*it)->dx > 0)
					{
						(*it)->x = p.x - (*it)->w; 
						(*it)->dx = 0;
					}
					if ((*it)->dx < 0)
					{
						(*it)->x = p.x + p.w; 
						(*it)->dx = 0;
					}
				}
			}

			for (it2 = entities.begin(); it2 != entities.end(); it2++)
			{
				if ((*it)->getRect() != (*it2)->getRect())
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "easyEnemy") && ((*it2)->name == "MagicPower"))
					{
						
							
							(*it)->health -= 20;
							ghostDeath.play();
							(*it)->shift = 120;
							(*it)->update(time, shift, numberLevel);

					}

			}
				
			
			if (((*it)->getRect().intersects(p.getRect())) && ((*it)->name == "magicF"))
			{
				p.magic += 10;
				(*it)->life = false;
				flower.play();
			}
			if (((*it)->getRect().intersects(p.getRect())) && ((*it)->name == "healthF"))
			{
				p.health += 10;
				(*it)->life = false;
				flower.play();
			}
			if (((*it)->getRect().intersects(p.getRect())) && ((*it)->name == "powerF"))
			{
				p.power += 20;
				(*it)->life = false;
				flower.play();
			}
			for (it2 = entities.begin(); it2 != entities.end(); it2++)
			{
				if ((*it)->getRect() != (*it2)->getRect())
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "easyEnemy") && ((*it2)->name == "easyEnemy"))
					{
						(*it)->dx *= -1;
						(*it)->sprite.scale(-1, 1);
					}
					
			}
			if (p.health == 0) { death.play(); }
			if (Keyboard::isKeyPressed(Keyboard::Up)) { jump.play(); }
			if (p.power == 100) { win.play(); }

		}
		lifeBarPlayer.update(p.health,1.1,0);
		magicBarPlayer.update(p.magic,1.1,0);
		if (numberLevel == 5) {
			powerBarPlayer.update(100, 1.1, 0);
		}
		else {
			powerBarPlayer.update(p.power, 0.92, 0);
		}
		
		if ((Keyboard::isKeyPressed(Keyboard::T)) || (p.power == 100)) {
			win.play();
			lvl.levelNumber++; 
			return next; }
		if ((Keyboard::isKeyPressed(Keyboard::Tab)) || (p.health == 0)) { death.play(); return reload; }
		if (Keyboard::isKeyPressed(Keyboard::Escape)) { return ex; }

		p.CurrentFrame += 0.005 * time;
		p.update(time,0,numberLevel);

		

		for (it = entities.begin(); it != entities.end();)
		{
			Entity* b = *it;
			if (b->name == "easyEnemy") {
				b->update(time, b->shift,numberLevel);
			}
			else {
				b->update(time, 0,numberLevel);
			}

			if (b->life == false) { it = entities.erase(it); delete b; }
			else it++;
		}



		window.setView(view);
		window.clear(Color(77, 83, 140));
		lvl.Draw(window);


		for (it = entities.begin(); it != entities.end(); it++) {
			window.draw((*it)->sprite);
		}
		lifeBarPlayer.draw(window, 0,p.health+10,1.1);
		magicBarPlayer.draw(window, 40, p.magic+10,1.1);
		powerBarPlayer.draw(window, 80, p.power,0.92);
		window.draw(p.sprite);
		window.display();

	}
}
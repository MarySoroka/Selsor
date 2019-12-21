#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "view.h"
#include <iostream>
#include "mission.h"
#include "iostream"
#include "level.h"
#include <vector>
#include <list>
#include "TinyXML/tinyxml.h"
#include "Bar.h"
#include "menu.h"
using namespace sf;
////////////////////////////////////����� �����-��������//////////////////////////
class Entity {
public:
	std::vector<Object> obj;//������ �������� �����
	float dx, dy, x, y, speed, moveTimer;
	int w, h, health, shift;
	bool life, isMove, onGround, isFast;
	Texture texture;
	Sprite sprite;
	String name;
	Entity(Image& image, String Name, float X, float Y, int W, int H) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false; isFast = true;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}

	FloatRect getRect() {//�-��� ��������� ��������������. ��� �����,������� (���,�����).
		return FloatRect(x, y, w, h);//��� �-��� ����� ��� �������� ������������ 
	}
	virtual void update(float time, int shift, int level) = 0;
};
////////////////////////////////////////////////////����� ������////////////////////////
class Player :public Entity {
public:
	enum { left, right, up, down, jump, stay} state;
	int playerScore;
	bool isShoot, isMagicPower;
	int power, magic;
	float CurrentFrame = 0;
	Player(Image& image, String Name, Level& lev, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		playerScore =isShoot = isMagicPower = 0; state = stay; obj = lev.GetAllObjects();//��������������.�������� ��� ������� ��� �������������� ��������� � ������
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
				if (CurrentFrame > 3) CurrentFrame -= 2; //проходимся по кадрам с первого по третий включительно. если пришли к третьему кадру - откидываемся назад.
				sprite.setTextureRect(IntRect(35 * int(CurrentFrame), 20, 40, 60)); //проходимся по координатам Х. получается 96,96*2,96*3 и опять 96
				sprite.move(-0.1 * time, 0);
				sprite.setScale(-1.5f,1.5f);
				state = left; speed = 0.2;
				if (!isFast) {
					speed = 0.05;
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				if (CurrentFrame > 3) CurrentFrame -= 2; //проходимся по кадрам с первого по третий включительно. если пришли к третьему кадру - откидываемся назад.
				sprite.setTextureRect(IntRect(35* int(CurrentFrame), 20, 40, 60)); //проходимся по координатам Х. получается 96,96*2,96*3 и опять 96
				sprite.move(-0.1 * time, 0);
				sprite.setScale(1.5f, 1.5f);
				state = right; speed = 0.2;
				if (!isFast) {
					speed = 0.05;
				}
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround)) {
				sprite.setScale(-1.5f, 1.5f);
				state = jump; dy = -0.9; onGround = false;
				if (!isFast) {
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

		for (int i = 0; i < obj.size(); i++)//���������� �� ��������
			if (getRect().intersects(obj[i].rect))//��������� ����������� ������ � ��������
			{
				if (obj[i].name == "solid")//���� ��������� �����������
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx > 0) { x = obj[i].rect.left - w; }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }
					isFast = true;
				}
				if (obj[i].name == "swamp")//���� ��������� �����������
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx > 0) { x = obj[i].rect.left - w; }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }
					isFast = false;
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
			}
		}
		dy = dy + 0.0015 * time;
	}


};

class MagicPower :public Entity {//����� ����
public:
	int direction;//����������� ����

	MagicPower(Image& image, String Name, Level& lvl, float X, float Y, int W, int H, int dir) :Entity(image, Name, X, Y, W, H) {//�� ��� ��, ������ ����� � ����� ��������� ������ (int dir)
		obj = lvl.GetObjects("solid");//�������������� .�������� ������ ������� ��� �������������� ���� � ������
		x = X;
		y = Y;
		direction = dir;
		speed = 0.8;
		w = h = 16;
		life = true;
		//���� ������������� � ������������
	}


	void update(float time, int shift, int level)
	{
		switch (direction)
		{
		case 0: dx = -speed; dy = 0;   break;//������� �������� state = left
		case 1: dx = speed; dy = 0;   break;//������� �������� state = right
		case 2: dy = 0; dx = -speed;   break;//������� �������� state = up
		case 3: dy = 0; dx = -speed;   break;//������� �������� �� ������� ��������� � �����������, ���� ������ ��������� �����, ��� ������ ��� �� �����
		case 4: dy = 0; dx = -speed;   break;//������� �������� �� ������� ��������� � �����������, ���� ������ ��������� �����, ��� ������ ��� �� �����
		case 5: dy = 0; dx = -speed;   break;//������� �������� �� ������� ��������� � �����������, ���� ������ ��������� �����, ��� ������ ��� �� �����

		}

		x += dx * time;//���� �������� ���� �� �
		y += dy * time;//�� �

		if (x <= 0) x = 1;// �������� ���� � ����� �����, ����� ��� ���������� ������ ��� �������� �� �������� �� ������ ����� � �� ���� ������
		if (y <= 0) y = 1;
		sprite.setPosition(x + w / 2, y + h / 2);//�������� ������� ����
	}
};



class MovingPlatform : public Entity {//����� ���������� ���������
public:
	MovingPlatform(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		sprite.setTextureRect(IntRect(0, 0, W, H));//������������� 
		dx = 0.08;//����������� ��������� �� �
	}

	void update(float time, int shift, int level)//������� ���������� ���������.
	{
		x += dx * time;//���������� �������� �� �����������
		moveTimer += time;//���������� ������
		if (moveTimer > 2000) { dx *= -1; moveTimer = 0; }//���� ������ �������� 2 ���, �� �������� ����������� �������� ���������,� ������ ����������
		sprite.setPosition(x + w / 2, y + h / 2);//������ ������� �������
	}
};


class Enemy :public Entity {
public:
	float CurrentFrame = 0;
	Enemy(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		obj = lvl.GetObjects("solid");//��������������.�������� ������ ������� ��� �������������� ����� � ������
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
				if (obj[i].name == "solid")//���� ��������� �����������
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
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

class magicFlowers :public Entity {//класс цветов
public:
	magicFlowers(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		sprite.setTextureRect(IntRect(0, 0, W, H));//������������� 
		sprite.setPosition(x + (w) / 2, y + (h)/2);//�������� ������� ����
		sprite.setScale(0.5f, 0.5f);
	}

	void update(float time, int shift, int level)//������� ���������� ���������.
	{
		if (health <= 0) { life = false; }
	}
};
class powerFlowers :public Entity {//класс цветов
public:
	powerFlowers(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		sprite.setTextureRect(IntRect(0, 0, W, H));//�������������
		sprite.setPosition(x + w / 2, y + h / 2+20);//�������� ������� ���

	};
	void update(float time, int shift, int level) { if (health <= 0) { life = false; } };
};
class healthFlowers :public Entity {//класс цветов
public:
	healthFlowers(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		sprite.setTextureRect(IntRect(0, 0, W, H));//������������� 
		sprite.setPosition(x + w / 2, y + h  / 2);//�������� ������� ����
		
		sprite.setScale(0.7f, 0.7f);
	};
	void update(float time,int shift, int level) { if (health <= 0) { life = false; } };
};

void changeLevel(Level& lvl, int& numberLevel) {
	if (numberLevel == 1) { lvl.LoadFromFile("map/swampMap.tmx"); }
	if (numberLevel == 2) { lvl.LoadFromFile("map/lakeMap.tmx"); }
	if (numberLevel == 3) { lvl.LoadFromFile("map/homeMap.tmx"); }
}

void showWin() {
	SoundBuffer winBuffer;
	winBuffer.loadFromFile("sound/goodEnd.ogg");
	Sound win(winBuffer);
	win.play();

}

bool startGame(RenderWindow& window, int& numberLevel) {

	view.reset(sf::FloatRect(0, 0, 1376, 768));
	Level lvl;
	changeLevel(lvl, numberLevel);//для загрузки карты для нужного уровня
	Image easyEnemyImage;
	Image movePlatformImage;
	Image magicFImage;
	magicFImage.loadFromFile("images/magicF.png");
	Image healthFImage;
	healthFImage.loadFromFile("images/healthF.png");
	healthFImage.createMaskFromColor(Color(255, 255, 255));
	Image powerFImage;
	powerFImage.loadFromFile("images/powerF.png");

	Music music;//������� ������ ������

	Music winMusic;
	Image  MagicPowerImage;//����������� ��� ����
	MagicPowerImage.loadFromFile("images/MagicEffect2.png");//��������� �������� � ������ �����������

	Bar lifeBarPlayer("images/HealthBar/1.png", 202, 667);//��������� ������ ������� ��������
	Bar magicBarPlayer("images/MagicBar/1.png", 202, 667);//��������� ������ ������� ��������
	Bar powerBarPlayer("images/FlowerBar/7.png", 202, 667);//��������� ������ ������� ��������
	Sound shoot;
	Sound ghostScream;
	Sound ghostDeath;

	
	

	int wEnemy, Henemy;
	std::list<Entity*>  entities;
	std::list<Entity*>::iterator it;
	std::list<Entity*>::iterator it2;
	std::vector<Object> e = lvl.GetObjects("easyEnemy");

	switch (numberLevel)
	{
	   case 1: {
		
			easyEnemyImage.loadFromFile("images/enemies/swampGhost/ghost.png");
			easyEnemyImage.createMaskFromColor(Color(255, 0, 0));

		
			movePlatformImage.loadFromFile("images/MovingPlatform.png");

		
			SoundBuffer shootBuffer;
			shootBuffer.loadFromFile("sound/shoot.ogg");
			Sound shoot(shootBuffer);


			SoundBuffer ghostScreamtBuffer;
			ghostScreamtBuffer.loadFromFile("images/enemies/swampGhost/ghostScream.ogg");
			Sound ghostScream(ghostScreamtBuffer);


			SoundBuffer ghostDeathBuffer;
			ghostDeathBuffer.loadFromFile("images/enemies/swampGhost/ghostDeath.ogg");
			Sound ghostDeath(ghostDeathBuffer);


			music.openFromFile("sound/swamp.ogg");
			music.play();
			music.setVolume(15); 
			Henemy = 80;
			wEnemy = 40;
			break;
    	}
	   case 2:
	   {
		   easyEnemyImage.loadFromFile("images/wizard.png");
		   easyEnemyImage.createMaskFromColor(Color(255, 0, 0));


		   movePlatformImage.loadFromFile("images/MovingPlatform.png");


		   SoundBuffer shootBuffer;
		   shootBuffer.loadFromFile("sound/shoot.ogg");
		   Sound shoot(shootBuffer);


		   SoundBuffer ghostScreamtBuffer;
		   ghostScreamtBuffer.loadFromFile("images/enemies/skeleton/attackSkel.ogg");
		   Sound ghostScream(ghostScreamtBuffer);


		   SoundBuffer ghostDeathBuffer;
		   ghostDeathBuffer.loadFromFile("images/enemies/skeleton/skelletonDeath.ogg");
		   Sound ghostDeath(ghostDeathBuffer);

		   

		   music.openFromFile("sound/lake.ogg");
		   music.play();
		   music.setVolume(15);
		   Henemy = 80;
		   wEnemy = 90;
		   break;
	    }
	   case 3: {
		   easyEnemyImage.loadFromFile("images/enemies/dog/dog.png");
		   easyEnemyImage.createMaskFromColor(Color(255, 0, 0));


		   movePlatformImage.loadFromFile("images/MovingPlatform.png");


		   SoundBuffer shootBuffer;
		   shootBuffer.loadFromFile("sound/shoot.ogg");
		   Sound shoot(shootBuffer);


		   SoundBuffer ghostScreamtBuffer;
		   ghostScreamtBuffer.loadFromFile("images/enemies/dog/dogGrowl.ogg");
		   Sound ghostScream(ghostScreamtBuffer);


		   SoundBuffer ghostDeathBuffer;
		   ghostDeathBuffer.loadFromFile("images/enemies/dog/dogDeath.ogg");
		   Sound ghostDeath(ghostDeathBuffer);


		   Henemy = 20;
		   wEnemy = 40;
		   music.openFromFile("sound/home.ogg");
		   music.play();
		   music.setVolume(15);

		  
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
	

	for (int i = 0; i < e.size(); i++) {//���������� �� ��������� ����� �������(� ������ �� ������)
		entities.push_back(new Enemy(easyEnemyImage, "easyEnemy", lvl, e[i].rect.left, e[i].rect.top, wEnemy, Henemy));//� ���������� � ������ ���� ����� ������ � �����
		e[i].rect.left;//����� �
		e[i].rect.top;//����� Y
	}

	if (numberLevel == 3) {
		Image middleEnemyImage;
		middleEnemyImage.loadFromFile("images/enemies/skeleton/skelet.png");
		e = lvl.GetObjects("middleEnemy");//�������� ��� ��������� � ������ 
		for (int i = 0; i < e.size(); i++) {//���������� �� ��������� ����� �������(� ������ �� ������)
			entities.push_back(new Enemy(middleEnemyImage, "middleEnemy", lvl, e[i].rect.left, e[i].rect.top, 40, 32));//� ���������� � ������ ���� ����� ������ � �����
			e[i].rect.left;//����� �
			e[i].rect.top;//����� Y
		}
	}
	e = lvl.GetObjects("movingPlatform");//�������� ��� ��������� � ������ 
	for (int i = 0; i < e.size(); i++)
		entities.push_back(new MovingPlatform(movePlatformImage, "movingPlatform", lvl, e[i].rect.left, e[i].rect.top, 95, 22));//���������� ��������� � ������.�������� ����������� ��� ������� ���������� ��������� (����� �� tmx �����), � ��� �� �������

	e = lvl.GetObjects("magicF");//�������� ��� ��������� � ������ 
	for (int i = 0; i < e.size(); i++)
		entities.push_back(new magicFlowers(magicFImage, "magicF", lvl, e[i].rect.left, e[i].rect.top, 100, 100));//���������� ��������� � ������.�������� ����������� ��� ������� ���������� ��������� (����� �� tmx �����), � ��� �� �������

	e = lvl.GetObjects("healthF");//�������� ��� ��������� � ������ 
	for (int i = 0; i < e.size(); i++)
		entities.push_back(new healthFlowers(healthFImage, "healthF", lvl, e[i].rect.left, e[i].rect.top, 75, 101));//���������� ��������� � ������.�������� ����������� ��� ������� ���������� ��������� (����� �� tmx �����), � ��� �� �������

	e = lvl.GetObjects("powerF");//�������� ��� ��������� � ������ 
	for (int i = 0; i < e.size(); i++)
		entities.push_back(new powerFlowers(powerFImage, "powerF", lvl, e[i].rect.left, e[i].rect.top, 31, 50 ));//���������� ��������� � ������.�������� ����������� ��� ������� ���������� ��������� (����� �� tmx �����), � ��� �� �������

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
			if (p.isShoot == true) {
				p.isShoot = false;
				entities.push_back(new  MagicPower(MagicPowerImage, "MagicPower", lvl, p.x, p.y, 100, 16, p.state));
				shoot.play();
				p.isMagicPower = true;
			} 


		}

		for (it = entities.begin(); it != entities.end(); it++)//���������� �� ��-��� ������
		{
			if (((*it)->name == "movingPlatform") && ((*it)->getRect().intersects(p.getRect())))//���� ����� ���������� � �������� ������ � ��� ����� ������� movingplatform
			{
				Entity* movPlat = *it;
				if ((p.dy > 0) || (p.onGround == false))//��� ���� ����� ��������� � ��������� ����� ������, �.� ������ ����
					if (p.y + p.h < movPlat->y + movPlat->h)//���� ����� ��������� ���� ���������, �.� ��� ��� ���� ������� (�� �� ��� ��������� ��� �� ���������� � ����������)
					{
						p.y = movPlat->y - p.h + 3; p.x += movPlat->dx * time; p.dy = 0; p.onGround = true; // �� ����������� ������ ���, ����� �� ��� �� ����� �� ���������
					}

			}
			if (((*it)->getRect().intersects(p.getRect())) && ((*it)->name == "easyEnemy"))
			{
				if ((p.dy > 0) && (p.onGround == false)) {
					p.dy = -0.2;
					(*it)->health -= 50;
					ghostDeath.play();
					(*it)->shift = 120;
				    (*it)->update(time,shift,numberLevel);
					
				}//если прыгнули на врага,то даем врагу скорость 0,отпрыгиваем от него чуть вверх,даем ему здоровье 0
				else {
					ghostScream.play();
					(*it)->shift = 60;
					p.health -= 10;	//иначе враг подошел к нам сбоку и нанес урон
				}
				if ((*it)->dx > 0)//���� ���� ���� ������
				{
					(*it)->x = p.x - (*it)->w;
					(*it)->dx *= -1;
					(*it)->sprite.scale(-1, 1);
				}
				if ((*it)->dx < 0)//���� ���� ���� �����
				{
					(*it)->x = p.x + p.w; //���������� - ����������� ������
					(*it)->dx *= -1;//�������������
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
				if ((p.dy > 0) && (p.onGround == false)) {
					p.dy = -0.2;
					(*it)->health -= 20;
					ghostDeath.play();
					(*it)->shift = 120;
					(*it)->update(time, shift, numberLevel);

				}//если прыгнули на врага,то даем врагу скорость 0,отпрыгиваем от него чуть вверх,даем ему здоровье 0
				else {
					ghostScream.play();
					(*it)->shift = 60;
					p.health -= 20;	//иначе враг подошел к нам сбоку и нанес урон
				}
				if ((*it)->dx > 0)//���� ���� ���� ������
				{
					(*it)->x = p.x - (*it)->w;
					(*it)->dx *= -1;
					(*it)->sprite.scale(-1, 1);
				}
				if ((*it)->dx < 0)//���� ���� ���� �����
				{
					(*it)->x = p.x + p.w; //���������� - ����������� ������
					(*it)->dx *= -1;//�������������
					(*it)->sprite.scale(-1, 1);
				}
				if (p.dx < 0) {
					p.x = (*it)->x + (*it)->w;
				}
				if (p.dx > 0) {
					p.x = (*it)->x - p.w;
				}

			}
			
				if ((*it)->name == "easyEnemy")
				{

		
					for ( it2 = entities.begin(); it2 != entities.end(); it2++)
					{
						
						if ((*it2)->name == "MagicPower")
							//std::cout << MagicPower->life;
							if ((*it2)->life == true)
							{
								if ((*it2)->getRect().intersects((*it)->getRect()))
								{
									p.isMagicPower = false;
									(*it2)->life = false;
									(*it)->health = 0;
								}
							}
					}
				}
			
			if (((*it)->getRect().intersects(p.getRect())) && ((*it)->name == "magicF"))
			{
				p.magic += 10;
				(*it)->life = false;
			}
			if (((*it)->getRect().intersects(p.getRect())) && ((*it)->name == "healthF"))
			{
				p.health += 10;
				(*it)->life = false;
			}
			if (((*it)->getRect().intersects(p.getRect())) && ((*it)->name == "powerF"))
			{
				p.power += 20;
				(*it)->life = false;
			}
			for (it2 = entities.begin(); it2 != entities.end(); it2++)
			{
				if ((*it)->getRect() != (*it2)->getRect())
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "easyEnemy") && ((*it2)->name == "easyEnemy"))//���� ����������� ��� ������� � ��� �����
					{
						(*it)->dx *= -1;
						(*it)->sprite.scale(-1, 1);
					}
			}



		}
		lifeBarPlayer.update(p.health,1.1,0);
		magicBarPlayer.update(p.magic,1.1,0);
		powerBarPlayer.update(p.power,0.92,0);
		if ((Keyboard::isKeyPressed(Keyboard::T)) || (p.power == 100)) {
			music.pause();
			winMusic.openFromFile("sound/goodEnd.ogg");
			winMusic.play();
			winMusic.setVolume(15);
			lvl.levelNumber++; 
			return true; }
		if (Keyboard::isKeyPressed(Keyboard::Tab)) { return true; }//если таб, то перезагружаем игру
		if (Keyboard::isKeyPressed(Keyboard::Escape)) { return false; }//если эскейп, то выходим из игры

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
		lifeBarPlayer.draw(window, 0,p.health+10,1.1);//������ ������� ��������
		magicBarPlayer.draw(window, 40, p.magic+10,1.1);
		powerBarPlayer.draw(window, 80, p.power,0.92);

		//window.draw(easyEnemy.sprite);//������ ������� ��������� ������ �����
		window.draw(p.sprite);
		window.display();

	}
}
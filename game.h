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
	int w, h, health;
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
	virtual void update(float time) = 0;
};
////////////////////////////////////////////////////����� ������////////////////////////
class Player :public Entity {
public:
	enum { left, right, up, down, jump, stay, right_Top } state;
	int playerScore;
	bool isShoot;
	int power, magic;

	Player(Image& image, String Name, Level& lev, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		playerScore = 0; state = stay; obj = lev.GetAllObjects();//��������������.�������� ��� ������� ��� �������������� ��������� � ������
		if (name == "Player1") {
			sprite.setTextureRect(IntRect(4, 19, w, h));
		}
		health = 50;
		power = 0;
		magic = 50;
	}

	void control() {
		if (Keyboard::isKeyPressed) {
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				state = left; speed = 0.2;
				if (!isFast) {
					speed = 0.05;
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				state = right; speed = 0.2;
				if (!isFast) {
					speed = 0.05;
				}
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround)) {

				state = jump; dy = -0.9; onGround = false;
				if (!isFast) {
					speed = 0.05;
				}
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				state = down;
				if (!isFast) {
					speed = 0.05;
				}
			}
			if ((Keyboard::isKeyPressed(Keyboard::Right)) && (Keyboard::isKeyPressed(Keyboard::Up))) {
				state = right_Top;
			}

			/////�������
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
				if (obj[i].name == "magicF")//���� ��������� �����������
				{
					magic+=10;
				}
				if (obj[i].name == "powerF")//���� ��������� �����������
				{
					power++;
				}
				if (obj[i].name == "healthF")//���� ��������� �����������
				{
					health += 20;
				}
			}
	}

	void update(float time)
	{
		control();
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
		setPlayerCoordinateForView(x, y);
		if (life) { setPlayerCoordinateForView(x, y); }
		dy = dy + 0.0015 * time;
	}


};

class Bullet :public Entity {//����� ����
public:
	int direction;//����������� ����

	Bullet(Image& image, String Name, Level& lvl, float X, float Y, int W, int H, int dir) :Entity(image, Name, X, Y, W, H) {//�� ��� ��, ������ ����� � ����� ��������� ������ (int dir)
		obj = lvl.GetObjects("solid");//�������������� .�������� ������ ������� ��� �������������� ���� � ������
		x = X;
		y = Y;
		direction = dir;
		speed = 0.8;
		w = h = 16;
		life = true;
		//���� ������������� � ������������
	}


	void update(float time)
	{
		switch (direction)
		{
		case 0: dx = -speed; dy = 0;   break;//������� �������� state = left
		case 1: dx = speed; dy = 0;   break;//������� �������� state = right
		case 2: dx = 0; dy = -speed;   break;//������� �������� state = up
		case 3: dx = 0; dy = -speed;   break;//������� �������� �� ������� ��������� � �����������, ���� ������ ��������� �����, ��� ������ ��� �� �����
		case 4: dx = 0; dy = -speed;   break;//������� �������� �� ������� ��������� � �����������, ���� ������ ��������� �����, ��� ������ ��� �� �����
		case 5: dx = 0; dy = -speed;   break;//������� �������� �� ������� ��������� � �����������, ���� ������ ��������� �����, ��� ������ ��� �� �����
		case 6: dx = speed; dy = -speed;   break;//������� �������� state = right_Top

		}

		x += dx * time;//���� �������� ���� �� �
		y += dy * time;//�� �

		if (x <= 0) x = 1;// �������� ���� � ����� �����, ����� ��� ���������� ������ ��� �������� �� �������� �� ������ ����� � �� ���� ������
		if (y <= 0) y = 1;

		for (int i = 0; i < obj.size(); i++) {//������ �� �������� solid
			if (getRect().intersects(obj[i].rect)) //���� ���� ������ ���������� � �����,
			{
				life = false;// �� ���� �������
			}
		}

		sprite.setPosition(x + w / 2, y + h / 2);//�������� ������� ����
	}
};



class MovingPlatform : public Entity {//����� ���������� ���������
public:
	MovingPlatform(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		sprite.setTextureRect(IntRect(0, 0, W, H));//������������� 
		dx = 0.08;//����������� ��������� �� �
	}

	void update(float time)//������� ���������� ���������.
	{
		x += dx * time;//���������� �������� �� �����������
		moveTimer += time;//���������� ������
		if (moveTimer > 2000) { dx *= -1; moveTimer = 0; }//���� ������ �������� 2 ���, �� �������� ����������� �������� ���������,� ������ ����������
		sprite.setPosition(x + w / 2, y + h / 2);//������ ������� �������
	}
};


class Enemy :public Entity {
public:
	Enemy(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		obj = lvl.GetObjects("solid");//��������������.�������� ������ ������� ��� �������������� ����� � ������
		if (name == "easyEnemy") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
			dx = 0.1;
			health = 100;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = 0; i < obj.size(); i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "solid")//���� ��������� �����������
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx > 0) { x = obj[i].rect.left - w;  dx = -0.1; sprite.scale(-1, 1); }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; sprite.scale(-1, 1); }
				}
			}
	}


	void update(float time)
	{
		if (name == "easyEnemy") {
			//moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//������ ����������� �������� ������ 3 ���(�������������� ������ ����� �����������)
			checkCollisionWithMap(dx, 0);
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
		sprite.setScale(0.2f,0.2f);
		sprite.setPosition(x + (w*0.2f) / 2-10, y + (h*0.2f)/2-20);//�������� ������� ����

	}

	void update(float time)//������� ���������� ���������.
	{
		if (health <= 0) { life = false; }
	}
};
class powerFlowers :public Entity {//класс цветов
public:
	powerFlowers(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		sprite.setTextureRect(IntRect(0, 0, W, H));//������������� 
		sprite.setScale(0.1f, 0.1f);
		sprite.setPosition(x + (w * 0.1f) / 2-10, y + (h * 0.1f) / 2-20);//�������� ������� ����

	};
	void update(float time) { if (health <= 0) { life = false; } };
};
class healthFlowers :public Entity {//класс цветов
public:
	healthFlowers(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		sprite.setTextureRect(IntRect(0, 0, W, H));//������������� 
		sprite.setScale(0.1f, 0.1f);
		sprite.setPosition(x + (w * 0.1f) / 2-10, y + (h * 0.1f) / 2-20);//�������� ������� ����

	};
	void update(float time) { if (health <= 0) { life = false; } };
};

void changeLevel(Level& lvl, int& numberLevel) {
	if (numberLevel == 1) { lvl.LoadFromFile("map/swampMap.tmx"); }
	if (numberLevel == 2) { lvl.LoadFromFile("map/lakeMap.tmx"); }
	if (numberLevel == 3) { lvl.LoadFromFile("map3.tmx"); }
}

bool startGame(RenderWindow& window, int& numberLevel) {


	//menu(window);
	view.reset(sf::FloatRect(0, 0, 1376, 768));
	Level lvl;
	changeLevel(lvl, numberLevel);//для загрузки карты для нужного уровня

	Image heroImage;
	heroImage.loadFromFile("images/MilesTailsPrower.gif");

	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/man.png");
	easyEnemyImage.createMaskFromColor(Color(255, 0, 0));

	Image movePlatformImage;
	movePlatformImage.loadFromFile("images/MovingPlatform.png");

	Image magicFImage;
	magicFImage.loadFromFile("images/magicF.png");
	Image healthFImage;
	healthFImage.loadFromFile("images/healthF.png");
	healthFImage.createMaskFromColor(Color(255, 255, 255));
	Image powerFImage;
	powerFImage.loadFromFile("images/powerF.png");


	Image BulletImage;//����������� ��� ����
	BulletImage.loadFromFile("images/bullet.png");//��������� �������� � ������ �����������
	BulletImage.createMaskFromColor(Color(0, 0, 0));

	Bar lifeBarPlayer("images/HealthBar/1.png", 202, 667);//��������� ������ ������� ��������
	Bar magicBarPlayer("images/MagicBar/1.png", 202, 667);//��������� ������ ������� ��������


	SoundBuffer shootBuffer;//������ ����� ��� �����
	shootBuffer.loadFromFile("sound/shoot.ogg");//��������� � ���� ����
	Sound shoot(shootBuffer);//������� ���� � ��������� � ���� ���� �� ������

	Music music;//������� ������ ������
	music.openFromFile("sound/swamp.ogg");//��������� ����
	music.play();//������������� ������
	music.setLoop(true);
	music.setVolume(15);

	Object player = lvl.GetObject("player");


	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 40, 30);
	std::vector<Object> e = lvl.GetObjects("easyEnemy");//��� ������� ����� �� tmx ����� �������� � ���� �������
	std::list<Entity*>  entities;//������ ������, ���� ���� ������ �������.�������� ������.
	std::list<Entity*>::iterator it;//�������� ����� ��������� �� ��-��� ������
	std::list<Entity*>::iterator it2;//������ ��������.��� �������������� ����� ��������� ������


	for (int i = 0; i < e.size(); i++) {//���������� �� ��������� ����� �������(� ������ �� ������)
		entities.push_back(new Enemy(easyEnemyImage, "easyEnemy", lvl, e[i].rect.left, e[i].rect.top, 40, 65));//� ���������� � ������ ���� ����� ������ � �����
		e[i].rect.left;//����� �
		e[i].rect.top;//����� Y
	}

	e = lvl.GetObjects("movingPlatform");//�������� ��� ��������� � ������ 

	for (int i = 0; i < e.size(); i++)
		entities.push_back(new MovingPlatform(movePlatformImage, "movingPlatform", lvl, e[i].rect.left, e[i].rect.top, 95, 22));//���������� ��������� � ������.�������� ����������� ��� ������� ���������� ��������� (����� �� tmx �����), � ��� �� �������

	e = lvl.GetObjects("magicF");//�������� ��� ��������� � ������ 

	for (int i = 0; i < e.size(); i++)
		entities.push_back(new magicFlowers(magicFImage, "magicF", lvl, e[i].rect.left, e[i].rect.top, 310, 500));//���������� ��������� � ������.�������� ����������� ��� ������� ���������� ��������� (����� �� tmx �����), � ��� �� �������

	e = lvl.GetObjects("healthF");//�������� ��� ��������� � ������ 

	for (int i = 0; i < e.size(); i++)
		entities.push_back(new healthFlowers(healthFImage, "healthF", lvl, e[i].rect.left, e[i].rect.top, 880, 968));//���������� ��������� � ������.�������� ����������� ��� ������� ���������� ��������� (����� �� tmx �����), � ��� �� �������

	e = lvl.GetObjects("powerF");//�������� ��� ��������� � ������ 

	for (int i = 0; i < e.size(); i++)
		entities.push_back(new powerFlowers(powerFImage, "powerF", lvl, e[i].rect.left, e[i].rect.top, 900, 1000));//���������� ��������� � ������.�������� ����������� ��� ������� ���������� ��������� (����� �� tmx �����), � ��� �� �������

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
				if (p.isShoot == true) {
					p.isShoot = false;
					entities.push_back(new Bullet(BulletImage, "Bullet", lvl, p.x, p.y, 16, 16, p.state));
					shoot.play();
				}//���� ����������, �� ���������� ����. enum �������� ��� int 

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
				}//если прыгнули на врага,то даем врагу скорость 0,отпрыгиваем от него чуть вверх,даем ему здоровье 0
				else {
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
			if (((*it)->getRect().intersects(p.getRect())) && ((*it)->name == "magicF"))
			{
			
			}
			if (((*it)->getRect().intersects(p.getRect())) && ((*it)->name == "healthF"))
			{

			}
			if (((*it)->getRect().intersects(p.getRect())) && ((*it)->name == "powerF"))
			{

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
		lifeBarPlayer.update(p.health);
		magicBarPlayer.update(p.magic);

		if (Keyboard::isKeyPressed(Keyboard::T)) { lvl.levelNumber++; return true; }
		if (Keyboard::isKeyPressed(Keyboard::Tab)) { return true; }//если таб, то перезагружаем игру
		if (Keyboard::isKeyPressed(Keyboard::Escape)) { return false; }//если эскейп, то выходим из игры



		p.update(time);
		for (it = entities.begin(); it != entities.end();)//������� ��� ���������� �� ������ �� �����
		{
			Entity* b = *it;//��� ��������, ����� �� ������ (*it)->
			b->update(time);//�������� �-��� update ��� ���� �������� (�� ���� ��� ���, ��� ���)
			if (b->life == false) { it = entities.erase(it); delete b; }// ���� ���� ������ �����, �� ������� ���
			else it++;//� ���� �������� (����������) � ���� �������. ��� ������ �� ����� ��������� ������
		}



		window.setView(view);
		window.clear(Color(77, 83, 140));
		lvl.Draw(window);


		for (it = entities.begin(); it != entities.end(); it++) {
			window.draw((*it)->sprite);
		}
		lifeBarPlayer.draw(window, 0,p.health+10);//������ ������� ��������
		magicBarPlayer.draw(window, 40, p.magic+10);

		//window.draw(easyEnemy.sprite);//������ ������� ��������� ������ �����
		window.draw(p.sprite);
		window.display();

	}
}
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
#include "LifeBar.h"

using namespace sf;
////////////////////////////////////Общий класс-родитель//////////////////////////
class Entity {
public:
	std::vector<Object> obj;//вектор объектов карты
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

	FloatRect getRect() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений 
	}
	virtual void update(float time) = 0;
};
////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player :public Entity {
public:
	enum { left, right, up, down, jump, stay, right_Top} state;
	int playerScore;
	bool isShoot;


	Player(Image& image, String Name, Level& lev, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		playerScore = 0; state = stay; obj = lev.GetAllObjects();//инициализируем.получаем все объекты для взаимодействия персонажа с картой
		if (name == "Player1") {
			sprite.setTextureRect(IntRect(4, 19, w, h));
		}
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

			/////выстрел
			if (Keyboard::isKeyPressed(Keyboard::Space)) {
				isShoot = true;
			}
			
			
		}
	};



	void checkCollisionWithMap(float Dx, float Dy)
	{

		for (int i = 0; i < obj.size(); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")//если встретили препятствие
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx > 0) { x = obj[i].rect.left - w; }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }
					isFast = true;
				}
				if (obj[i].name == "swamp")//если встретили препятствие
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx > 0) { x = obj[i].rect.left - w; }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }
					isFast = false;
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

class Bullet :public Entity {//класс пули
public:
	int direction;//направление пули

	Bullet(Image& image, String Name, Level& lvl, float X, float Y, int W, int H, int dir) :Entity(image, Name, X, Y, W, H) {//всё так же, только взяли в конце состояние игрока (int dir)
		obj = lvl.GetObjects("solid");//инициализируем .получаем нужные объекты для взаимодействия пули с картой
		x = X;
		y = Y;
		direction = dir;
		speed = 0.8;
		w = h = 16;
		life = true;
		//выше инициализация в конструкторе
	}


	void update(float time)
	{
		switch (direction)
		{
		case 0: dx = -speed; dy = 0;   break;//интовое значение state = left
		case 1: dx = speed; dy = 0;   break;//интовое значение state = right
		case 2: dx = 0; dy = -speed;   break;//интовое значение state = up
		case 3: dx = 0; dy = -speed;   break;//интовое значение не имеющее отношения к направлению, пока просто стрельнем вверх, нам сейчас это не важно
		case 4: dx = 0; dy = -speed;   break;//интовое значение не имеющее отношения к направлению, пока просто стрельнем вверх, нам сейчас это не важно
		case 5: dx = 0; dy = -speed;   break;//интовое значение не имеющее отношения к направлению, пока просто стрельнем вверх, нам сейчас это не важно
		case 6: dx = speed; dy = -speed;   break;//интовое значение state = right_Top

		}

		x += dx * time;//само движение пули по х
		y += dy * time;//по у

		if (x <= 0) x = 1;// задержка пули в левой стене, чтобы при проседании кадров она случайно не вылетела за предел карты и не было ошибки
		if (y <= 0) y = 1;

		for (int i = 0; i < obj.size(); i++) {//проход по объектам solid
			if (getRect().intersects(obj[i].rect)) //если этот объект столкнулся с пулей,
			{
				life = false;// то пуля умирает
			}
		}

		sprite.setPosition(x + w / 2, y + h / 2);//задается позицию пуле
	}
};



class MovingPlatform : public Entity {//класс движущейся платформы
public:
	MovingPlatform(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		sprite.setTextureRect(IntRect(0, 0, W, H));//прямоугольник 
		dx = 0.08;//изначальное ускорение по Х
	}

	void update(float time)//функция обновления платформы.
	{
		x += dx * time;//реализация движения по горизонтали
		moveTimer += time;//наращиваем таймер
		if (moveTimer > 2000) { dx *= -1; moveTimer = 0; }//если прошло примерно 2 сек, то меняется направление движения платформы,а таймер обнуляется
		sprite.setPosition(x + w / 2, y + h / 2);//задаем позицию спрайту
	}
};



class Enemy :public Entity {
public:
	Enemy(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		obj = lvl.GetObjects("solid");//инициализируем.получаем нужные объекты для взаимодействия врага с картой
		if (name == "easyEnemy") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
			dx = 0.1;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = 0; i < obj.size(); i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "solid")//если встретили препятствие
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
			//moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//меняет направление примерно каждые 3 сек(альтернативная версия смены направления)
			checkCollisionWithMap(dx, 0);
			x += dx * time;
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { life = false; }
		}
	}
};

void menu(RenderWindow& window) {
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("images/111.png");
	menuTexture2.loadFromFile("images/222.png");
	menuTexture3.loadFromFile("images/333.png");
	aboutTexture.loadFromFile("images/about.png");
	menuBackground.loadFromFile("images/Penguins.jpg");
	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground);
	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(100, 30);
	menu2.setPosition(100, 90);
	menu3.setPosition(100, 150);
	menuBg.setPosition(345, 0);
	//////////////////////////////МЕНЮ///////////////////
	while (isMenu)
	{
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menu3.setColor(Color::White);
		menuNum = 0;
		window.clear(Color(129, 181, 221));
		if (IntRect(100, 30, 300, 50).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Blue); menuNum = 1; }
		if (IntRect(100, 90, 300, 50).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Blue); menuNum = 2; }
		if (IntRect(100, 150, 300, 50).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Blue); menuNum = 3; }
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) isMenu = false;//если нажали первую кнопку, то выходим из меню 
			if (menuNum == 2) { window.draw(about); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
			if (menuNum == 3) { window.close(); isMenu = false; }
		}
		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);
		window.display();
	}
	////////////////////////////////////////////////////
}

int main()
{

	RenderWindow window(VideoMode(1376, 768), "SELSOR");
	menu(window);//вызов меню
    view.reset(sf::FloatRect(0, 0, 1376, 768));
	Level lvl;//создали экземпляр класса уровень
	lvl.LoadFromFile("map/swampMap.tmx");//загрузили в него карту, внутри класса с помощью методов он ее обработает.

	Image heroImage;
	heroImage.loadFromFile("images/MilesTailsPrower.gif");

	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/man.png");
	easyEnemyImage.createMaskFromColor(Color(255, 0, 0));

	Image movePlatformImage;
	movePlatformImage.loadFromFile("images/MovingPlatform.png");

	Image BulletImage;//изображение для пули
	BulletImage.loadFromFile("images/bullet.png");//загрузили картинку в объект изображения
	BulletImage.createMaskFromColor(Color(0, 0, 0));

	LifeBar lifeBarPlayer;//экземпляр класса полоски здоровья

	SoundBuffer shootBuffer;//создаём буфер для звука
	shootBuffer.loadFromFile("sound/shoot.ogg");//загружаем в него звук
	Sound shoot(shootBuffer);//создаем звук и загружаем в него звук из буфера
	
	Music music;//создаем объект музыки
	music.openFromFile("sound/swamp.ogg");//загружаем файл
	music.play();//воспроизводим музыку
	music.setLoop(true);
	music.setVolume(15);

	Object player = lvl.GetObject("player");


	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 40, 30);
	std::vector<Object> e = lvl.GetObjects("easyEnemy");//все объекты врага на tmx карте хранятся в этом векторе


	std::list<Entity*>  entities;//создаю список, сюда буду кидать объекты.например врагов.
	std::list<Entity*>::iterator it;//итератор чтобы проходить по эл-там списка
	std::list<Entity*>::iterator it2;//второй итератор.для взаимодействия между объектами списка

	
	for (int i = 0; i < e.size(); i++){//проходимся по элементам этого вектора(а именно по врагам)
		entities.push_back(new Enemy(easyEnemyImage, "easyEnemy", lvl, e[i].rect.left, e[i].rect.top, 40, 65));//и закидываем в список всех наших врагов с карты
	    e[i].rect.left;//коорд Х
		e[i].rect.top;//коорд Y
	}

	e = lvl.GetObjects("movingPlatform");//забираем все платформы в вектор 

	for (int i = 0; i < e.size(); i++)
		entities.push_back(new MovingPlatform(movePlatformImage, "movingPlatform", lvl, e[i].rect.left, e[i].rect.top, 95, 22));//закидываем платформу в список.передаем изображение имя уровень координаты появления (взяли из tmx карты), а так же размеры
	       
	Clock clock;
	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();

		clock.restart();
		time = time / 800;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (p.isShoot == true) { 
				p.isShoot = false; 
				entities.push_back(new Bullet(BulletImage, "Bullet", lvl, p.x, p.y, 16, 16, p.state)); 
				shoot.play();
			}//если выстрелили, то появляется пуля. enum передаем как int 

		}
		for (it = entities.begin(); it != entities.end();)//говорим что проходимся от начала до конца
		{
			Entity *b = *it;//для удобства, чтобы не писать (*it)->
			b->update(time);//вызываем ф-цию update для всех объектов (по сути для тех, кто жив)
			if (b->life == false)	{ it = entities.erase(it); delete b; }// если этот объект мертв, то удаляем его
			else it++;//и идем курсором (итератором) к след объекту. так делаем со всеми объектами списка
		}
 
 
 
		for (it = entities.begin(); it != entities.end(); it++)//проходимся по эл-там списка
		{
			if (((*it)->name == "movingPlatform") && ((*it)->getRect().intersects(p.getRect())))//если игрок столкнулся с объектом списка и имя этого объекта movingplatform
			{
				Entity *movPlat = *it;
				if ((p.dy>0) || (p.onGround == false))//при этом игрок находится в состоянии после прыжка, т.е падает вниз
				if (p.y + p.h<movPlat->y + movPlat->h)//если игрок находится выше платформы, т.е это его ноги минимум (тк мы уже проверяли что он столкнулся с платформой)
				{
					p.y = movPlat->y - p.h + 3; p.x += movPlat->dx*time; p.dy = 0; p.onGround = true; // то выталкиваем игрока так, чтобы он как бы стоял на платформе
				}
			}
			
			if ((*it)->getRect().intersects(p.getRect()))
			{
				if ((*it)->name == "easyEnemy"){//и при этом имя объекта EasyEnemy,то..
 
					////////выталкивание врага
					if ((*it)->dx>0)//если враг идет вправо
					{
						std::cout << "(*it)->x" << (*it)->x << "\n";//коорд игрока
						std::cout << "p.x" << p.x << "\n\n";//коорд врага
 
						(*it)->x = p.x - (*it)->w; //отталкиваем его от игрока влево (впритык)
						(*it)->dx = 0;//останавливаем
						
						std::cout << "new (*it)->x" << (*it)->x << "\n";//новая коорд врага
						std::cout << "new p.x" << p.x << "\n\n";//новая коорд игрока (останется прежней)
					}
					if ((*it)->dx < 0)//если враг идет влево
					{
						(*it)->x = p.x + p.w; //аналогично - отталкиваем вправо
						(*it)->dx = 0;//останавливаем
					}
					///////выталкивание игрока
					if (p.dx < 0) { p.x = (*it)->x + (*it)->w;}//если столкнулись с врагом и игрок идет влево то выталкиваем игрока
					if (p.dx > 0) { p.x = (*it)->x - p.w;}//если столкнулись с врагом и игрок идет вправо то выталкиваем игрока
				}			
 
				
				
			}
 
			for (it2 = entities.begin(); it2 != entities.end(); it2++)
			{
				if ((*it)->getRect() != (*it2)->getRect())//при этом это должны быть разные прямоугольники
				if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "EasyEnemy") && ((*it2)->name == "EasyEnemy"))//если столкнулись два объекта и они враги
				{
					(*it)->dx *= -1;//меняем направление движения врага
					(*it)->sprite.scale(-1, 1);//отражаем спрайт по горизонтали
				}
			}
			
 
 
		}
		lifeBarPlayer.update(100);
		p.update(time);
		window.setView(view);
		window.clear(Color(77,83,140));
		lvl.Draw(window);
 
 
		for (it = entities.begin(); it != entities.end(); it++){
			window.draw((*it)->sprite); 
		}
		lifeBarPlayer.draw(window);//рисуем полоску здоровья
		//window.draw(easyEnemy.sprite);//старый вариант рисования одного врага
		window.draw(p.sprite);
		window.display();
	}
	return 0;
}
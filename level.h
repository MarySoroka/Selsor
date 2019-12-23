#include <SFML/Audio.hpp>
#include "view.h"
#include "map.h"
#include <vector>
#include <list>
#include "TinyXML/tinyxml.h"

#include "Bar.h"
#include "Entity.h"
#include "Player.h"
#include "Flowers.h"
#include "MagicPower.h"
#include "MovingPlatform.h"
#include "Enemy.h"

using namespace sf;

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
		lvl.LoadFromFile("map/forestmap.tmx");
		break;
	case 5:
		lvl.LoadFromFile("map/finallMap.tmx");
		break;

	default:
		break;
	}
}

enum stateOfGame { next, reload, ex, loadMenu};

enum stateOfGame downloadLevel(RenderWindow& window, int& level) {

	view.reset(sf::FloatRect(0, 0, 1376, 768));
	Level lvl;
	nextLevel(lvl, level);
	Image easyEnemyImage;
	Image movePlatformImage;
	Image magicFImage;
	magicFImage.loadFromFile("images/flowers/magicF.png");
	Image healthFImage;
	healthFImage.loadFromFile("images/flowers/healthF.png");
	Image powerFImage;
	powerFImage.loadFromFile("images/flowers/powerF.png");

	Music music;
	music.setVolume(25);

	SoundBuffer winBuffer;
	winBuffer.loadFromFile("sound/goodEnd.ogg");
	Sound win(winBuffer);

	SoundBuffer ghostScreamtBuffer;
	ghostScreamtBuffer.loadFromFile("sound/ghostScream.ogg");
	Sound ghostScream(ghostScreamtBuffer);
	ghostScream.setVolume(15);

	SoundBuffer ghostDeathBuffer;
	ghostDeathBuffer.loadFromFile("sound/ghostDeath.ogg");
	Sound ghostDeath(ghostDeathBuffer);
	ghostDeath.setVolume(15);

	SoundBuffer dogScreamtBuffer;
	dogScreamtBuffer.loadFromFile("sound/dogGrowl.ogg");
	Sound dogScream(dogScreamtBuffer);
	dogScream.setVolume(15);

	SoundBuffer dogDeathBuffer;
	dogDeathBuffer.loadFromFile("sound/dogDeath.ogg");
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

	SoundBuffer deathDemonBuffer;
	deathDemonBuffer.loadFromFile("sound/demonDeath.ogg");
	Sound deathDemon(deathDemonBuffer);
	deathDemon.setVolume(15);

	SoundBuffer skeletDeathBuffer;
	skeletDeathBuffer.loadFromFile("sound/skelletonDeath.ogg");
	Sound skeletDeath(skeletDeathBuffer);
	skeletDeath.setVolume(15);

	SoundBuffer skeletBuffer;
	skeletBuffer.loadFromFile("sound/AttackSkel.ogg");
	Sound  skelet(skeletBuffer);
	skelet.setVolume(15);

	SoundBuffer demonBuffer;
	demonBuffer.loadFromFile("sound/demonAttack.ogg");
	Sound demon(demonBuffer);
	demon.setVolume(15);


	SoundBuffer flowerBuffer;
	flowerBuffer.loadFromFile("sound/flowerGathering.ogg");
	Sound flower(flowerBuffer);
	flower.setVolume(15);

	Image middleEnemyImage;
	Image hardEnemyImage;
	Image bossImage;

	Image  MagicPowerImage;
	MagicPowerImage.loadFromFile("images/hero/MagicEffect2.png");
	String nameOfBar;
	if (level == 5) {
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

	switch (level)
	{
	case 1: {
		easyEnemyImage.loadFromFile("images/enemies/swampGhost/ghost.png");
		movePlatformImage.loadFromFile("images/platform/MovingPlatform.png");
		music.openFromFile("sound/swamp.ogg");
		music.play();
		Henemy = 80;
		wEnemy = 40;
		break;
	}
	case 2:
	{
		easyEnemyImage.loadFromFile("images/enemies/wizard/wizard.png");
		movePlatformImage.loadFromFile("images/platform/MovingPlatform2.png");
		music.openFromFile("sound/lake.ogg");
		music.play();
		Henemy = 80;
		wEnemy = 90;
		break;
	}
	case 3: {
		easyEnemyImage.loadFromFile("images/enemies/dog/dog.png");
		middleEnemyImage.loadFromFile("images/enemies/skeleton/skelet.png");
		movePlatformImage.loadFromFile("images/platform/MovingPlatform3.png");
		Henemy = 20;
		wEnemy = 40;
		music.openFromFile("sound/home.ogg");
		music.play();
		break;
	}
	case 4: {
		easyEnemyImage.loadFromFile("images/enemies/swampGhost/ghost.png");
		middleEnemyImage.loadFromFile("images/enemies/wizard/wizard.png");
		hardEnemyImage.loadFromFile("images/enemies/wizard/wizard.png");
		movePlatformImage.loadFromFile("images/platform/MovingPlatform.png");
		Henemy = 20;
		wEnemy = 40;
		music.openFromFile("sound/forest.ogg");
		music.play();
		break;
	}
	case 5: {
		easyEnemyImage.loadFromFile("images/enemies/dog/dog.png");
		middleEnemyImage.loadFromFile("images/enemies/wizard/wizard.png");
		hardEnemyImage.loadFromFile("images/enemies/swampGhost/ghost.png");
		bossImage.loadFromFile("images/enemies/demon/demon.png");
		movePlatformImage.loadFromFile("images/platform/MovingPlatform3.png");
		Henemy = 140;
		wEnemy = 150;
		music.openFromFile("sound/final.ogg");
		music.play();
		break;
	}
	default:
		break;
	}


	Image heroImage;
	heroImage.loadFromFile("images/hero/girlCharacter.png");

	music.setLoop(true);


	Object player = lvl.GetObject("player");
	Player p(heroImage, "Player", lvl, player.rect.left, player.rect.top, 60, 60);


	for (int i = 0; i < e.size(); i++) {
		entities.push_back(new Enemy(easyEnemyImage, "easyEnemy", lvl, e[i].rect.left, e[i].rect.top, wEnemy, Henemy));
		e[i].rect.left;
		e[i].rect.top;
	}
	if ((level == 3)||(level == 4)) {
		e = lvl.GetObjects("middleEnemy");
		for (int i = 0; i < e.size(); i++) {
			entities.push_back(new Enemy(middleEnemyImage, "middleEnemy", lvl, e[i].rect.left, e[i].rect.top, wEnemy, Henemy));
			e[i].rect.left;
			e[i].rect.top;
		}

	}
	if ((level == 4)||(level == 5)) {
		e = lvl.GetObjects("hardEnemy");
		for (int i = 0; i < e.size(); i++) {
			entities.push_back(new Enemy(hardEnemyImage, "hardEnemy", lvl, e[i].rect.left, e[i].rect.top, wEnemy, Henemy));
			e[i].rect.left;
			e[i].rect.top;
		}
	}
	if (level == 5) {
		e = lvl.GetObjects("boss");
		entities.push_back(new Enemy(bossImage, "boss", lvl, e[0].rect.left, e[0].rect.top, wEnemy, Henemy));
	}
	int bossHealth = 3000;

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
		entities.push_back(new powerFlowers(powerFImage, "powerF", lvl, e[i].rect.left, e[i].rect.top, 31, 50));

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
					entities.push_back(new  MagicPower(MagicPowerImage, "MagicPower", lvl, p.x, p.y, 16, 16, p.state));
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
					if ((level == 3) || (level == 5)) {
						dogDeath.play();
						(*it)->shift = 40;
					}
					else {
						ghostDeath.play();
						if (level == 2) {
							(*it)->shift = 60;
						}
						else {
							(*it)->shift = 120;
						}
					}
					(*it)->update(time, shift, level);

				}
				else {
					if ((level == 3) || (level == 5)) {
						dogScream.play();
						(*it)->shift = 40;
					}
					else {
						ghostScream.play();
						(*it)->shift = 60;
					}
					
					p.health -= 5;
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
					if (level == 3) {
						skeletDeath.play();
						(*it)->shift = 120;
					}
					else {
						ghostDeath.play();
						(*it)->shift = 120;
					}
					
					(*it)->update(time, shift, level);

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
			if (((*it)->getRect().intersects(p.getRect())) && ((*it)->name == "hardEnemy"))
			{
				if ((p.dy > 0) && (p.isGravity == false)) {
					p.dy = -0.2;
					(*it)->health -= 10;
					ghostDeath.play();
					(*it)->shift = 120;
					(*it)->update(time, shift, level);

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
			if (((*it)->getRect().intersects(p.getRect())) && ((*it)->name == "boss"))
			{
				if ((p.dy > 0) && (p.isGravity == false)) {
					p.dy = -0.2;
					(*it)->health -= 5;
					bossHealth -= 5;
					deathDemon.play();
					(*it)->shift = 120;
					(*it)->update(time, shift, level);
				}
				else {
					demon.play();
					(*it)->shift = 60;
					p.health -= 25;
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

			for (it2 = entities.begin(); it2 != entities.end(); it2++)
			{
				if ((*it)->getRect() != (*it2)->getRect())
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "easyEnemy") && ((*it2)->name == "MagicPower"))
					{
						(*it)->health -= 100;
						if ((level == 3) || (level == 5)) {
							dogDeath.play();
						}
						else {
							ghostDeath.play();
						}
						(*it)->shift = 120;
						(*it)->update(time, shift, level);
						(*it2)->life = false;

					}
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "middleEnemy") && ((*it2)->name == "MagicPower"))
					{
						(*it)->health -= 50;
						if ((level == 3) || (level == 5)) {
							skeletDeath.play();
						}
						else {
							ghostDeath.play();
						}
						(*it)->shift = 120;
						(*it)->update(time, shift, level);
						(*it2)->life = false;
					}
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "hardEnemy") && ((*it2)->name == "MagicPower"))
					{
						(*it)->health -= 25;
						ghostDeath.play();
						(*it)->shift = 120;
						(*it)->update(time, shift, level);
						(*it2)->life = false;

					}
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "boss") && ((*it2)->name == "MagicPower"))
					{
						(*it)->health -= 20;
						bossHealth -= 20;
						deathDemon.play();
						if (bossHealth <= 500) {
							(*it)->shift = 750;
						}
						else {
							(*it)->shift = 0;
						}
						(*it)->update(time, shift, level);

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
		lifeBarPlayer.update(p.health, 1.1, 0);
		magicBarPlayer.update(p.magic, 1.1, 0);
		if (level == 5) {
			powerBarPlayer.update(bossHealth, 1.1, 1);
		}
		else {
			powerBarPlayer.update(p.power, 0.92, 0);
		}


	    if ((Keyboard::isKeyPressed(Keyboard::T)) || (p.power == 100) || (bossHealth == 0)) {
			win.play();
			if (level == 5) {
				return loadMenu;
			}
			else {
				lvl.levelNumber++;
				return next;
			}
		}
		
		
		if ((Keyboard::isKeyPressed(Keyboard::Tab)) || (p.health == 0)) { 
			death.play(); return reload;
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape)) { 
			return ex; 
		}

		p.CurrentFrame += 0.005 * time;
		p.update(time, 0, level);



		for (it = entities.begin(); it != entities.end();)
		{
			Entity* b = *it;
			if (b->name == "easyEnemy") {
				b->update(time, b->shift, level);
			}
			else {
				b->update(time, 0, level);
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
		lifeBarPlayer.draw(window, 0, p.health + 10, 1.1, 0);
		magicBarPlayer.draw(window, 40, p.magic + 10, 1.1,0);
		
		if (level == 5) {
			powerBarPlayer.draw(window, 80, bossHealth+10, 1.1,1);
		}
		else {
			powerBarPlayer.draw(window, 80, p.power, 0.92,0);
		}
		window.draw(p.sprite);
		window.display();

	}
}
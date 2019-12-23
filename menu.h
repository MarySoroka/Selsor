#ifndef MENU_H
#define MENU_H


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;
enum menuState {newGame, aboutGame, exitGame};

bool menu() {
	RenderWindow window(VideoMode(1376, 768), "SELSOR");
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("images/menu/newGame.png");
	menuTexture2.loadFromFile("images/menu/aboutGame.png");
	menuTexture3.loadFromFile("images/menu/exit.png");
	aboutTexture.loadFromFile("images/menu/about.jpg");
	menuBackground.loadFromFile("images/menu/backgr.png");
	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground);
	bool isMenu = 1;
	enum menuState menuNum;
	menu1.setPosition(780, 30);
	menu2.setPosition(780, 90);
	menu3.setPosition(780, 150);
	menuBg.setPosition(0, 0);

	Music music;
	music.openFromFile("sound/cemetery.ogg");
	music.play();
	music.setVolume(25);

	while (isMenu)
	{
		music.setLoop(true);
		window.clear(Color(255, 255, 255));
		if (IntRect(780, 30, 300, 50).contains(Mouse::getPosition(window))) {  menuNum = newGame; }
		if (IntRect(780, 90, 300, 50).contains(Mouse::getPosition(window))) { menuNum = aboutGame; }
		if (IntRect(780, 150, 300, 50).contains(Mouse::getPosition(window))) {  menuNum = exitGame; }
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			switch (menuNum)
			{
			case newGame:
				return true;
				break;
			case aboutGame:
				window.draw(about); 
				window.display(); 
				while (!Keyboard::isKeyPressed(Keyboard::Escape));
				break;
			case exitGame:
				window.close(); 
				return false;
				break;
			default:
				break;
			}
		}
		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);
		window.display();
	}
}

#endif  
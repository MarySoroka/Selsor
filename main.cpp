#include <SFML/Graphics.hpp>
#include "map.h"
#include "menu.h"
#include "level.h"


void gameRunning(RenderWindow& window, int& level) {
	switch (downloadLevel(window, level))
	{
	case next :
		level++; 
	    gameRunning(window, level);
		break;
	case reload :
		gameRunning(window, level);
		break;

	default:
		break;
	}
}


int main(){
	
	while (menu()) {
		int currentLevel = 1;
		RenderWindow window(VideoMode(1376, 768), "SELSOR");
		gameRunning(window, currentLevel);
	}

   return 0;
}
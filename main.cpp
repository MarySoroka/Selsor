#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "view.h"
#include <iostream>
#include "mission.h"
#include "iostream"
#include "level.h"
#include "menu.h"
#include <vector>
#include <list>
#include "TinyXML/tinyxml.h"
#include "Bar.h"
#include "game.h"


void gameRunning(RenderWindow& window, int& numberLevel) {//�-��� ������������� ���� , ���� ��� ����������
	if (startGame(window, numberLevel)) { numberLevel++; gameRunning(window, numberLevel); }//��������� � ������ ������ ������ ����
}


int main(){
	RenderWindow window(VideoMode(1376, 768), "SELSOR");
	int numOfCurrLev = 1;
	menu(window);
	gameRunning(window,numOfCurrLev);
   return 0;
}
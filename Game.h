#ifndef GAME_H
#define GAME_H

#include "GameController.h"

class Game {
public:
	Game();

	void start();

	~Game();
private:
	GameModel* model;
	GameView* view;
	GameController* controller;;
};

#endif
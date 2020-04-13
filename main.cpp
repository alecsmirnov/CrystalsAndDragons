#include "GameController.h"

int main(int argc, char* argv[]) {
	GameModel* model = new GameModel();
	GameView* view = new GameView(model);
	GameController* controller = new GameController(model, view);

	controller->start();

	delete model;
	delete view;
	delete controller;

	return 0;
}
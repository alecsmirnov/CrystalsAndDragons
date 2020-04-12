#include "Game.h"

Game::Game() {
	model = new GameModel();
	view = new GameView(model);
	controller = new GameController(model, view);
}

void Game::start() {
	controller->start();
}

Game::~Game() {
	delete model;
	delete view;
	delete controller;
}
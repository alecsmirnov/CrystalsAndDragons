#include "GameController.h"

#include <cmath>

static constexpr std::uint64_t WAIT_TIME_SEC = 5;

static constexpr std::uint32_t HEALTH_PENALTY = 1;
static constexpr double        LOSE_PERCENT   = 0.1;
static constexpr double        GET_PERCENT    = 0.1;

GameController::GameController(GameModel* model, GameView* view) {
	this->model = model;
	this->view = view;

	executed = true;
}

void GameController::start() {
	view->initInput();
	model->initModel(view->getRoomsCount());

	GameOperation operation;

	do {
		view->update();
		view->input();

		operation = view->getOperation();
		bool threat = view->getThreatStatus();

		if (operation.command != GameCommand::EXIT && executed) {
			if (!threat)
				executeOperation(operation);
			else 
				if (WAIT_TIME_SEC < view->getOperationTime())
					knockBack();
				else
					checkRoll(operation);
		}
	} while (operation.command != GameCommand::EXIT && executed);
}

void GameController::checkRoll(GameOperation operation) {
	auto dice_roll = rollDice();

	switch (dice_roll) {
		case DiceResult::LOSING:  knockBack();		   break;
		case DiceResult::DRAW:    toDrow(operation);   break;
		case DiceResult::VICTORY: complete(operation); break;
	}
}

void GameController::executeOperation(GameOperation operation) {
	switch (operation.command) {
		case GameCommand::NORTH: moveToDirection(CellDirection::NORTH); break;
		case GameCommand::EAST:  moveToDirection(CellDirection::EAST);  break;
		case GameCommand::SOUTH: moveToDirection(CellDirection::SOUTH); break;
		case GameCommand::WEST:  moveToDirection(CellDirection::WEST);  break;
		case GameCommand::GET:   getItem(operation.object);				break;
		case GameCommand::DROP:  dropItem(operation.object);			break;
		case GameCommand::OPEN:  openChest();							break;
		case GameCommand::EAT:	 eatFood(operation.object);				break;
		case GameCommand::FIGHT: fightMonster(operation.object);		break;
		case GameCommand::MAP:   view->displayMaze();					break;
	}
}

void GameController::knockBack() {
	view->displayFightLose();

	auto health = static_cast<std::uint32_t>(std::round(model->getHeroHealth() * LOSE_PERCENT));

	model->setHeroHealth(health);
	moveToDirection(model->getPreviosRoom());
}

void GameController::toDrow(GameOperation operation) {
	view->displayFightDrow();

	auto health = static_cast<std::uint32_t>(std::round(model->getHeroHealth() * LOSE_PERCENT));

	model->setHeroHealth(health);
	executeOperation(operation);
}

void GameController::complete(GameOperation operation) {
	view->displayFightWin();

	executeOperation(operation); 
}

void GameController::moveToDirection(CellDirection direction) {
	switch (direction) {
		case CellDirection::NORTH: model->setHeroX(model->getHeroX() - 1); break;
		case CellDirection::EAST:  model->setHeroY(model->getHeroY() + 1); break;
		case CellDirection::SOUTH: model->setHeroX(model->getHeroX() + 1); break;
		case CellDirection::WEST:  model->setHeroY(model->getHeroY() - 1); break;
	}

	auto health = model->getHeroHealth() - HEALTH_PENALTY;

	if (health)
		model->setHeroHealth(health);
	else {
		close();
		view->displayLose();
	}
}

void GameController::getItem(Object item) {
	auto peek_obj = model->peekRoomObject(item);

	model->pickupHeroItem(peek_obj);
}

void GameController::dropItem(Object item) {
	auto drop_item = model->dropHeroItem(item);

	model->pushRoomObject(drop_item);
}

void GameController::eatFood(Object food) {
	auto used_food = model->dropHeroItem(food);
	used_food = model->peekRoomObject(food);

	auto health = static_cast<std::uint32_t>(std::round(model->getHeroHealth() * GET_PERCENT));

	model->setHeroHealth(health);
}

void GameController::fightMonster(Object monster) {
	model->peekRoomObject(monster);
}

void GameController::openChest() {
	auto items = model->getHeroItems();

	bool found = false;
	for (auto it = items.begin(); it != items.end() && !found; ++it)
		if (it->type == ObjectType::KEY)
			found = true;

	if (found) {
		close();
		view->displayWin();
	}
	else
		view->displayClosedChest();
}

void GameController::close() {
	executed = false;
}
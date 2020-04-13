#include "GameController.h"

#include <cmath>

GameController::GameController(GameModel* model, GameView* view) {
	this->model = model;
	this->view = view;

	executed = true;
}

// Начать игру
void GameController::start() {
	// Получить размер лабиринта и инициализировать его
	view->initInput();
	model->initModel(view->getRoomsCount());

	GameOperation operation;

	do {
		// Считать выбранную команду и выполнить её
		view->update();
		view->input();

		// Получить время ввода команды и статус угрозы
		operation = view->getOperation();
		auto threat = view->getThreatStatus();

		if (operation.command != GameCommand::EXIT && executed) {
			// Если угрозы в комнате нет -- выполнить команду,
			// иначе -- проверяем время ввода команды: откидываем назад или бросаем кости
			if (!threat)
				executeOperation(operation);
			else 
				if (model->getWaitTime() < view->getOperationTime())
					knockBack();
				else
					checkRoll(operation);
		}
	} while (operation.command != GameCommand::EXIT && executed);
}

// Бросить кости в случае встречи с монстром
void GameController::checkRoll(GameOperation operation) {
	auto dice_roll = rollDice();

	switch (dice_roll) {
		case DiceResult::LOSING:  knockBack();		   break;
		case DiceResult::DRAW:    toDrow(operation);   break;
		case DiceResult::VICTORY: complete(operation); break;
	}
}

// Выполнить операцию без угрозы
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

// Откинуть Героя в комнату, откуда он пришёл и вывести сообщение
void GameController::knockBack() {
	view->displayFightLose();

	// Отнять здоровье на указанный процент
	auto health = std::round(model->getHeroHealth() * model->getHealthLosePercent());

	// Переместить в комнату, откуда пришёл Герой
	model->setHeroHealth(health);
	moveToDirection(model->getPreviosRoom());
}

// Сыграть ничью. Отнять Герою здоровье
void GameController::toDrow(GameOperation operation) {
	view->displayFightDrow();

	// Отнять здоровье на указанный процент
	auto health = std::round(model->getHeroHealth() * model->getHealthLosePercent());

	// Выполнить команду
	model->setHeroHealth(health);
	executeOperation(operation);
}

// Выполнить команду ничего не потеряв
void GameController::complete(GameOperation operation) {
	view->displayFightWin();

	// Выполнить команду
	executeOperation(operation); 
}

// Пойти в указанном направлении
void GameController::moveToDirection(CellDirection direction) {
	switch (direction) {
		case CellDirection::NORTH: model->setHeroX(model->getHeroX() - 1); break;
		case CellDirection::EAST:  model->setHeroY(model->getHeroY() + 1); break;
		case CellDirection::SOUTH: model->setHeroX(model->getHeroX() + 1); break;
		case CellDirection::WEST:  model->setHeroY(model->getHeroY() - 1); break;
	}

	// Отнять здоровье на величину уменьшения при перемещении
	auto health = model->getHeroHealth() - model->getHealthPenalty();

	// Проверить здоровье после перемещения
	if (0 < health)
		model->setHeroHealth(health);
	else {
		close();
		view->displayLose();
	}
}

// Взять предмет из комнаты
void GameController::getItem(Object item) {
	// Взять предмет из комнаты
	auto peek_obj = model->peekRoomObject(item);

	// Передать предмет Герою
	model->pickupHeroItem(peek_obj);
}

// Выкинуть предмет из инвентаря
void GameController::dropItem(Object item) {
	// Взять предмет у игрока
	auto drop_item = model->dropHeroItem(item);

	// Положить предмет в комнату
	model->pushRoomObject(drop_item);
}

// Съесть пищу и восполнить здоровье
void GameController::eatFood(Object food) {
	// Взять пищу из инвентаря
	auto used_food = model->dropHeroItem(food);
	// Взять пищу из комнаты
	used_food = model->peekRoomObject(food);

	// Увеличить здоровье на процент увеличения здоровья
	auto health = std::round(model->getHeroHealth() * model->getHealthLiftPercent());

	model->setHeroHealth(health);
}

// Сразиться с монтром
void GameController::fightMonster(Object monster) {
	// Удалить монстра из комнаты
	model->peekRoomObject(monster);
}

// Открыть сундук
void GameController::openChest() {
	auto items = model->getHeroItems();

	// Проверить наличие ключа в инвентаре
	bool found = false;
	for (auto it = items.begin(); it != items.end() && !found; ++it)
		if (it->type == ObjectType::KEY)
			found = true;

	// Если ключ найден -- закончить игру, иначе -- вывести сообщение
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
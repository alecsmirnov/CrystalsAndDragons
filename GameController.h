#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "GameView.h"

enum class DiceResult {
	LOSING,
	DRAW,
	VICTORY,
	SIZE
};

class GameController {
public:
	GameController(GameModel* model, GameView* view);

	void start();

private:
	void executeOperation(GameOperation operation);

	void knockBack();
	void toDrow(GameOperation operation);
	void complete(GameOperation operation);
	void checkRoll(GameOperation operation);

	void moveToDirection(CellDirection direction);
	void getItem(Object item);
	void dropItem(Object item);
	void eatFood(Object food);
	void fightMonster(Object monster);
	void openChest();

	void close();

private:
	bool executed;

	GameModel* model;
	GameView* view;
};

static inline DiceResult rollDice() {
	return static_cast<DiceResult>(rand(0, static_cast<std::uint16_t>(DiceResult::SIZE)));
}

#endif
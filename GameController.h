#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "GameView.h"

// Результат броска игральных костей в случае встречи с монстром
enum class DiceResult {
	LOSING,				// Победа
	DRAW,				// Ничья
	VICTORY,			// Поражения
	SIZE
};

class GameController {
public:
	GameController(GameModel* model, GameView* view);

	// Начать игру
	void start();

private:
	// Выполнить переданную операцию
	void executeOperation(GameOperation operation);

	// Откинуть Героя на шаг назад в случае проигрыша
	void knockBack();
	// Сыграть ничью. Отнять Герою здоровье
	void toDrow(GameOperation operation);
	// Выполнить команду ничего не потеряв, при встрече с монтром
	void complete(GameOperation operation);
	// Бросить игральные кости для определения исхода встречи с монтром
	void checkRoll(GameOperation operation);

	// Пойти в указанном направлении
	void moveToDirection(CellDirection direction);
	// Взять предмет из комнаты
	void getItem(Object item);
	// Выкинуть предмет из инвентаря
	void dropItem(Object item);
	// Съесть пищу и восполнить здоровье
	void eatFood(Object food);
	// Сразиться с монтром
	void fightMonster(Object monster);
	// Открыть сундук
	void openChest();

	void close();

private:
	bool executed;

	GameModel* model;
	GameView* view;
};

// Бросить ингральные кости
static inline DiceResult rollDice() {
	return static_cast<DiceResult>(rand(0, static_cast<std::uint16_t>(DiceResult::SIZE)));
}

#endif
#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "GameModel.h"

// Перечень команд для исполнения
enum class GameCommand {
	NORTH,					// Переместиться на N
	EAST,					// Переместиться на E
	SOUTH,					// Переместиться на S
	WEST,					// Переместиться на W
	GET,					// Взять предмет
	DROP,					// Выкинуть предмет
	OPEN,					// Открыть сундук
	EAT,					// Съесть пищу
	FIGHT,					// Сразиться с монстром (если есть меч)
	MAP,					// Карта
	EXIT					// Выход из игры
};

// Операция -- выполнение команды над объектом
struct GameOperation {
	GameCommand command;	// Команда
	Object object;			// Объект
};

class GameView {
public:
	GameView(GameModel* model);

	// Ввод кол-ва комнат в лабиринте
	void initInput();
	// Выбор команд из списка
	void input();

	// Вывод/обновление информации на экране
	void update();

	// Вывод предупреждения о неправильно заданном кол-ве комнат
	void displayRoomsCountWarnig() const;
	// Вывод предупреждения о неправильно введённой команде
	void displayCommandWarnig() const;
	// Вывод предупреждения о попытке открыть сундук без ключа
	void displayClosedChest() const;
	// Вывод сообщения о победе (открытии сундука)
	void displayWin() const;
	// Вывод сообщения о поражении (смерти)
	void displayLose() const;
	// Вывод лабиринта
	void displayMaze() const;

	// Вывод сообщения о неудачной встрече с монстром
	void displayFightLose() const;
	// Вывод сообщения о ничьей
	void displayFightDrow() const;
	// Вывод сообщения об удачной встрече с монстром
	void displayFightWin() const;

	// Получить количество введённых комнат 
	std::uint32_t getRoomsCount() const;
	// Получить введённую операцияю
	GameOperation getOperation() const;

	// Получить время введённой операции
	std::uint64_t getOperationTime() const;
	// Получить статус угрозы (есть или нет монстр в комнате)
	bool getThreatStatus() const;

	~GameView();

private:
	// Ввод команты с выводом её времени
	static std::uint64_t inputCommandWithTime(std::uint32_t &command_num);
	// Получить префикс команты (move to, get, drop, fight, ...)
	static std::string getCommandPrefix(GameCommand command, bool directions);
	// Получить оформление объекта (цвет, преписка для денег)
	static std::string getObjectEdging(Object object);

	void clearCommandsList();

private:
	std::uint32_t rooms_count;							// Кол-во комнат
	GameOperation operation;							// Операция над объектом

	std::uint64_t input_time;							// Время ввода
	bool threat;										// Угроза в комнате

	std::vector<GameOperation> operations_list;			// Перечень доступных операций для выбора

	GameModel* model;
};

// Окрасить текст в красный цвет
static inline std::string redText(std::string text) {
	return "\x1B[31m" + text + "\033[0m";
}

// Окрасить текст в зелёный цвет
static inline std::string greenText(std::string text) {
	return "\x1B[32m" + text + "\033[0m";
}

// Окрасить текст в жёлтый цвет
static inline std::string yellowText(std::string text) {
	return "\x1B[33m" + text + "\033[0m";
}

#endif
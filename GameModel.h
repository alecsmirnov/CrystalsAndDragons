#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "Hero.h"
#include "Maze.h"

// Вспомогательная структура для объединения координат
struct Pair {
	std::uint16_t x;
	std::uint16_t y;
};

class GameModel {
public:
	GameModel() = default;

	// Инициализация данных (Кол-во комнат, положение игровых обектов, ...)
	void initModel(std::uint32_t rooms_count);

	// Добавить игровой объект в лабиринт
	void addRoomObject(std::uint16_t x, std::uint16_t y, Object object);

	// Изменине/получение положения Героя
	void setHeroX(std::uint16_t x);
	std::uint16_t getHeroX() const;

	void setHeroY(std::uint16_t y);
	std::uint16_t getHeroY() const;

	// Изменение/получения здоровья Героя
	void setHeroHealth(double health);
	double getHeroHealth() const;

	// Задать/получить инвентарь Героя
	void setHeroItems(const std::vector<Object> &items);
	std::vector<Object> getHeroItems() const;
	// Взять/выкинуть предмет из инвентаря
	void pickupHeroItem(Object item);
	Object dropHeroItem(Object item);

	// Получить все объекты в комнате
	std::vector<Object> getRoomObjects() const;
	// Положить объект в комнату
	void pushRoomObject(Object object);
	// Взять объект из комнаты
	Object peekRoomObject(Object object);

	// Получить список всех дверей комнаты
	std::vector<CellDirection> getRoomDoors() const;

	// Получить тип комнаты (светлая, тёмная)
	CellType getRoomType() const;

	// Получить любую комнату по координам
	Cell getRoom(std::uint16_t x, std::uint16_t y) const;

	// Получить предыдущую комнату в которой был Герой
	CellDirection getPreviosRoom() const;

	// Получить позицию ключа
	std::uint16_t getKeyX() const;
	std::uint16_t getKeyY() const;

	// получить позицию сундука
	std::uint16_t getChestX() const;
	std::uint16_t getChestY() const;

	// Получить размеры лабиринта
	std::uint16_t getMazeWidth() const;
	std::uint16_t getMazeHeight() const;

	// Получить минимально-возможное количество комнат
	std::uint32_t getRoomsMin() const;
	// Получить время ожидания ответа в случае встречи с монстром
	std::uint64_t getWaitTime() const;

	// Получить величину уменьшения здоровья при перемещении
	double getHealthPenalty() const;
	// Получить процент уменьшение здоровья при атаках монстра
	double getHealthLosePercent() const;
	// Получить процент увеличения здоровья после приёма пищи
	double getHealthLiftPercent() const;

private:
	// Функция взятия золота (группировка золота в один предмет)
	void pickupHeroGold(Object item);
	// Получить ширину и высоту массива по количеству комнат
	static void getMazeSize(std::uint32_t rooms_count, std::uint16_t &width, std::uint16_t &height);

private:
	CellDirection previos_room;		// Направление предыдущей комнаты

	Pair key_position;				// Положение ключа
	Pair chest_position;			// Положение сундука

	Hero hero;						// Герой
	Maze maze;						// Лабиринт
};

#endif
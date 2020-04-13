#ifndef CELL_H
#define CELL_H

#include <cstdint>
#include <string>
#include <vector>

#include "Object.h"

// Направление дверей в комнате
enum class CellDirection {
	NORTH = 0,
	EAST  = 6,
	SOUTH = 4,
	WEST  = 2,
	SIZE  = 4
};

// Тип комнаты
enum class CellType :bool {
	DARK,			// Тёмная
	LIGHT,			// Светлая
	NONE			// Нет типа
};

// Комната
class Cell {
public:
	Cell();
	Cell(uint16_t x, uint16_t y, CellType type = CellType::LIGHT);

	// Задать координаты комнаты
	void setCoords(uint16_t x, uint16_t y);

	// Задать/получить координат по оси х
	void setX(uint16_t x);
	uint16_t getX() const;

	// Задать/получить координат по оси y
	void setY(uint16_t y);
	uint16_t getY() const;

	// Задать/получить тип комнаты
	void setType(CellType type);
	CellType getType() const;

	// Добавить/получить объект комнаты
	void pushObject(Object item);
	Object peekObject(Object item);

	// Получить все объекты комнаты
	std::vector<Object> getObjects() const;
	// Очистить объекты комнаты
	void clearObjects();

	// Открыть указанное направление (дверь) комнаты
	void open(CellDirection direction);
	// Закрыть указанное направление (дверь) комнаты
	void close(CellDirection direction);

	// Преверить открыто ли указанное направление (дверь)
	bool isOpened(CellDirection direction) const;

	// Проверить посущена ли комната во время генерации
	bool isVisited() const;

	~Cell();

private:
	// Закрыть/открыть указанное направление (дверь)
	void changeDirection(CellDirection direction, bool status);

private:
	std::uint16_t x;
	std::uint16_t y;

	bool north;
	bool east;
	bool south;
	bool west;

	CellType type;

	std::vector<Object> objects;
};

// Получить направление между двумя точками
CellDirection getDirection(std::uint16_t x1, std::uint16_t y1, 
						   std::uint16_t x2, std::uint16_t y2);
#endif
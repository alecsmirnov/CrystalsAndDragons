#ifndef MAZE_H
#define MAZE_H

#include "Cell.h"

class Maze {
public:
	Maze();
	Maze(std::uint16_t width, std::uint16_t height);

	// Инициализация лабиринта
	void init(std::uint16_t width, std::uint16_t height);

	// Получить ширину/высоту лабиринта
	std::uint16_t getWidth() const;
	std::uint16_t getHeight() const;
	
	// Получить комнату по координатам
	Cell getCell(std::uint16_t x, std::uint16_t y) const;

	// Добавить объект в комнату
	void pushCellObject(std::uint16_t x, std::uint16_t y, Object object);
	// Взять объект из комнаты
	Object peekCellObject(std::uint16_t x, std::uint16_t y, Object object) const;

	// Получить объекты комнаты
	std::vector<Object> getCellObjects(std::uint16_t x, std::uint16_t y) const;
	// Очистить объекты комнаты
	void clearCellObjects(std::uint16_t x, std::uint16_t y);

	// Установить тип комнаты
	void setCellType(std::uint16_t x, std::uint16_t y, CellType type);
	// Получить тип комнаты
	CellType getCellType(std::uint16_t x, std::uint16_t y) const;

	// Сгенерировать лабиринт
	void generate();
	// Очистить лабиринт
	void clear();

	~Maze();

private:
	std::uint16_t width;
	std::uint16_t height;

	Cell** field;
};

// Получение случайного числа в диапазоне
std::uint16_t rand(std::uint16_t beg, std::uint16_t end);

#endif
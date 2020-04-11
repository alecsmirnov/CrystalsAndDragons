#ifndef CELL_H
#define CELL_H

#include <cstdint>
#include <string>
#include <vector>

#include "Object.h"

enum class CellDirection {
	NORTH = 0,
	EAST  = 6,
	SOUTH = 4,
	WEST  = 2,
	SIZE  = 4
};

enum class CellType :bool {
	DARK,
	LIGHT,
	NONE
};

class Cell {
public:
	Cell();
	Cell(uint16_t x, uint16_t y, CellType type = CellType::LIGHT);

	void setCoords(uint16_t x, uint16_t y);

	void setX(uint16_t x);
	uint16_t getX() const;

	void setY(uint16_t y);
	uint16_t getY() const;

	void setType(CellType type);
	CellType getType() const;

	void pushObject(Object item);
	Object peekObject(Object item);

	std::vector<Object> getObjects() const;
	void clearObjects();

	void open(CellDirection direction);
	void close(CellDirection direction);

	bool isOpened(CellDirection direction) const;

	bool isVisited() const;

	~Cell();

private:
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

CellDirection getDirection(std::uint16_t x1, std::uint16_t y1, 
						   std::uint16_t x2, std::uint16_t y2);
#endif
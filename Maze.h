#ifndef MAZE_H
#define MAZE_H

#include "Cell.h"

class Maze {
public:
	Maze();
	Maze(std::uint16_t width, std::uint16_t height);

	void init(std::uint16_t width, std::uint16_t height);

	std::uint16_t getWidth() const;
	std::uint16_t getHeight() const;

	void setCell(Cell cell);
	Cell getCell(std::uint16_t x, std::uint16_t y) const;

	void pushCellObject(std::uint16_t x, std::uint16_t y, Object object);
	Object peekCellObject(std::uint16_t x, std::uint16_t y, Object object) const;

	std::vector<Object> getCellObjects(std::uint16_t x, std::uint16_t y) const;
	void clearCellObjects(std::uint16_t x, std::uint16_t y);

	void setCellType(std::uint16_t x, std::uint16_t y, CellType type);
	CellType getCellType(std::uint16_t x, std::uint16_t y) const;

	void generate();
	void clear();

	~Maze();

private:
	std::uint16_t width;
	std::uint16_t height;

	Cell** field;
};

std::uint16_t rand(std::uint16_t beg, std::uint16_t end);

#endif
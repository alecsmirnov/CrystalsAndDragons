#include "Cell.h"

#include <cmath>

static constexpr double        PI         = 3.141592653589793238463;
static constexpr double        ANGLE_45   = PI / 4;
static constexpr std::uint16_t DIRECTIONS = 8;

Cell::Cell(): Cell(0, 0) {}

Cell::Cell(uint16_t x, uint16_t y, CellType type) {
	this->north = false;
	this->east  = false;
	this->south = false;
	this->west  = false;

	this->x = x;
	this->y = y;

	this->type = type;
}

void Cell::setCoords(uint16_t x, uint16_t y) {
	this->x = x;
	this->y = y;
}

void Cell::setX(uint16_t x) {
	this->x = x;
}

uint16_t Cell::getX() const {
	return x;
}

void Cell::setY(uint16_t y) {
	this->y = y;
}

uint16_t Cell::getY() const {
	return y;
}

void Cell::setType(CellType type) {
	this->type = type;
}

CellType Cell::getType() const {
	return type;
}

void Cell::pushObject(Object object) {
	objects.push_back(object);
}

Object Cell::peekObject(Object object) {
	Object peek_object;

	auto peek_object_pos = std::find(objects.begin(), objects.end(), object);

	if (peek_object_pos != objects.end()) {
		objects.erase(peek_object_pos);
		peek_object = object;
	}

	return peek_object;
}

std::vector<Object> Cell::getObjects() const {
	return objects;
}

void Cell::clearObjects() {
	std::vector<Object>().swap(objects);
}

void Cell::open(CellDirection direction) {
	changeDirection(direction, true);
}

void Cell::close(CellDirection direction) {
	changeDirection(direction, false);
}

bool Cell::isOpened(CellDirection direction) const {
	bool direction_status = false;

	switch (direction) {
		case CellDirection::NORTH: direction_status = north; break;
		case CellDirection::EAST:  direction_status = east;  break;
		case CellDirection::SOUTH: direction_status = south; break;
		case CellDirection::WEST:  direction_status = west;  break;
	}

	return direction_status;
}

bool Cell::isVisited() const {
	return north || east || south || west;
}

Cell::~Cell() {
	clearObjects();
}

void Cell::changeDirection(CellDirection direction, bool status) {
	switch (direction) {
		case CellDirection::NORTH: north = status; break;
		case CellDirection::EAST:  east  = status; break;
		case CellDirection::SOUTH: south = status; break;
		case CellDirection::WEST:  west  = status; break;
	}
}

CellDirection getDirection(std::uint16_t x1, std::uint16_t y1,
						   std::uint16_t x2, std::uint16_t y2) {
	std::int32_t y_difference = y2 - y1;
	std::int32_t x_difference = x2 - x1;

	double angle = (std::atan2(y_difference, x_difference) + PI) / ANGLE_45;
	std::uint16_t direction = static_cast<std::uint16_t>(angle) % DIRECTIONS;

	return static_cast<CellDirection>(direction);
}
#include "Cell.h"

#include <cmath>

static constexpr double PI = 3.141592653589793238463;
// Угол 45 градусов для вычисления направления по тангенсу угла наклона
static constexpr double ANGLE_45 = PI / 4;
// Количество направлений для расчёта
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

// Задать координаты комнаты
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

// Добавить объект в комнату
void Cell::pushObject(Object object) {
	objects.push_back(object);
}

// Взять объект из комнаты
Object Cell::peekObject(Object object) {
	Object peek_object;

	// Поиск объекта в комнате
	auto peek_object_pos = std::find(objects.begin(), objects.end(), object);

	if (peek_object_pos != objects.end()) {
		objects.erase(peek_object_pos);
		peek_object = object;
	}

	return peek_object;
}

// Получить все объекты комнаты
std::vector<Object> Cell::getObjects() const {
	return objects;
}

// Очистить объекты комнаты
void Cell::clearObjects() {
	std::vector<Object>().swap(objects);
}

// Открыть указанное направление (дверь) комнаты
void Cell::open(CellDirection direction) {
	changeDirection(direction, true);
}

// Закрыть указанное направление (дверь) комнаты
void Cell::close(CellDirection direction) {
	changeDirection(direction, false);
}

// Преверить открыто ли указанное направление (дверь)
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

// Проверить посущена ли комната во время генерации
bool Cell::isVisited() const {
	return north || east || south || west;
}

Cell::~Cell() {
	clearObjects();
}

// Закрыть/открыть указанное направление (дверь)
void Cell::changeDirection(CellDirection direction, bool status) {
	switch (direction) {
		case CellDirection::NORTH: north = status; break;
		case CellDirection::EAST:  east  = status; break;
		case CellDirection::SOUTH: south = status; break;
		case CellDirection::WEST:  west  = status; break;
	}
}

// Получить направление между двумя точками
CellDirection getDirection(std::uint16_t x1, std::uint16_t y1,
						   std::uint16_t x2, std::uint16_t y2) {
	std::int32_t y_difference = y2 - y1;
	std::int32_t x_difference = x2 - x1;

	// Вычислить тангенс угла наклона
	double angle = (std::atan2(y_difference, x_difference) + PI) / ANGLE_45;
	// Перевод в область окружности
	std::uint16_t direction = static_cast<std::uint16_t>(angle) % DIRECTIONS;

	return static_cast<CellDirection>(direction);
}
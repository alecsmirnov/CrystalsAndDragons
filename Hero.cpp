#include "Hero.h"

Hero::Hero(): x(0), y(0), health(0) {}

Hero::Hero(std::uint16_t x, std::uint16_t y, double health) {
	this->x = x;
	this->y = y;

	this->health = health;
}

// Задать положение Героя по оси х
void Hero::setX(std::uint16_t x) {
	this->x = x;
}

// Получить положение Героя по оси х
std::uint16_t Hero::getX() const {
	return x;
}

// Задать положение Героя по оси y
void Hero::setY(std::uint16_t y) {
	this->y = y;
}

// Получить положение Героя по оси y
std::uint16_t Hero::getY() const {
	return y;
}

// Задать здоровье Героя
void Hero::setHelth(double health) {
	this->health = health;
}

// Получить здоровье Героя
double Hero::getHealth() const {
	return health;
}

// Положить предмет в инвентарь
void Hero::pickupItem(Object item) {
	items.push_back(item);
}

// Выкинуть предмет из инвентаря
Object Hero::dropItem(Object item) {
	Object drop_item;

	// Поиск указанного предмета
	auto drop_item_pos = std::find(items.begin(), items.end(), item);

	if (drop_item_pos != items.end()) {
		// Если предмет найден -- выкидываем из инвентаря
		items.erase(drop_item_pos);
		drop_item = item;
	}

	return drop_item;
}

// Задать инвентарь Героя
void Hero::setItems(const std::vector<Object>& items) {
	this->items = items;
}

// Получить инвентарь Героя
std::vector<Object> Hero::getItems() const {
	return items;
}

void Hero::clearItems() {
	std::vector<Object>().swap(items);
}

Hero::~Hero() {
	clearItems();
}
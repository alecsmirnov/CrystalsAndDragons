#include "Hero.h"

Hero::Hero(): x(0), y(0), health(0) {}

Hero::Hero(std::uint16_t x, std::uint16_t y, double health) {
	this->x = x;
	this->y = y;

	this->health = health;
}

void Hero::setX(std::uint16_t x) {
	this->x = x;
}

std::uint16_t Hero::getX() const {
	return x;
}

void Hero::setY(std::uint16_t y) {
	this->y = y;
}

std::uint16_t Hero::getY() const {
	return y;
}

void Hero::setHelth(double health) {
	this->health = health;
}

double Hero::getHealth() const {
	return health;
}

void Hero::pickupItem(Object item) {
	items.push_back(item);
}

Object Hero::dropItem(Object item) {
	Object drop_item = {"", ObjectType::NONE};

	auto drop_item_pos = std::find(items.begin(), items.end(), item);

	if (drop_item_pos != items.end()) {
		items.erase(drop_item_pos);
		drop_item = item;
	}

	return drop_item;
}

void Hero::setItems(const std::vector<Object>& items) {
	this->items = items;
}

std::vector<Object> Hero::getItems() const {
	return items;
}

void Hero::clearInventory() {
	std::vector<Object>().swap(items);
}

Hero::~Hero() {
	clearInventory();
}
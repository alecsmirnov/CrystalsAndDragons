#ifndef HERO_H
#define HERO_H

#include <cstdint>
#include <string>
#include <vector>

#include "Object.h"

class Hero {
public:
	Hero();
	Hero(std::uint16_t x, std::uint16_t y, std::uint32_t health);

	void setX(std::uint16_t x);
	std::uint16_t getX() const;

	void setY(std::uint16_t y);
	std::uint16_t getY() const;

	void setHelth(std::uint32_t health);
	std::uint32_t getHealth() const;

	void pickupItem(Object item);
	Object dropItem(Object item);

	std::vector<Object> getItems() const;
	void clearInventory();

	~Hero();

private:
	std::uint16_t x;
	std::uint16_t y;

	std::uint32_t health;

	std::vector<Object> items;
};

#endif
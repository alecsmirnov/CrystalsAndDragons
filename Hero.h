#ifndef HERO_H
#define HERO_H

#include <cstdint>
#include <string>
#include <vector>

#include "Object.h"

// Объект Героя
class Hero {
public:
	Hero();
	Hero(std::uint16_t x, std::uint16_t y, double health);
	
	// Изменине/получение положения Героя
	void setX(std::uint16_t x);
	std::uint16_t getX() const;

	void setY(std::uint16_t y);
	std::uint16_t getY() const;

	// Изменение/получения здоровья Героя
	void setHelth(double health);
	double getHealth() const;

	// Положить предмет в инвентарь
	void pickupItem(Object item);
	// Выкинуть предмет из инвентаря
	Object dropItem(Object item);

	// Задать/получить/очистить инвентарь Героя
	void setItems(const std::vector<Object>& items);
	std::vector<Object> getItems() const;
	void clearItems();

	~Hero();

private:
	std::uint16_t x;		// Положение Героя по оси х,
	std::uint16_t y;		// по оси y

	double health;			// Здоровье Героя

	std::vector<Object> items;
};

#endif
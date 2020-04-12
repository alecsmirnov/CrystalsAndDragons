#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "Hero.h"
#include "Maze.h"

struct Pair {
	std::uint16_t x;
	std::uint16_t y;
};

class GameModel {
public:
	GameModel() = default;

	void initModel(std::uint32_t rooms_count);

	void addRoomObject(std::uint16_t x, std::uint16_t y, Object object);

	void setHeroX(std::uint16_t x);
	std::uint16_t getHeroX() const;

	void setHeroY(std::uint16_t y);
	std::uint16_t getHeroY() const;

	void setHeroHealth(std::uint32_t health);
	std::uint32_t getHeroHealth() const;

	std::vector<Object> getHeroItems() const;
	void pickupHeroItem(Object item);
	Object dropHeroItem(Object item);

	std::vector<Object> getRoomObjects() const;
	void pushRoomObject(Object object);
	Object peekRoomObject(Object object);

	std::vector<CellDirection> getRoomDoors() const;

	void setRoomType(CellType type);
	CellType getRoomType() const;

	Cell getRoom(std::uint16_t x, std::uint16_t y) const;

	CellDirection getPreviosRoom() const;

	void setKeyPosition(std::uint16_t x, std::uint16_t y);
	std::uint16_t getKeyX() const;
	std::uint16_t getKeyY() const;

	void setChestPosition(std::uint16_t x, std::uint16_t y);
	std::uint16_t getChestX() const;
	std::uint16_t getChestY() const;

	std::uint16_t getMazeWidth() const;
	std::uint16_t getMazeHeight() const;

private:
	static void getMazeSize(std::uint32_t rooms_count, std::uint16_t &width, std::uint16_t &height);

private:
	CellDirection previos_room;

	Pair key_position;
	Pair chest_position;

	Hero hero;
	Maze maze;
};

#endif
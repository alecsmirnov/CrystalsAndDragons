#ifndef GAME_H
#define GAME_H

#include "Hero.h"
#include "Maze.h"
#include "Observer.h"

class GameModel : public Supervised {
public:
	GameModel() = default;

	void initHero(std::uint16_t x, std::uint16_t y, std::uint32_t health);
	void initMaze(std::uint16_t width, std::uint16_t height);

	void addRoomObject(std::uint16_t x, std::uint16_t y, Object object);

	void setHeroHealth(std::uint32_t health);
	std::uint32_t getHeroHealth() const;

	void setHeroX(std::uint16_t x);
	std::uint16_t getHeroX() const;

	void setHeroY(std::uint16_t y);
	std::uint16_t getHeroY() const;

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

	void setKeyPosition(std::uint16_t x, std::uint16_t y);
	std::uint16_t getKeyX() const;
	std::uint16_t getKeyY() const;

	void setChestPosition(std::uint16_t x, std::uint16_t y);
	std::uint16_t getChestX() const;
	std::uint16_t getChestY() const;

	std::uint16_t getMazeWidth() const;
	std::uint16_t getMazeHeight() const;

private:
	std::pair<std::uint16_t, std::uint16_t> key_position;
	std::pair<std::uint16_t, std::uint16_t> chest_position;

	Hero hero;
	Maze maze;
};

// -------------------------------------------------------

enum class GameCommand {
	NORTH,
	EAST,
	SOUTH,
	WEST,
	GET,
	DROP,
	OPEN,
	EAT,
	FIGHT,
	MAP,
	EXIT
};

struct GameOperation {
	GameCommand command;
	Object object;
};

class GameView : public Observer {
public:
	GameView(GameModel* model);

	void init();
	void update();
	void input();

	void displayCommandWarnig() const;
	void displayClosedChest() const;
	void displayWin() const;
	void displayLose() const;

	void displayMaze() const;

	std::uint32_t getRoomsCount() const;
	GameOperation getOperation() const;

	~GameView();

private:
	void clearCommandsList();

private:
	std::uint32_t rooms_count;
	GameOperation operation;

	std::vector<GameOperation> operations_list;

	GameModel* model;
};

// --------------------------------------------------------

class GameController {
public:
	GameController(GameModel* model, GameView* view);

	void start();

private:
	void moveToDirection(CellDirection direction);
	
	void getItem(Object item);
	void dropItem(Object item);

	void openChest();

	void close();

private:
	bool executed;

	GameModel* model;
	GameView*  view;
};

#endif
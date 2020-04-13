#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "GameModel.h"

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

class GameView {
public:
	GameView(GameModel* model);

	void initInput();
	void input();

	void update();

	void displayRoomsCountWarnig() const;
	void displayCommandWarnig() const;
	void displayClosedChest() const;
	void displayWin() const;
	void displayLose() const;
	void displayMaze() const;

	void displayFightLose() const;
	void displayFightDrow() const;
	void displayFightWin() const;

	std::uint32_t getRoomsCount() const;
	GameOperation getOperation() const;

	std::uint64_t getOperationTime() const;
	bool getThreatStatus() const;

	~GameView();

private:
	static std::uint64_t inputCommandWithTime(std::uint32_t &command_num);
	static std::string getCommandPrefix(GameCommand command, bool directions);

	void clearCommandsList();

private:
	std::uint32_t rooms_count;
	GameOperation operation;

	std::uint64_t input_time;
	bool threat;

	std::vector<GameOperation> operations_list;

	GameModel* model;
};

#endif
#include "Game.h"

#include <iostream>
#include <algorithm>
#include <map>
#include <cctype>

// ------------------- MODEL ---------------------

void GameModel::initHero(std::uint16_t x, std::uint16_t y, std::uint32_t health) {
	hero.setX(x);
	hero.setY(y);
	hero.setHelth(health);
}

void GameModel::initMaze(std::uint16_t width, std::uint16_t height) {
	maze.init(width, height);
	maze.generate();

	std::uint16_t rand_x = rand(0, maze.getWidth() - 1);
	std::uint16_t rand_y = rand(0, maze.getHeight() - 1);

	setKeyPosition(rand_x, rand_y);
	maze.pushCellObject(rand_x, rand_y, {"key", ObjectType::ITEM});

	rand_x = rand(0, maze.getWidth() - 1);
	rand_y = rand(0, maze.getHeight() - 1);

	setChestPosition(rand_x, rand_y);
	maze.pushCellObject(rand_x, rand_y, {"chest", ObjectType::CHEST});

	rand_x = rand(0, maze.getWidth() - 1);
	rand_y = rand(0, maze.getHeight() - 1);

	maze.pushCellObject(rand_x, rand_y, {"kobylianskiy", ObjectType::MONSTER});

	rand_x = rand(0, maze.getWidth() - 1);
	rand_y = rand(0, maze.getHeight() - 1);

	maze.setCellType(rand_x, rand_y, CellType::DARK);

	rand_x = rand(0, maze.getWidth() - 1);
	rand_y = rand(0, maze.getHeight() - 1);

	maze.pushCellObject(rand_x, rand_y, {"torchlight", ObjectType::TORCH});

	rand_x = rand(0, maze.getWidth() - 1);
	rand_y = rand(0, maze.getHeight() - 1);

	maze.pushCellObject(rand_x, rand_y, {"sword", ObjectType::WEAPON});
}

void GameModel::addRoomObject(std::uint16_t x, std::uint16_t y, Object item) {
	maze.pushCellObject(x, y, item);
}

void GameModel::setHeroHealth(std::uint32_t health) {
	hero.setHelth(health);
}

std::uint32_t GameModel::getHeroHealth() const {
	return hero.getHealth();
}

void GameModel::setHeroX(std::uint16_t x) {
	hero.setX(x);

	notify();
}

std::uint16_t GameModel::getHeroX() const {
	return hero.getX();
}

void GameModel::setHeroY(std::uint16_t y) {
	hero.setY(y);

	notify();
}

std::uint16_t GameModel::getHeroY() const {
	return hero.getY();
}

std::vector<Object> GameModel::getHeroItems() const {
	return hero.getItems();
}

void GameModel::pickupHeroItem(Object item) {
	hero.pickupItem(item);

	notify();
}

Object GameModel::dropHeroItem(Object item) {
	return hero.dropItem(item);
}

std::vector<Object> GameModel::getRoomObjects() const {
	return maze.getCellObjects(hero.getX(), hero.getY());
}

void GameModel::pushRoomObject(Object item) {
	maze.pushCellObject(hero.getX(), hero.getY(), item);

	notify();
}

Object GameModel::peekRoomObject(Object item) {
	return maze.peekCellObject(hero.getX(), hero.getY(), item);
}

std::vector<CellDirection> GameModel::getRoomDoors() const {
	std::vector<CellDirection> room_doors;

	if (maze.getCell(hero.getX(), hero.getY()).isOpened(CellDirection::NORTH))
		room_doors.push_back(CellDirection::NORTH);

	if (maze.getCell(hero.getX(), hero.getY()).isOpened(CellDirection::EAST))
		room_doors.push_back(CellDirection::EAST);

	if (maze.getCell(hero.getX(), hero.getY()).isOpened(CellDirection::SOUTH))
		room_doors.push_back(CellDirection::SOUTH);

	if (maze.getCell(hero.getX(), hero.getY()).isOpened(CellDirection::WEST))
		room_doors.push_back(CellDirection::WEST);

	return room_doors;
}

void GameModel::setRoomType(CellType type) {
	maze.setCellType(hero.getX(), hero.getY(), type);
}

CellType GameModel::getRoomType() const {
	return maze.getCellType(hero.getX(), hero.getY());
}

Cell GameModel::getRoom(std::uint16_t x, std::uint16_t y) const {
	return maze.getCell(x, y);
}

void GameModel::setKeyPosition(std::uint16_t x, std::uint16_t y) {
	key_position = {x, y};
}

std::uint16_t GameModel::getKeyX() const {
	return key_position.first;
}

std::uint16_t GameModel::getKeyY() const {
	return key_position.second;
}

void GameModel::setChestPosition(std::uint16_t x, std::uint16_t y) {
	chest_position = {x, y};
}

std::uint16_t GameModel::getChestX() const {
	return chest_position.first;
}

std::uint16_t GameModel::getChestY() const {
	return chest_position.second;
}

std::uint16_t GameModel::getMazeWidth() const {
	return maze.getWidth();
}

std::uint16_t GameModel::getMazeHeight() const {
	return maze.getHeight();
}

// ------------------- VIEW ---------------------

GameView::GameView(GameModel* model) {
	this->model = model;
	this->model->addObserver(this);

	this->rooms_count = 0;
}

void GameView::init() {
	std::cout << "Enter the number of rooms:" << std::endl;
	std::cin >> rooms_count;
}

void GameView::update() {
	clearCommandsList();

	std::uint16_t x = model->getHeroX();
	std::uint16_t y = model->getHeroY();

	auto room_type = model->getRoomType();

	if (room_type == CellType::LIGHT)
		std::cout << "You are in the room [" << x << ", " << y << "]. ";
	else
		std::cout << "Can't see anything in this dark place! ";

	auto doors = model->getRoomDoors();

	if (!doors.empty()) {
		std::cout << "There are " << doors.size() << " doors: ";

		std::string str_dir;
		for (auto dir = doors.begin(); dir != doors.end() - 1; ++dir) {
			str_dir = dirToString(*dir);
			commands_list.push_back({str_dir, ObjectType::NONE});
			std::cout << str_dir << ", ";
		}

		str_dir = dirToString(doors.back());
		commands_list.push_back({str_dir, ObjectType::NONE});
		std::cout << str_dir << "." << std::endl;
	}

	if (room_type == CellType::LIGHT) {
		auto hero_items = model->getHeroItems();
		bool armed = false;

		if (!hero_items.empty())
			for (auto item : hero_items) {
				switch (item.type) {
					case ObjectType::ITEM:
					case ObjectType::WEAPON:
					case ObjectType::TORCH: commands_list.push_back({"drop " + item.name, item.type}); break;
					case ObjectType::FOOD:  commands_list.push_back({"eat " + item.name, item.type});  break;
				}

				if (item.type == ObjectType::WEAPON)
					armed = true;
			}

		auto room_objects = model->getRoomObjects();
		bool items = false;

		if (!room_objects.empty())
			for (auto obj : room_objects) {
				if (obj.type != ObjectType::MONSTER) {
					switch (obj.type) {
						case ObjectType::ITEM:
						case ObjectType::WEAPON:
						case ObjectType::TORCH: commands_list.push_back({"get " + obj.name, obj.type});  break;
						case ObjectType::CHEST: commands_list.push_back({"open " + obj.name, obj.type}); break;
						case ObjectType::FOOD:  commands_list.push_back({"eat " + obj.name, obj.type});  break;
					}

					items = true;
				}
				else
					if (armed)
						commands_list.push_back({"fight " + obj.name, obj.type});
			}

		for (auto obj : room_objects)
			if (obj.type == ObjectType::MONSTER)
				std::cout << "There is an evil " << obj.name << " in the room!" << std::endl;

		if (!hero_items.empty()) {
			std::cout << "Your items:" << std::endl;

			for (auto item : hero_items)
				std::cout << " - " << item.name << std::endl;
		}

		if (items) {
			std::cout << "Items in the room:" << std::endl;

			for (auto obj : room_objects)
				if (obj.type != ObjectType::MONSTER)
					std::cout << " - " << obj.name << std::endl;
		}
	}

	commands_list.push_back({"map", ObjectType::NONE});
	commands_list.push_back({"exit", ObjectType::NONE});

	std::cout << "Select command:" << std::endl;
	for (std::uint16_t i = 0; i != commands_list.size(); ++i)
		std::cout << " " << i + 1 << " - " << commands_list[i].name << std::endl;
}

void GameView::input() {
	std::uint32_t command_num;

	std::cout << std::endl << "Enter command number: ";
	std::cin >> command_num;

	while (command_num == 0 || commands_list.size() < command_num) {
		displayCommandWarnig();

		std::cout << std::endl << "Enter command number: ";
		std::cin >> command_num;
	}

	--command_num;

	auto beg = commands_list[command_num].name.find_first_of(" \n", 0);
	auto end = commands_list[command_num].name.find_first_of(" \n", beg + 1);

	command = toLower(removeSpaces(commands_list[command_num].name.substr(0, beg)));
	object  = {toLower(removeSpaces(commands_list[command_num].name.substr(beg + 1, end - beg))), 
			   commands_list[command_num].type};

	std::cout << std::endl;
}

void GameView::displayCommandWarnig() const {
	std::cout << "Wron command item!" << std::endl;
}

void GameView::displayClosedChest() const {
	std::cout << "Can not open without a key!" << std::endl;
}

void GameView::displayWin() const {
	std::cout << "You win. You have found the Holy Grail!" << std::endl;
}

void GameView::displayLose() const {
	std::cout << std::endl << "YOU DIED" << std::endl;
}

std::uint32_t GameView::getRoomsCount() const {
	return rooms_count;
}

void GameView::displayMaze() const {
	enum MazeElem: char {
		WALL  = '#',
		WAY   = ' ',
		HERO  = 'H',
		KEY   = 'K',
		CHEST = 'C'
	};

	char maze_elem = WALL;

	for (std::uint16_t i = 0; i != model->getMazeWidth(); ++i) {
		for (std::uint16_t j = 0; j != model->getMazeHeight(); ++j) {
			maze_elem = model->getRoom(i, j).isOpened(CellDirection::NORTH) ? WAY : WALL;
			std::cout << WALL << maze_elem;
		}

		std::cout << WALL << std::endl;

		for (std::uint16_t j = 0; j != model->getMazeHeight(); ++j) {
			maze_elem = model->getRoom(i, j).isOpened(CellDirection::WEST) ? WAY : WALL;

			std::cout << maze_elem;
			if (i == model->getHeroX() && j == model->getHeroY())
				std::cout << HERO;
			else
				if (i == model->getChestX() && j == model->getChestY())
					std::cout << CHEST;
				else
					if (i == model->getKeyX() && j == model->getKeyY())
						std::cout << KEY;
					else
						std::cout << WAY;
		}

		std::cout << WALL << std::endl;
	}

	for (std::uint16_t j = 0; j != model->getMazeHeight(); ++j)
		std::cout << WALL << WALL;
	std::cout << WALL << std::endl;
}

std::string GameView::getCommand() const {
	return command;
}

Object GameView::getObject() const {
	return object;
}

GameView::~GameView() {
	clearCommandsList();
}

void GameView::clearCommandsList() {
	std::vector<Object>().swap(commands_list);
}

std::string GameView::toLower(std::string str) {
	for (auto &symbol : str) 
		symbol = std::tolower(symbol);

	return str;
}

std::string GameView::removeSpaces(std::string str) {
	auto end = std::remove(str.begin(), str.end(), ' ');
	str.erase(end, str.end());

	return str;
}

std::string GameView::dirToString(CellDirection direction) {
	std::string str_direction;

	switch (direction) {
		case CellDirection::NORTH: str_direction = "N"; break;
		case CellDirection::EAST:  str_direction = "E"; break;
		case CellDirection::SOUTH: str_direction = "S"; break;
		case CellDirection::WEST:  str_direction = "W"; break;
	}

	return str_direction;
}

// ------------------- CONTROLLER ---------------------

enum class GameCommand {
	NORTH,
	EAST,
	SOUTH,
	WEST,
	GET,
	DROP,
	OPEN,
	MAP,
	EXIT
};

GameController::GameController(GameModel* model, GameView* view) {
	this->model = model;
	this->view = view;

	executed = true;
}

void GameController::start() {
	view->init();

	std::uint32_t rooms_count = view->getRoomsCount();
	std::uint16_t width  = static_cast<std::uint16_t>(std::sqrt(rooms_count));
	std::uint16_t height = width;

	std::uint16_t rand_x = rand(0, width - 1);
	std::uint16_t rand_y = rand(0, height - 1);

	model->initHero(rand_x, rand_y, rooms_count + rooms_count / 2);
	model->initMaze(width, height);
	view->update();

	std::map<std::string, GameCommand> mapping;
	mapping["n"]    = GameCommand::NORTH;
	mapping["e"]    = GameCommand::EAST;
	mapping["s"]    = GameCommand::SOUTH;
	mapping["w"]    = GameCommand::WEST;
	mapping["get"]  = GameCommand::GET;
	mapping["drop"] = GameCommand::DROP;
	mapping["open"] = GameCommand::OPEN;
	mapping["map"]  = GameCommand::MAP;
	mapping["exit"] = GameCommand::EXIT;

	GameCommand command;
	Object object;

	do {
		view->input();

		command = mapping[view->getCommand()];
		object  = view->getObject();

		switch (command) {
			case GameCommand::NORTH: moveToDirection(CellDirection::NORTH); break;
			case GameCommand::EAST:  moveToDirection(CellDirection::EAST);  break;
			case GameCommand::SOUTH: moveToDirection(CellDirection::SOUTH); break;
			case GameCommand::WEST:  moveToDirection(CellDirection::WEST);  break;
			case GameCommand::GET:   getItem(object);                       break;
			case GameCommand::DROP:  dropItem(object);                      break;
			case GameCommand::OPEN:  openChest();                           break;
			case GameCommand::MAP:   view->displayMaze();                   break;
		}
	} while (command != GameCommand::EXIT && executed);
}

void GameController::moveToDirection(CellDirection direction) {
	switch (direction) {
		case CellDirection::NORTH: model->setHeroX(model->getHeroX() - 1); break;
		case CellDirection::EAST:  model->setHeroY(model->getHeroY() + 1); break;
		case CellDirection::SOUTH: model->setHeroX(model->getHeroX() + 1); break;
		case CellDirection::WEST:  model->setHeroY(model->getHeroY() - 1); break;
	}

	auto health = model->getHeroHealth() - 1;

	if (health != 0) 
		model->setHeroHealth(health);
	else {
		close();
		view->displayLose();
	}
}

void GameController::getItem(Object item) {
	auto peek_obj = model->peekRoomObject(item);
	model->pickupHeroItem(peek_obj);
}

void GameController::dropItem(Object item) {
	auto drop_item = model->dropHeroItem(item);
	model->pushRoomObject(drop_item);
}

void GameController::openChest() {
	auto key = model->dropHeroItem({"key", ObjectType::ITEM});
	
	if (!key.name.empty()) {
		close();
		view->displayWin();
	}
	else
		view->displayClosedChest();
}

void GameController::close() {
	executed = false;
}
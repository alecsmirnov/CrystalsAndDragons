#include "Game.h"

#include <iostream>
#include <algorithm>
#include <map>
#include <cctype>

#include <array>

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

	maze.pushCellObject(rand_x, rand_y, {"lurker", ObjectType::MONSTER});

	// --------------------
	rand_x = rand(0, maze.getWidth() - 1);
	rand_y = rand(0, maze.getHeight() - 1);

	maze.setCellType(rand_x, rand_y, CellType::DARK);
	// --------------------

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

	std::array<CellDirection, static_cast<std::uint8_t>(CellDirection::SIZE)> all_directions = {
		CellDirection::NORTH, CellDirection::EAST, CellDirection::SOUTH, CellDirection::WEST
	};

	for (auto dir : all_directions)
		if (maze.getCell(hero.getX(), hero.getY()).isOpened(dir))
			room_doors.push_back(dir);

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

	auto door_directions = model->getRoomDoors();

	if (!door_directions.empty())
		for (auto dir : door_directions)
			switch (dir) {
				case CellDirection::NORTH: operations_list.push_back({GameCommand::NORTH, {"N", ObjectType::DIRECTION}}); break;
				case CellDirection::EAST:  operations_list.push_back({GameCommand::EAST,  {"E", ObjectType::DIRECTION}}); break;
				case CellDirection::SOUTH: operations_list.push_back({GameCommand::SOUTH, {"S", ObjectType::DIRECTION}}); break;
				case CellDirection::WEST:  operations_list.push_back({GameCommand::WEST,  {"W", ObjectType::DIRECTION}}); break;
			}

	auto hero_items = model->getHeroItems();
	bool armed = false;
	bool hero_torch = false;

	if (!hero_items.empty())
		for (auto item : hero_items)
			switch (item.type) {
				case ObjectType::WEAPON: armed = true;
				case ObjectType::ITEM:
				case ObjectType::KEY:    operations_list.push_back({GameCommand::DROP, item});					  break;
				case ObjectType::TORCH:  operations_list.push_back({GameCommand::DROP, item}); hero_torch = true; break;
				case ObjectType::FOOD:   operations_list.push_back({GameCommand::EAT,  item});
									     operations_list.push_back({GameCommand::DROP, item});					  break;
			}

	auto room_objects = model->getRoomObjects();
	bool items_exist = false;
	bool room_torch = false;

	if (!room_objects.empty())
		for (auto obj : room_objects)
			switch (obj.type) {
				case ObjectType::TORCH:	  room_torch = true;
				case ObjectType::ITEM:
				case ObjectType::WEAPON:
				case ObjectType::KEY:     operations_list.push_back({GameCommand::GET,  obj}); items_exist = true; break;
				case ObjectType::CHEST:   operations_list.push_back({GameCommand::OPEN, obj}); items_exist = true; break;
				case ObjectType::FOOD:	  operations_list.push_back({GameCommand::EAT,  obj});  
										  operations_list.push_back({GameCommand::GET,  obj}); items_exist = true; break;
				case ObjectType::MONSTER: if (armed) operations_list.push_back({GameCommand::FIGHT, obj});		   break;
			}

	auto room_type = model->getRoomType();

	if (room_type == CellType::LIGHT || hero_torch || room_torch)
		std::cout << "You are in the room [" << x << ", " << y << "]. ";
	else
		std::cout << "Can't see anything in this dark place! ";

	std::cout << "There are " << door_directions.size() << " doors: ";

	std::uint8_t doors_count = 0;
	std::uint8_t doors_max = static_cast<std::uint8_t>(CellDirection::SIZE);

	for (auto op_it = operations_list.begin(); op_it != operations_list.end() && doors_count != doors_max; ++op_it, ++doors_count)
		if (doors_count < doors_max - 1 && op_it->object.type == ObjectType::DIRECTION)
			std::cout << op_it->object.name << ", ";
		else 
			std::cout << op_it->object.name << "." << std::endl;

	if (room_type == CellType::LIGHT || hero_torch || room_torch) {
		if (!hero_items.empty()) {
			std::cout << "Your items:" << std::endl;

			for (auto op : operations_list)
				if (op.command == GameCommand::DROP || op.command == GameCommand::EAT)
					std::cout << " - " << op.object.name << std::endl;
		}

		if (items_exist) {
			std::cout << "Items in the room:" << std::endl;

			for (auto op : operations_list)
				if (op.command == GameCommand::GET || op.command == GameCommand::OPEN ||
					op.command == GameCommand::EAT)
					std::cout << " - " << op.object.name << std::endl;
		}
	}

	operations_list.push_back({GameCommand::MAP,  {"map",  ObjectType::NONE}});
	operations_list.push_back({GameCommand::EXIT, {"exit", ObjectType::NONE}});

	std::cout << "Select command:" << std::endl;

	for (std::size_t i = 0; i != operations_list.size(); ++i) {
		std::string command_prefix;

		switch (operations_list[i].command) {
			case GameCommand::NORTH: 
			case GameCommand::EAST:
			case GameCommand::SOUTH:
			case GameCommand::WEST:  
			case GameCommand::MAP:
			case GameCommand::EXIT:  command_prefix = "";      break;
			case GameCommand::GET:   command_prefix = "get";   break;
			case GameCommand::DROP:  command_prefix = "drop";  break;
			case GameCommand::OPEN:  command_prefix = "open";  break;
			case GameCommand::EAT:   command_prefix = "eat";   break;
			case GameCommand::FIGHT: command_prefix = "fight"; break;
		}

		std::cout << " " << i + 1 << " - " << command_prefix << (command_prefix.empty() ? "" : " ");
		std::cout << operations_list[i].object.name << std::endl;
	}
}

void GameView::input() {
	std::uint32_t command_num;

	std::cout << std::endl << "Enter command number: ";
	std::cin >> command_num;

	while (command_num == 0 || operations_list.size() < command_num) {
		displayCommandWarnig();

		std::cout << std::endl << "Enter command number: ";
		std::cin >> command_num;
	}

	--command_num;

	operation = operations_list[command_num];
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

GameOperation GameView::getOperation() const {
	return operation;
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

GameView::~GameView() {
	clearCommandsList();
}

void GameView::clearCommandsList() {
	std::vector<GameOperation>().swap(operations_list);
}

// ------------------- CONTROLLER ---------------------

GameController::GameController(GameModel* model, GameView* view) {
	this->model = model;
	this->view = view;

	executed = true;
}

void GameController::start() {
	view->init();

	std::uint32_t rooms_count = view->getRoomsCount();

	std::vector<std::pair<std::uint16_t, std::uint16_t>> factors;

	for (std::uint32_t i = 1; i <= rooms_count; ++i)
		if (rooms_count % i == 0)
			factors.push_back({i, rooms_count / i});

	std::uint16_t width  = factors[factors.size() / 2].first;
	std::uint16_t height = factors[factors.size() / 2].second;

	std::uint16_t rand_x = rand(0, width - 1);
	std::uint16_t rand_y = rand(0, height - 1);

	model->initHero(rand_x, rand_y, rooms_count * 2);
	model->initMaze(width, height);
	view->update();

	GameOperation operation;

	do {
		view->input();

		operation = view->getOperation();

		switch (operation.command) {
			case GameCommand::NORTH: moveToDirection(CellDirection::NORTH); break;
			case GameCommand::EAST:  moveToDirection(CellDirection::EAST);  break;
			case GameCommand::SOUTH: moveToDirection(CellDirection::SOUTH); break;
			case GameCommand::WEST:  moveToDirection(CellDirection::WEST);  break;
			case GameCommand::GET:   getItem(operation.object);             break;
			case GameCommand::DROP:  dropItem(operation.object);            break;
			case GameCommand::OPEN:  openChest();                           break;
			case GameCommand::MAP:   view->displayMaze();                   break;
		}
	} while (operation.command != GameCommand::EXIT && executed);
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
	auto items = model->getHeroItems();

	bool found = false;
	for (auto it = items.begin(); it != items.begin() && !found; ++it)
		if (it->type == ObjectType::KEY)
			found = true;
	
	if (found) {
		close();
		view->displayWin();
	}
	else
		view->displayClosedChest();
}

void GameController::close() {
	executed = false;
}
#include "GameView.h"

#include <iostream>
#include <chrono>

GameView::GameView(GameModel* model) {
	this->model = model;

	this->rooms_count = 0;

	this->input_time = 0;
	this->threat = false;
}

// Ввод кол-ва комнат в лабиринте
void GameView::initInput() {
	auto rooms_min = model->getRoomsMin();

	std::cout << "Enter the number of rooms:" << std::endl;
	std::cin >> rooms_count;

	// Проверка на допустимое кол-во
	while (rooms_count < rooms_min) {
		displayRoomsCountWarnig();
		std::cout << "Enter the number of rooms:" << std::endl;
		std::cin >> rooms_count;
	}
}

// Выбор команд из списка
void GameView::input() {
	std::uint32_t command_num;
	// Получение времени ввода
	input_time = inputCommandWithTime(command_num);

	// Проверка на допустимый пункт
	while (command_num == 0 || operations_list.size() < command_num) {
		displayCommandWarnig();
		input_time += inputCommandWithTime(command_num);
	}

	--command_num;

	operation = operations_list[command_num];
	std::cout << std::endl;
}

// Вывод/обновление информации на экране
void GameView::update() {
	// Очищаем предыдущий список команд
	clearCommandsList();
	// Избавляемся от угроз
	threat = false;

	// Получение положения Героя
	std::uint16_t x = model->getHeroX();
	std::uint16_t y = model->getHeroY();

	// Получение вещей Героя
	auto hero_items = model->getHeroItems();
	bool armed = false;
	bool hero_torch = false;

	// Проверяем, есть ли оружие или факел у героя
	for (auto item : hero_items)
		switch (item.type) {
			case ObjectType::WEAPON: armed = true;		break; 
			case ObjectType::TORCH:  hero_torch = true; break;
		}

	// Получение объектов комнаты
	auto room_objects = model->getRoomObjects();
	bool items_exist = false;
	bool room_torch = false;

	// Проверяем есть ли в комнате факел
	for (auto obj_it = room_objects.begin(); obj_it != room_objects.end(); ++obj_it)
		if (obj_it->type == ObjectType::TORCH)
			room_torch = true;

	// Получем кол-во дверей в комнате и её тип (светлая, тёмная)
	auto room_type = model->getRoomType();
	auto door_directions = model->getRoomDoors();

	if (room_type == CellType::LIGHT || hero_torch || room_torch)
		std::cout << "You are in the room [" << x << ", " << y << "]. ";
	else
		std::cout << "Can't see anything in this dark place! ";

	std::cout << "There are " << door_directions.size() << " doors: ";

	// Добавляем операции по перемещению
	if (!door_directions.empty())
		for (auto dir : door_directions)
			switch (dir) {
				case CellDirection::NORTH: operations_list.push_back({GameCommand::NORTH, {"N", ObjectType::DIRECTION}}); break;
				case CellDirection::EAST:  operations_list.push_back({GameCommand::EAST,  {"E", ObjectType::DIRECTION}}); break;
				case CellDirection::SOUTH: operations_list.push_back({GameCommand::SOUTH, {"S", ObjectType::DIRECTION}}); break;
				case CellDirection::WEST:  operations_list.push_back({GameCommand::WEST,  {"W", ObjectType::DIRECTION}}); break;
			}

	std::uint8_t doors_count = 0;
	std::uint8_t doors_max = static_cast<std::uint8_t>(door_directions.size());

	// Выводим перечень доступных направлений
	for (auto op_it = operations_list.begin(); op_it != operations_list.end() && 
		 doors_count != doors_max; ++op_it, ++doors_count)
		if (op_it->object.type == ObjectType::DIRECTION) {
			if (doors_count < doors_max - 1)
				std::cout << op_it->object.name << ", ";
			else
				std::cout << op_it->object.name << "." << std::endl;
		}

	// Если комната светлая, или у Героя есть факел или в комнате лежит факел
	// Добавляем в операции перечень предметов игрока, перечень объектов в комнате
	if (room_type == CellType::LIGHT || hero_torch || room_torch) {
		if (!hero_items.empty())
			for (auto item : hero_items)
				switch (item.type) {
					case ObjectType::WEAPON:
					case ObjectType::TORCH:
					case ObjectType::GOLD: 
					case ObjectType::KEY:  operations_list.push_back({GameCommand::DROP, item}); break;
					case ObjectType::FOOD: operations_list.push_back({GameCommand::EAT,  item});
										   operations_list.push_back({GameCommand::DROP, item}); break;
				}

		if (!room_objects.empty())
			for (auto obj : room_objects)
				switch (obj.type) {
					case ObjectType::TORCH: room_torch = true;
					case ObjectType::WEAPON:
					case ObjectType::GOLD:
					case ObjectType::KEY:   operations_list.push_back({GameCommand::GET,  obj}); items_exist = true; break;
					case ObjectType::CHEST: operations_list.push_back({GameCommand::OPEN, obj}); items_exist = true; break;
					case ObjectType::FOOD:  operations_list.push_back({GameCommand::EAT,  obj});
											operations_list.push_back({GameCommand::GET,  obj}); items_exist = true; break;
					case ObjectType::MONSTER: if (armed) operations_list.push_back({GameCommand::FIGHT, obj});		 break;
				}

		// Если в комнате есть монстры -- выводим соответствующее сообшение
		// и устанавливаем положительный уровень угрозы
		for (auto obj : room_objects)
			if (obj.type == ObjectType::MONSTER) {
				std::cout << "There is an evil " << getObjectEdging(obj) << " in the room!" << std::endl;

				threat = true;
			}

		// Выводим перечень предметов Героя
		if (!hero_items.empty()) {
			std::cout << "Your items:" << std::endl;

			for (auto op : operations_list)
				if (op.command == GameCommand::DROP)
					std::cout << " - " << getObjectEdging(op.object) << std::endl;
		}

		// Выводим перечень предметов в комнате
		if (items_exist) {
			std::cout << "Items in the room:" << std::endl;

			for (auto op : operations_list)
				if (op.command == GameCommand::GET || op.command == GameCommand::OPEN)
					std::cout << " - " << getObjectEdging(op.object) << std::endl;
		}
	}

	// Добавляем в операции карту и выход
	operations_list.push_back({GameCommand::MAP,  {"map", ObjectType::EMPTY}});
	operations_list.push_back({GameCommand::EXIT, {"exit", ObjectType::EMPTY}});

	std::cout << "Select command:" << std::endl;
	
	// Выводим, из списка операций, список доступных для ввода команд 
	for (std::size_t i = 0; i != operations_list.size(); ++i) {
		std::string command_prefix = getCommandPrefix(operations_list[i].command, false);

		std::cout << " " << i + 1 << " - " << command_prefix << (command_prefix.empty() ? "" : " ");
		std::cout << getObjectEdging(operations_list[i].object) << std::endl;
	}

	std::cout << std::endl;
}

// Вывод предупреждения о неправильно заданном кол-ве комнат
void GameView::displayRoomsCountWarnig() const {
	std::cout << "The number of rooms cannot be less than " << model->getRoomsMin() << "!" << std::endl << std::endl;
}

// Вывод предупреждения о неправильно введённой команде
void GameView::displayCommandWarnig() const {
	std::cout << "Wron command item!" << std::endl << std::endl;
}

// Вывод предупреждения о попытке открыть сундук без ключа
void GameView::displayClosedChest() const {
	std::cout << "Can not open without a key!" << std::endl << std::endl;
}

// Вывод сообщения о победе (открытии сундука)
void GameView::displayWin() const {
	std::cout << "You win. You have found the Holy Grail!" << std::endl;
}

// Вывод сообщения о поражении (смерти)
void GameView::displayLose() const {
	std::cout << std::endl << "YOU DIED" << std::endl;
}

// Вывод сообщения о неудачной встрече с монстром
void GameView::displayFightLose() const {
	std::cout << "The monster took your health and threw you back!" << std::endl << std::endl;
}

// Вывод сообщения о ничьей
void GameView::displayFightDrow() const {
	auto prefix = getCommandPrefix(operation.command, true);

	std::cout << "You " << prefix << " " << getObjectEdging(operation.object) << " but lost your health!" << std::endl << std::endl;
}

// Вывод сообщения об удачной встрече с монстром
void GameView::displayFightWin() const {
	auto prefix = getCommandPrefix(operation.command, true);

	std::cout << "You successfully " << prefix << " " << getObjectEdging(operation.object) << "!" << std::endl << std::endl;
}

// Получить количество введённых комнат 
std::uint32_t GameView::getRoomsCount() const {
	return rooms_count;
}

// Получить введённую операцияю
GameOperation GameView::getOperation() const {
	return operation;
}

// Получить время введённой операции
std::uint64_t GameView::getOperationTime() const {
	return input_time;
}

// Получить статус угрозы (есть или нет монстр в комнате)
bool GameView::getThreatStatus() const {
	return threat;
}

// Вывод лабиринта
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
	std::cout << WALL << std::endl << std::endl;
}

GameView::~GameView() {
	clearCommandsList();
}

// Ввод команты с выводом её времени
std::uint64_t GameView::inputCommandWithTime(std::uint32_t& command_num) {
	auto beg = std::chrono::steady_clock::now();

	std::cout << "Enter command number: ";
	std::cin >> command_num;

	auto end = std::chrono::steady_clock::now();
	auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end - beg).count();

	return elapsed_time;
}

// Получить префикс команты (move to, get, drop, fight, ...)
std::string GameView::getCommandPrefix(GameCommand command, bool directions) {
	std::string command_prefix;

	switch (command) {
		case GameCommand::NORTH:
		case GameCommand::EAST:
		case GameCommand::SOUTH:
		case GameCommand::WEST:
		case GameCommand::MAP:
		case GameCommand::EXIT:  command_prefix = directions ? "move to" : ""; break;
		case GameCommand::GET:   command_prefix = "get";   break;
		case GameCommand::DROP:  command_prefix = "drop";  break;
		case GameCommand::OPEN:  command_prefix = "open";  break;
		case GameCommand::EAT:   command_prefix = "eat";   break;
		case GameCommand::FIGHT: command_prefix = "fight"; break;
	}

	return command_prefix;
}

// Получить оформление объекта (цвет, преписка для денег)
std::string GameView::getObjectEdging(Object object) {
	std::string result = object.name;

	switch (object.type) {
		case ObjectType::GOLD:	  result = yellowText("gold (" + object.name + " coins)"); break;
		case ObjectType::MONSTER: result = redText(object.name);   break;
		case ObjectType::FOOD:	  result = greenText(object.name); break;
	}

	return result;
}

void GameView::clearCommandsList() {
	std::vector<GameOperation>().swap(operations_list);
}
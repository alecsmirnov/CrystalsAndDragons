#include "GameModel.h"

#include <cmath>
#include <string>
#include <array>

static constexpr std::uint32_t ROOMS_MIN = 2;

static constexpr std::uint64_t WAIT_TIME_SEC = 5;

static constexpr double HEALTH_INIT_PERCENT = 2.8;
static constexpr double HEALTH_PENALTY		= 1;
static constexpr double HEALTH_LOSE_PERCENT = 0.1;
static constexpr double HEALTH_LIFT_PERCENT = 1.1;

static constexpr double FOOD_PERCENT = 0.5;

static constexpr double MONSTERS_PERCENT   = 0.1;
static constexpr double DARK_ROOMS_PERCENT = 0.2;

static constexpr double        GOLD_PERCENT		= 0.5;
static constexpr std::uint16_t GOLD_PORTION_MAX = 100;

void GameModel::initModel(std::uint32_t rooms_count) {
	Pair maze_size = {1, 1};

	getMazeSize(rooms_count, maze_size.x, maze_size.y);
	maze.init(maze_size.x, maze_size.y);
	maze.generate();

	Pair hero_pos = {rand(0, maze_size.x - 1), rand(0, maze_size.y - 1)};
	hero.setX(hero_pos.x);
	hero.setY(hero_pos.y);

	auto hero_init_health = std::round(rooms_count * HEALTH_INIT_PERCENT);
	hero.setHelth(hero_init_health);

	Pair key_pos = {0, 0};
	do {
		key_pos = {rand(0, maze_size.x - 1), rand(0, maze_size.y - 1)};
	} while (key_pos.x == hero_pos.x && key_pos.y == hero_pos.y);


	key_position = key_pos;
	maze.pushCellObject(key_pos.x, key_pos.y, {"key", ObjectType::KEY});

	Pair chest_pos = {0, 0};
	do {
		chest_pos = {rand(0, maze_size.x - 1), rand(0, maze_size.y - 1)};
	} while (chest_pos.x == key_pos.x && chest_pos.y == key_pos.y);

	chest_position = chest_pos;
	maze.pushCellObject(chest_pos.x, chest_pos.y, {"chest", ObjectType::CHEST});

	auto monsters_count = static_cast<std::uint32_t>(std::round(rooms_count * MONSTERS_PERCENT));
	for (std::uint32_t i = 0; i != monsters_count; ++i) {
		Pair monster_pos = {0, 0};
		do {
			monster_pos = {rand(0, maze_size.x - 1), rand(0, maze_size.y - 1)};
		} while (monster_pos.x == hero_pos.x && monster_pos.y == hero_pos.y);

		maze.pushCellObject(monster_pos.x, monster_pos.y, {"lurker", ObjectType::MONSTER});
	}

	Pair prev_dark_room = {0, 0};
	auto dark_rooms_count = static_cast<std::uint32_t>(std::round(rooms_count * DARK_ROOMS_PERCENT));
	for (std::uint32_t i = 0; i != dark_rooms_count; ++i) {
		Pair dark_room = {hero_pos.x, hero_pos.y};
		do {
			dark_room = {rand(0, maze_size.x - 1), rand(0, maze_size.y - 1)};
		} while (dark_room.x == prev_dark_room.x && dark_room.y == prev_dark_room.y);

		maze.setCellType(dark_room.x, dark_room.y, CellType::DARK);

		prev_dark_room = dark_room;
	}

	maze.pushCellObject(rand(0, maze_size.x - 1), rand(0, maze_size.y - 1), {"torchlight", ObjectType::TORCH});
	maze.pushCellObject(rand(0, maze_size.x - 1), rand(0, maze_size.y - 1), {"sword",	   ObjectType::WEAPON});

	auto food_count = static_cast<std::uint32_t>(std::round(rooms_count * FOOD_PERCENT));
	for (std::uint32_t i = 0; i != food_count; ++i)
		maze.pushCellObject(rand(0, maze_size.x - 1), rand(0, maze_size.y - 1), {"fried chicken", ObjectType::FOOD});

	auto gold_chunks_count = static_cast<std::uint32_t>(std::round(rooms_count * GOLD_PERCENT));
	for (std::uint32_t i = 0; i != gold_chunks_count; ++i) {
		auto gold_portion = rand(0, GOLD_PORTION_MAX - 1);
		maze.pushCellObject(rand(0, maze_size.x - 1), rand(0, maze_size.y - 1), {std::to_string(gold_portion), ObjectType::GOLD});
	}
}

void GameModel::addRoomObject(std::uint16_t x, std::uint16_t y, Object item) {
	maze.pushCellObject(x, y, item);
}

void GameModel::setHeroX(std::uint16_t x) {
	previos_room = getDirection(x, hero.getY(), hero.getX(), hero.getY());

	hero.setX(x);
}

std::uint16_t GameModel::getHeroX() const {
	return hero.getX();
}

void GameModel::setHeroY(std::uint16_t y) {
	previos_room = getDirection(hero.getX(), y, hero.getX(), hero.getY());

	hero.setY(y);
}

std::uint16_t GameModel::getHeroY() const {
	return hero.getY();
}

void GameModel::setHeroHealth(double health) {
	hero.setHelth(health);
}

double GameModel::getHeroHealth() const {
	return hero.getHealth();
}

void GameModel::setHeroItems(const std::vector<Object>& items) {
	hero.setItems(items);
}

std::vector<Object> GameModel::getHeroItems() const {
	return hero.getItems();
}

void GameModel::pickupHeroItem(Object item) {
	item.type == ObjectType::GOLD ? pickupHeroGold(item) : hero.pickupItem(item);
}

Object GameModel::dropHeroItem(Object item) {
	return hero.dropItem(item);
}

std::vector<Object> GameModel::getRoomObjects() const {
	return maze.getCellObjects(hero.getX(), hero.getY());
}

void GameModel::pushRoomObject(Object item) {
	maze.pushCellObject(hero.getX(), hero.getY(), item);
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

CellDirection GameModel::getPreviosRoom() const {
	return previos_room;
}

std::uint16_t GameModel::getKeyX() const {
	return key_position.x;
}

std::uint16_t GameModel::getKeyY() const {
	return key_position.y;
}

std::uint16_t GameModel::getChestX() const {
	return chest_position.x;
}

std::uint16_t GameModel::getChestY() const {
	return chest_position.y;
}

std::uint16_t GameModel::getMazeWidth() const {
	return maze.getWidth();
}

std::uint16_t GameModel::getMazeHeight() const {
	return maze.getHeight();
}

std::uint32_t GameModel::getRoomsMin() const {
	return ROOMS_MIN;
}

std::uint64_t GameModel::getWaitTime() const {
	return WAIT_TIME_SEC;
}

double GameModel::getHelthPenalty() const {
	return HEALTH_PENALTY;
}

double GameModel::getHelthLosePercent() const {
	return HEALTH_LOSE_PERCENT;
}

double GameModel::getHelthLiftPercent() const {
	return HEALTH_LIFT_PERCENT;
}

void GameModel::pickupHeroGold(Object item) {
	auto hero_items = hero.getItems();
	auto found_gold = std::find_if(hero_items.begin(), hero_items.end(),
								   [&](const Object& obj) { return item.type == obj.type; });

	Object gold_sum = item;

	if (found_gold != hero_items.end()) {
		gold_sum = {std::to_string(std::stoi(item.name) +
										  std::stoi(found_gold->name)), item.type};
		hero_items.erase(found_gold);
	}

	hero_items.insert(hero_items.begin(), gold_sum);
	hero.setItems(hero_items);
}

void GameModel::getMazeSize(std::uint32_t rooms_count, std::uint16_t &width, std::uint16_t &height) {
	std::vector<Pair> factors;

	for (std::uint32_t i = 1; i <= rooms_count; ++i)
		if (rooms_count % i == 0) {
			auto a = static_cast<std::uint16_t>(i);
			auto b = static_cast<std::uint16_t>(rooms_count / i);

			factors.push_back({a, b});
		}

	if (!factors.empty()) {
		auto center = factors.size() / 2;

		width = factors[center].x;
		height = factors[center].y;
	}

	std::vector<Pair>().swap(factors);
}
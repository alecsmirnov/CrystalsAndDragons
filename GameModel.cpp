#include "GameModel.h"

#include <cmath>
#include <string>
#include <array>

// Минимально-допустимое количество комнат в лабиринте
static constexpr std::uint32_t ROOMS_MIN = 2;

// Время ожидания ответа в случае встречи с монстром
static constexpr std::uint64_t WAIT_TIME_SEC = 5;

// Параметры влияния на здоровье Героя
static constexpr double HEALTH_INIT_PERCENT = 2.8;		// Инициализация начального здоровья Героя от кол-ва комнат
static constexpr double HEALTH_PENALTY		= 1;		// Величина уменьшения здоровья при перемещении
static constexpr double HEALTH_LOSE_PERCENT = 0.9;		// Процент уменьшение здоровья при атаках монстра
static constexpr double HEALTH_LIFT_PERCENT = 1.1;		// Процент увеличения здоровья после приёма пищи

// Процент количество еды на количество комнат лабиринта
static constexpr double FOOD_PERCENT = 0.5;

// Процент количества монстров на количество комнат лабиринта
static constexpr double MONSTERS_PERCENT   = 0.2;
// Процент количества тёмных комнат на количество комнат лабиринта
static constexpr double DARK_ROOMS_PERCENT = 0.2;

// Процент количества золота на количество комнат лабиринта
static constexpr double        GOLD_PERCENT		= 0.5;
// Размер максимально-возможной найденной кучки золота
static constexpr std::uint16_t GOLD_PORTION_MAX = 100;

// Инициализация данных (Кол-во комнат, положение игровых обектов, ...)
void GameModel::initModel(std::uint32_t rooms_count) {
	Pair maze_size = {1, 1};

	// Получение ширины и высоты лабиринта по кол-ву комнат
	getMazeSize(rooms_count, maze_size.x, maze_size.y);
	maze.init(maze_size.x, maze_size.y);
	maze.generate();

	// Определение положения Героя
	Pair hero_pos = {rand(0, maze_size.x - 1), rand(0, maze_size.y - 1)};
	hero.setX(hero_pos.x);
	hero.setY(hero_pos.y);

	auto hero_init_health = std::round(rooms_count * HEALTH_INIT_PERCENT);
	hero.setHelth(hero_init_health);

	// Определение положения ключа
	Pair key_pos = {0, 0};
	do {
		key_pos = {rand(0, maze_size.x - 1), rand(0, maze_size.y - 1)};
	} while (key_pos.x == hero_pos.x && key_pos.y == hero_pos.y);

	key_position = key_pos;
	maze.pushCellObject(key_pos.x, key_pos.y, {"key", ObjectType::KEY});

	// Определение положения сундука. Сундук и ключ не должны находиться в одной комнате
	Pair chest_pos = {0, 0};
	do {
		chest_pos = {rand(0, maze_size.x - 1), rand(0, maze_size.y - 1)};
	} while (chest_pos.x == key_pos.x && chest_pos.y == key_pos.y);

	chest_position = chest_pos;
	maze.pushCellObject(chest_pos.x, chest_pos.y, {"chest", ObjectType::CHEST});

	// Определение кол-ва монстров и их положений. Монстры не спаунятся в точке спауна Героя
	auto monsters_count = static_cast<std::uint32_t>(std::round(rooms_count * MONSTERS_PERCENT));
	for (std::uint32_t i = 0; i != monsters_count; ++i) {
		Pair monster_pos = {0, 0};
		do {
			monster_pos = {rand(0, maze_size.x - 1), rand(0, maze_size.y - 1)};
		} while (monster_pos.x == hero_pos.x && monster_pos.y == hero_pos.y);

		maze.pushCellObject(monster_pos.x, monster_pos.y, {"lurker", ObjectType::MONSTER});
	}

	// Определение кол-ва тёмных комнат и их положений
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

	// Определение положения фаекла и меча
	maze.pushCellObject(rand(0, maze_size.x - 1), rand(0, maze_size.y - 1), {"torchlight", ObjectType::TORCH});
	maze.pushCellObject(rand(0, maze_size.x - 1), rand(0, maze_size.y - 1), {"sword",	   ObjectType::WEAPON});

	// Определение кол-ва еды и её положения
	auto food_count = static_cast<std::uint32_t>(std::round(rooms_count * FOOD_PERCENT));
	for (std::uint32_t i = 0; i != food_count; ++i)
		maze.pushCellObject(rand(0, maze_size.x - 1), rand(0, maze_size.y - 1), {"fried chicken", ObjectType::FOOD});

	// Определение кол-ва золота и его размера
	auto gold_chunks_count = static_cast<std::uint32_t>(std::round(rooms_count * GOLD_PERCENT));
	for (std::uint32_t i = 0; i != gold_chunks_count; ++i) {
		auto gold_portion = rand(0, GOLD_PORTION_MAX - 1);
		maze.pushCellObject(rand(0, maze_size.x - 1), rand(0, maze_size.y - 1), {std::to_string(gold_portion), ObjectType::GOLD});
	}
}

// Добавить игровой объект в лабиринт
void GameModel::addRoomObject(std::uint16_t x, std::uint16_t y, Object item) {
	maze.pushCellObject(x, y, item);
}

// Задать положение Героя по оси х
void GameModel::setHeroX(std::uint16_t x) {
	// Получение предыдущей комнаты по двум координамт
	previos_room = getDirection(x, hero.getY(), hero.getX(), hero.getY());

	hero.setX(x);
}

// Получить положение Героя по оси х
std::uint16_t GameModel::getHeroX() const {
	return hero.getX();
}

// Задать положение Героя по оси y
void GameModel::setHeroY(std::uint16_t y) {
	// Получение предыдущей комнаты по двум координамт
	previos_room = getDirection(hero.getX(), y, hero.getX(), hero.getY());

	hero.setY(y);
}

// Получить положение Героя по оси y
std::uint16_t GameModel::getHeroY() const {
	return hero.getY();
}

// Задать здоровье Героя
void GameModel::setHeroHealth(double health) {
	hero.setHelth(health);
}

// Получить здоровье Героя
double GameModel::getHeroHealth() const {
	return hero.getHealth();
}

// Задать инвентарь Героя
void GameModel::setHeroItems(const std::vector<Object>& items) {
	hero.setItems(items);
}

// Получить инвентарь Героя
std::vector<Object> GameModel::getHeroItems() const {
	return hero.getItems();
}

// Положить предмет в инвентарь Героя
void GameModel::pickupHeroItem(Object item) {
	item.type == ObjectType::GOLD ? pickupHeroGold(item) : hero.pickupItem(item);
}

// Выкинуть предмет из инвентаря Героя
Object GameModel::dropHeroItem(Object item) {
	return hero.dropItem(item);
}

// Получить все объекты в комнате
std::vector<Object> GameModel::getRoomObjects() const {
	return maze.getCellObjects(hero.getX(), hero.getY());
}

// Положить объект в комнату
void GameModel::pushRoomObject(Object item) {
	maze.pushCellObject(hero.getX(), hero.getY(), item);
}

// Взять объект из комнаты
Object GameModel::peekRoomObject(Object item) {
	return maze.peekCellObject(hero.getX(), hero.getY(), item);
}

// Получить список всех дверей комнаты
std::vector<CellDirection> GameModel::getRoomDoors() const {
	std::vector<CellDirection> room_doors;

	// Т.к. перечисления имеют заданные значения, не позволяющие перемещаться по ним в обычном виде,
	// используем массив с перечислениями и проходимся по нему
	std::array<CellDirection, static_cast<std::uint8_t>(CellDirection::SIZE)> all_directions = {
		CellDirection::NORTH, CellDirection::EAST, CellDirection::SOUTH, CellDirection::WEST
	};

	for (auto dir : all_directions)
		if (maze.getCell(hero.getX(), hero.getY()).isOpened(dir))
			room_doors.push_back(dir);

	return room_doors;
}

// Получить тип комнаты (светлая, тёмная)
CellType GameModel::getRoomType() const {
	return maze.getCellType(hero.getX(), hero.getY());
}

// Получить любую комнату по координам
Cell GameModel::getRoom(std::uint16_t x, std::uint16_t y) const {
	return maze.getCell(x, y);
}

// Получить предыдущую комнату в которой был Герой
CellDirection GameModel::getPreviosRoom() const {
	return previos_room;
}

// Получить позицию ключа по оси х
std::uint16_t GameModel::getKeyX() const {
	return key_position.x;
}

// Получить позицию ключа по оси y
std::uint16_t GameModel::getKeyY() const {
	return key_position.y;
}

// Получить позицию сундука по оси х
std::uint16_t GameModel::getChestX() const {
	return chest_position.x;
}

// Получить позицию сундука по оси y
std::uint16_t GameModel::getChestY() const {
	return chest_position.y;
}

// Получить ширину лабиринта
std::uint16_t GameModel::getMazeWidth() const {
	return maze.getWidth();
}

// Получить высоту лабиринта
std::uint16_t GameModel::getMazeHeight() const {
	return maze.getHeight();
}

// Получить минимально-возможное количество комнат
std::uint32_t GameModel::getRoomsMin() const {
	return ROOMS_MIN;
}

// Получить время ожидания ответа в случае встречи с монстром
std::uint64_t GameModel::getWaitTime() const {
	return WAIT_TIME_SEC;
}

// Получить величину уменьшения здоровья при перемещении
double GameModel::getHealthPenalty() const {
	return HEALTH_PENALTY;
}

// Получить процент уменьшение здоровья при атаках монстра
double GameModel::getHealthLosePercent() const {
	return HEALTH_LOSE_PERCENT;
}

// Получить процент увеличения здоровья после приёма пищи
double GameModel::getHealthLiftPercent() const {
	return HEALTH_LIFT_PERCENT;
}

// Функция взятия золота (группировка золота в один предмет)
void GameModel::pickupHeroGold(Object item) {
	// Поиск имеющегося в инвентаре Героя золота
	auto hero_items = hero.getItems();
	auto found_gold = std::find_if(hero_items.begin(), hero_items.end(),
								   [&](const Object& obj) { return item.type == obj.type; });

	// Если золото найдено -- добавляем к нему переданное, иначе добавляем золото в начало инвентаря 
	Object gold_sum = item;
	if (found_gold != hero_items.end()) {
		gold_sum = {std::to_string(std::stoi(item.name) + std::stoi(found_gold->name)), item.type};
		hero_items.erase(found_gold);
	}

	hero_items.insert(hero_items.begin(), gold_sum);
	hero.setItems(hero_items);
}

// Получить ширину и высоту массива по количеству комнат
void GameModel::getMazeSize(std::uint32_t rooms_count, std::uint16_t &width, std::uint16_t &height) {
	std::vector<Pair> factors;

	// Поиск всех возможных делителей числа (кол-ва комнат)
	for (std::uint32_t i = 1; i <= rooms_count; ++i)
		if (rooms_count % i == 0) {
			auto a = static_cast<std::uint16_t>(i);
			auto b = static_cast<std::uint16_t>(rooms_count / i);

			factors.push_back({a, b});
		}

	// Если есть делители -- выбираем центральные из них,
	// приближающие лабиринт к квадратному/прямоугольному виду
	if (!factors.empty()) {
		auto center = factors.size() / 2;

		width = factors[center].x;
		height = factors[center].y;
	}

	std::vector<Pair>().swap(factors);
}
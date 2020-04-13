#include "Maze.h"

#include <random>
#include <stack>

Maze::Maze(): width(0), height(0), field(nullptr) {}

Maze::Maze(std::uint16_t width, std::uint16_t height) {
	init(width, height);
}

// Инициализация лабиринта
void Maze::init(std::uint16_t width, std::uint16_t height) {
	if (field)
		clear();

	this->width = width;
	this->height = height;

	field = new Cell * [width];
	for (std::uint16_t i = 0; i != width; ++i) {
		field[i] = new Cell[height];

		for (std::uint16_t j = 0; j != height; ++j)
			field[i][j].setCoords(i, j);
	}
}

// Получить ширину лабиринта
std::uint16_t Maze::getWidth() const {
	return width;
}

// Получить высоту лабиринта
std::uint16_t Maze::getHeight() const {
	return height;
}

// Получить комнату по координатам
Cell Maze::getCell(std::uint16_t x, std::uint16_t y) const {
	Cell cell;

	if (x < width && y < height)
		cell = field[x][y];

	return cell;
}

// Добавить объект в комнату
void Maze::pushCellObject(std::uint16_t x, std::uint16_t y, Object item) {
	if (x < width && y < height)
		field[x][y].pushObject(item);
}

// Взять объект из комнаты
Object Maze::peekCellObject(std::uint16_t x, std::uint16_t y, Object item) const {
	Object peek_item;

	if (x < width && y < height)
		peek_item = field[x][y].peekObject(item);

	return peek_item;
}

// Получить объекты комнаты
std::vector<Object> Maze::getCellObjects(std::uint16_t x, std::uint16_t y) const {
	std::vector<Object> items;

	if (x < width && y < height)
		items = field[x][y].getObjects();

	return items;
}

// Очистить объекты комнаты
void Maze::clearCellObjects(std::uint16_t x, std::uint16_t y) {
	if (x < width && y < height)
		field[x][y].clearObjects();
}

// Установить тип комнаты
void Maze::setCellType(std::uint16_t x, std::uint16_t y, CellType type) {
	if (x < width && y < height)
		field[x][y].setType(type);
}

// Получить тип комнаты
CellType Maze::getCellType(std::uint16_t x, std::uint16_t y) const {
	CellType type = CellType::NONE;

	if (x < width && y < height)
		type = field[x][y].getType();

	return type;
}

// Сгенерировать лабиринт
void Maze::generate() {
	// Определение начальной точки движения генерации
	std::uint16_t x_start = rand(0, width - 1);
	std::uint16_t y_start = rand(0, height - 1);

	// Добавление точку в "маршрут"
	std::stack<Cell*> path({&field[x_start][y_start]});

	// Пока "маршрут" не пустой
	while (!path.empty()) {
		// Берём верхнюю точку из маршрута
		Cell* cur_cell = path.top();
		std::vector<Cell*> neighbors;

		// Находим непосещённых соседей текущей точки
		if (0 < cur_cell->getX() && !field[cur_cell->getX() - 1][cur_cell->getY()].isVisited())
			neighbors.push_back(&field[cur_cell->getX() - 1][cur_cell->getY()]);

		if (cur_cell->getX() < width - 1 && !field[cur_cell->getX() + 1][cur_cell->getY()].isVisited())
			neighbors.push_back(&field[cur_cell->getX() + 1][cur_cell->getY()]);

		if (0 < cur_cell->getY() && !field[cur_cell->getX()][cur_cell->getY() - 1].isVisited())
			neighbors.push_back(&field[cur_cell->getX()][cur_cell->getY() - 1]);

		if (cur_cell->getY() < height - 1 && !field[cur_cell->getX()][cur_cell->getY() + 1].isVisited())
			neighbors.push_back(&field[cur_cell->getX()][cur_cell->getY() + 1]);

		// Если у точки есть непосещённые соседи
		if (!neighbors.empty()) {
			// Выбираем случайного соседа и получаем направление в котором он находится
			Cell* next_cell = neighbors[rand(0, static_cast<uint16_t>(neighbors.size()) - 1)];
			CellDirection direction = getDirection(cur_cell->getX(), cur_cell->getY(), 
												   next_cell->getX(), next_cell->getY());
			
			// Отрываем направления (двери) между текущей точкой и выбранным соседом
			switch (direction) {
				case CellDirection::NORTH: {
					cur_cell->open(CellDirection::NORTH);
					next_cell->open(CellDirection::SOUTH);
					break;
				}
				case CellDirection::EAST: {
					cur_cell->open(CellDirection::EAST);
					next_cell->open(CellDirection::WEST);
					break;
				}
				case CellDirection::SOUTH: {
					cur_cell->open(CellDirection::SOUTH);
					next_cell->open(CellDirection::NORTH);
					break;
				}
				case CellDirection::WEST: {
					cur_cell->open(CellDirection::WEST);
					next_cell->open(CellDirection::EAST);
					break;
				}
			}

			// Добавлеяем соседа 
			path.push(next_cell);
		}
		else
			path.pop();

		// Если доступных соседей нет -- выбираем точку из маршрута
	}
}

void Maze::clear() {
	for (std::uint16_t i = 0; i != width; ++i)
		delete[] field[i];
	delete[] field;
}

Maze::~Maze() {
	clear();
}

// Получение случайного числа в диапазоне
std::uint16_t rand(std::uint16_t beg, std::uint16_t end) {
	std::mt19937 engine(std::random_device{}());
	std::uniform_int_distribution<std::mt19937::result_type> distance(beg, end);

	return distance(engine);
}
#ifndef OBJECT_H
#define OBJECT_H

#include <string>

// Тип игрового объекта,
// расположенного в комнатах или инвентаре Героя
enum class ObjectType {
	WEAPON,				// Оружине
	TORCH,				// Освещение
	KEY,				// Ключ
	CHEST,				// Сундук
	FOOD,				// Еда
	MONSTER,			// Монстер
	DIRECTION,			// Направление движения
	GOLD,				// Деньги
	EMPTY				// Пустой объект
};

struct Object {
	std::string name;						// Название объекта
	ObjectType type = ObjectType::EMPTY;	// Тип объекта
};

// Перегрузка операции сравнения для игрового объекта
static inline bool operator==(const Object &lhs, const Object &rhs) {
	return lhs.name == rhs.name && lhs.type == rhs.type;
}

#endif
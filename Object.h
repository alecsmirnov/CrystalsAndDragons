#ifndef OBJECT_H
#define OBJECT_H

#include <string>

enum class ObjectType {
	ITEM,
	WEAPON,
	TORCH,
	CHEST,
	FOOD,
	MONSTER,
	NONE
};

struct Object {
	std::string name;
	ObjectType type;
};

static inline bool operator==(const Object &lhs, const Object &rhs) {
	return lhs.name == rhs.name && lhs.type == rhs.type;
}

#endif
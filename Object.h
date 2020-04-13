#ifndef OBJECT_H
#define OBJECT_H

#include <string>

// ��� �������� �������,
// �������������� � �������� ��� ��������� �����
enum class ObjectType {
	WEAPON,				// �������
	TORCH,				// ���������
	KEY,				// ����
	CHEST,				// ������
	FOOD,				// ���
	MONSTER,			// �������
	DIRECTION,			// ����������� ��������
	GOLD,				// ������
	EMPTY				// ������ ������
};

struct Object {
	std::string name;						// �������� �������
	ObjectType type = ObjectType::EMPTY;	// ��� �������
};

// ���������� �������� ��������� ��� �������� �������
static inline bool operator==(const Object &lhs, const Object &rhs) {
	return lhs.name == rhs.name && lhs.type == rhs.type;
}

#endif
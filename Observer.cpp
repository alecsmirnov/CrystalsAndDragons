#include "Observer.h"

void Supervised::addObserver(Observer* observer) {
	observers.push_back(observer);
}

void Supervised::notify() {
	for (auto observer : observers)
		observer->update();
}

Supervised::~Supervised() {
	std::vector<Observer*>().swap(observers);
}
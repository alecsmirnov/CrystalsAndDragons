#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>

class Observer {
public:
	virtual void update() = 0;
};

class Supervised {
public:
	Supervised() = default;

	void addObserver(Observer* observer);

	void notify();

	virtual ~Supervised();

private:
	std::vector<Observer*> observers;
};

#endif

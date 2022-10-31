/*
 * EventQueue.h
 *
 *  Created on: 20 Oct 2022
 *      Author: ahmed
 */

#ifndef EVENTQUEUE_H_
#define EVENTQUEUE_H_


#include <queue>
#include "MenuItem.h"

/**
 * @brief Short class implementing EventQueue for handling menu items
 */
class EventQueue {
public:
	EventQueue() {};
	~EventQueue() {};
	void publish(MenuItem::menuEvent event) {
		queue.push(event);
	}
	MenuItem::menuEvent consume() {
		MenuItem::menuEvent event(queue.front());
		queue.pop();
		return event;
	}
	bool pending() {
		return !queue.empty();
	}
private:
	std::queue<MenuItem::menuEvent> queue;
};


#endif /* EVENTQUEUE_H_ */

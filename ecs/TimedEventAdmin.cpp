//
// Created by seble on 02/02/2020.
//

#include "TimedEventAdmin.hpp"
#include <thread>

TimedEventAdmin::TimedEventAdmin():_manager(TimedEvent::get()) {

}

void TimedEventAdmin::addEvent(long time, TimeUnit unit, std::function<void()> &function) {
	_manager.addEvent(time, unit, function);
}
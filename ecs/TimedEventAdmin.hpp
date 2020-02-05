//
// Created by seble on 02/02/2020.
//

#pragma once

#include "TimedEvent.hpp"

class TimedEventAdmin {
public:
	TimedEventAdmin();

	void addEvent(long time, TimeUnit unit, std::function<void()> &function);

private:
	TimedEvent &_manager;

};




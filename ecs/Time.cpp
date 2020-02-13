//
// Created by seble on 02/02/2020.
//

#include "Time.hpp"

long Time::get(TimeUnit unit) {
	static Time time;

	return time.getTime(unit);
}

long Time::getTime(TimeUnit unit) {
	if (unit == TimeUnit::Hours)
		return (long) (std::chrono::duration_cast<std::chrono::hours>(_clock.now().time_since_epoch()).count());
	else if (unit == TimeUnit::Minutes)
		return (long) (std::chrono::duration_cast<std::chrono::minutes>(_clock.now().time_since_epoch()).count());
	else if (unit == TimeUnit::Seconds)
		return (long) (std::chrono::duration_cast<std::chrono::seconds>(_clock.now().time_since_epoch()).count());
	else if (unit == TimeUnit::MilliSeconds)
		return (long) (std::chrono::duration_cast<std::chrono::milliseconds>(_clock.now().time_since_epoch()).count());
	else if (unit == TimeUnit::MicroSeconds)
		return (long) (std::chrono::duration_cast<std::chrono::microseconds>(_clock.now().time_since_epoch()).count());
	else
		return (long) (std::chrono::duration_cast<std::chrono::nanoseconds>(_clock.now().time_since_epoch()).count());
}
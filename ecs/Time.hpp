//
// Created by seble on 02/02/2020.
//

#pragma once

#include <chrono>

enum class TimeUnit {
	Hours,
	Minutes,
	Seconds,
	MilliSeconds,
	MicroSeconds,
	NanoSeconds
};

namespace ecs {
	class Time {
	public:

		static long get(TimeUnit unit);

		long got(TimeUnit unit);

	private:
		std::chrono::high_resolution_clock	_clock;
	};
}
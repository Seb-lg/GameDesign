//
// Created by seb on 03/03/2020.
//

#pragma once

#include <ecs/Entity.hpp>
#include <vector>

class Flock {
public:
	Flock(float distance = 0.5f);

public:
	float distance;
	std::vector<ID> childs;
};


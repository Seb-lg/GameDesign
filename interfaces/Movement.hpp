//
// Created by seb on 26/02/2020.
//

#pragma once

#include <ecs/Entity.hpp>

#include <vector>

class Movement {
public:
	static void ZQSD(ID id);
	static void WASD(ID id);
	static void ARROW(ID id);

private:
	static void fromKeys(ID id, std::vector<int> keys);
};

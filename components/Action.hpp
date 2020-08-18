//
// Created by lulutor on 18/08/2020.
//
#include <ecs/Entity.hpp>

#include <vector>
#include "Shapes.hpp"

class Shoot{
public:
	static void ZQSD(ID id);
	static void WASD(ID id);
	static void ARROW(ID id);

private:
	static void fromKeys(ID id,GraphicalObject player, std::vector<int> keys);
};
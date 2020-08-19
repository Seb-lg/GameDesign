//
// Created by lulutor on 18/08/2020.
//
#include <ecs/Entity.hpp>

#include <vector>
#include "components/Shapes.hpp"

class Shoot{
public:
	static void ZQSD(ID id);
	static void WASD(ID id);
	static void ARROW(ID id);

private:
	static void fromKeys(ID id,std::vector<int> keys);
};
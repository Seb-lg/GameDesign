//
// Created by seb on 29/02/2020.
//

#pragma once

#include <vector>
#include <glm/glm.hpp>

class Hitbox {
public:
	Hitbox() = default;
	Hitbox(std::vector<glm::vec3> vertexs);
	float maxX;
	float minX;
	float maxY;
	float minY;
	float maxZ;
	float minZ;
};

//
// Created by seb on 29/02/2020.
//

#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Position3D.hpp"

class Hitbox {
public:
	Hitbox() = default;
	Hitbox(std::vector<glm::vec3> vertexs, glm::vec3 scale = DEFAULTSCALE);
	float maxX;
	float minX;
	float maxY;
	float minY;
	float maxZ;
	float minZ;
};
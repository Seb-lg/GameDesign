//
// Created by seb on 25/02/2020.
//

#pragma once

#include <glm/glm.hpp>

#define DEFAULTTRANS {0.f, 0.f, 0.f}
#define DEFAULTROT {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}
#define DEFAULTSCALE {1.f, 1.f, 1.f}

class Position3D {
public:
	Position3D(glm::vec3 trans = DEFAULTTRANS, glm::mat4 rot = DEFAULTROT, glm::vec3 scale = DEFAULTSCALE);

public:
	glm::mat4 mv_matrix;
	glm::vec3 trans;
	glm::mat4 rot;
	glm::vec3 scale;
};

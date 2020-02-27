//
// Created by seb on 26/02/2020.
//

#pragma once

#include <glm/vec3.hpp>

class Speed3D {
public:
	Speed3D();
	Speed3D(glm::vec3 direction, float speed);

public:
	glm::vec3 direction;
	float speed;
};

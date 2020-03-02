//
// Created by seb on 29/02/2020.
//

#include <iostream>
#include "Hitbox.hpp"

Hitbox::Hitbox(std::vector<glm::vec3> vertexs, glm::vec3 scale) {
	maxX = minX = maxY = minY = maxZ = minZ = 0.f;

	for (auto &elem : vertexs) {
		if (maxX < elem.x)
			maxX = elem.x;
		if (minX > elem.x)
			minX = elem.x;

		if (maxY < elem.y)
			maxY = elem.y;
		if (minY > elem.y)
			minY = elem.y;

		if (maxZ < elem.z)
			maxZ = elem.z;
		if (minZ > elem.z)
			minZ = elem.z;
	}

	maxX *= scale.x;
	minX *= scale.x;
	maxY *= scale.y;
	minY *= scale.y;
	maxZ *= scale.z;
	minZ *= scale.z;
}

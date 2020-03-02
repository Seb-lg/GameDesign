//
// Created by seb on 25/02/2020.
//

#include "Position3D.hpp"

Position3D::Position3D(glm::vec3 trans, glm::mat4 rot, glm::vec3 scale): mv_matrix(1.f), trans(trans), rot(rot), scale(scale) {
}

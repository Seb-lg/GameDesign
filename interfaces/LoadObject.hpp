//
// Created by seb on 03/02/2020.
//

#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <components/Position3D.hpp>

class LoadObject {
public:
	/// Create a Cube using the ECS
	/// \return the ID of the created Cube
	static ID Cube(glm::vec3 trans = DEFAULTTRANS, glm::mat4 rot = DEFAULTROT, glm::vec3 scale = DEFAULTSCALE, ID parent = 0);

	/// Create a Sphere using the ECS
	/// \return the ID of the created Sphere
	static ID Sphere(glm::vec3 trans = DEFAULTTRANS, glm::mat4 rot = DEFAULTROT, glm::vec3 scale = DEFAULTSCALE, ID parent = 0);

	/// Create a Arrow using the ECS
	/// \return the ID of the created Arrow
	static ID Arrow(glm::vec3 trans = DEFAULTTRANS, glm::mat4 rot = DEFAULTROT, glm::vec3 scale = DEFAULTSCALE, ID parent = 0);

	/// Create a Cylinder using the ECS
	/// \return the ID of the created Cylinder
	static ID Cylinder(glm::vec3 trans = DEFAULTTRANS, glm::mat4 rot = DEFAULTROT, glm::vec3 scale = DEFAULTSCALE, ID parent = 0);

	/// Create a Line using the ECS
	/// \return the ID of the created Line
	static ID Line(glm::vec3 trans = DEFAULTTRANS, glm::mat4 rot = DEFAULTROT, glm::vec3 scale = DEFAULTSCALE, ID parent = 0);

	/// Create a 3D Object from a given file
	/// \param path to the file where the object is stored
	/// \return the ID of the created 3D Object
	static ID FromSource(std::string path, glm::vec3 trans = DEFAULTTRANS, glm::mat4 rot = DEFAULTROT, glm::vec3 scale = DEFAULTSCALE, ID parent = 0);
};
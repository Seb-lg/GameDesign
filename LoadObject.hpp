//
// Created by seb on 03/02/2020.
//

#pragma once

#include <string>

class LoadObject {
public:
	/// Create a Cube using the ECS
	/// \return the ID of the created Cube
	static ID Cube();

	/// Create a Sphere using the ECS
	/// \return the ID of the created Sphere
	static ID Sphere();

	/// Create a Arrow using the ECS
	/// \return the ID of the created Arrow
	static ID Arrow();

	/// Create a Cylinder using the ECS
	/// \return the ID of the created Cylinder
	static ID Cylinder();

	/// Create a Line using the ECS
	/// \return the ID of the created Line
	static ID Line();

	/// Create a 3D Object from a given file
	/// \param path to the file where the object is stored
	/// \return the ID of the created 3D Object
	static ID FromSource(std::string path);
};
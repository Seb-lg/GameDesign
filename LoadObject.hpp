//
// Created by seb on 03/02/2020.
//

#pragma once

#include <string>

class LoadObject {
public:
	static ID Cube();
	static ID Sphere();
	static ID Arrow();
	static ID Cylinder();
	static ID Line();
	static ID FromSource(std::string path);
};
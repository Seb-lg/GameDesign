#pragma once

#include <iostream>
#include <vector>

using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Shapes {

public:
	Shapes();

	~Shapes();

	void Load();

	void Draw();

	void checkErrorShader(GLuint shader);

	vector<GLfloat> vertexPositions;

	GLuint program;
	GLuint vao;
	GLuint buffer;
	GLint mv_location;
	GLint proj_location;
	GLint color_location;
	glm::mat4 proj_matrix = glm::mat4(1.0f);
	glm::mat4 mv_matrix = glm::mat4(1.0f);

	glm::vec4 fillColor = glm::vec4(1.0, 0.0, 0.0, 1.0);
	glm::vec4 lineColor = glm::vec4(0.0, 0.0, 0.0, 1.0);
	float lineWidth = 2.0f;

protected:
	string rawData;                        // Import obj file from Blender (note: no textures or UVs).
	void LoadObj();
};

class GraphicalObject : public Shapes {
public:
	GraphicalObject() {

		// Exported from Blender a cube by default (OBJ File)
		rawData = R"(
v 0.500000 -0.500000 -0.500000
v 0.500000 -0.500000 0.500000
v -0.500000 -0.500000 0.500000
v -0.500000 -0.500000 -0.500000
v 0.500000 0.500000 -0.499999
v 0.499999 0.500000 0.500000
v -0.500000 0.500000 0.500000
v -0.500000 0.500000 -0.500000
f 1 3 4
f 8 6 5
f 5 2 1
f 6 3 2
f 7 4 3
f 1 8 5
f 1 2 3
f 8 7 6
f 5 6 2
f 6 7 3
f 7 8 4
f 1 4 8)";

		LoadObj();
	}

	GraphicalObject(std::string data);

	~GraphicalObject() {}
};

class Cube : public Shapes {
public:
	Cube();

	~Cube();
};

class Sphere : public Shapes {
public:
	Sphere();

	~Sphere();
};

class Arrow : public Shapes {
public:
	Arrow();

	~Arrow();
};

class Cylinder : public Shapes {
public:
	Cylinder();

	~Cylinder();
};

class Line : public Shapes {
public:
	Line();

	~Line();
};
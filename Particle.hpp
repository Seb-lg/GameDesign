//
// Created by seb on 06/02/2020.
//

#pragma once

/*#include <glm/ext.hpp>
#include <GL/glew.h>*/

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <string>

#define FOURCC_DXT1 0x31545844
#define FOURCC_DXT3 0x33545844
#define FOURCC_DXT5 0x35545844

static const GLfloat g_vertex_buffer_data[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,
};

struct Particle {
	glm::vec3 pos;
	glm::vec3 speed;
	unsigned char r,g,b,a; // Color
	float size, angle, weight;
	float life = -1.f; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	Particle() = default;
	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

class ParticleEmitter {
public:
	ParticleEmitter(GLuint size = 10000);

	void update(glm::mat4 viewMatrix, glm::mat4 projMatrix);
	void loadShaders(std::string vertex, std::string fragment);
	GLuint loadTexture(const char * imagepath);
	int FindUnusedParticle();

private:
	double lastTimes;
	int LastUsedParticle = 0;
	GLuint VertexArrayID;
	GLuint CameraRight_worldspace_ID;
	GLuint CameraUp_worldspace_ID;
	GLuint ViewProjMatrixID;

	GLuint program;
	GLuint textureID;
	GLuint texture;

	GLuint size;

	GLuint billboard_vertex_buffer;
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;

	std::vector<Particle> particles;
	GLfloat* particulePositionData;
	GLubyte* particuleColorData;
};

struct BITMAPFILEHEADER {
	unsigned short  bfType;
	unsigned long bfSize;
	unsigned short  bfReserved1;
	unsigned short  bfReserved2;
	unsigned long bfOffBits;
};
struct BITMAPINFOHEADER {
	unsigned long biSize;
	long  biWidth;
	long  biHeight;
	unsigned short  biPlanes;
	unsigned short  biBitCount;
	unsigned long biCompression;
	unsigned long biSizeImage;
	long  biXPelsPerMeter;
	long  biYPelsPerMeter;
	unsigned long biClrUsed;
	unsigned long biClrImportant;
};
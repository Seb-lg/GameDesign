//
// Created by seb on 06/02/2020.
//

#include "Particle.hpp"
#include "Time.hpp"
#include "Config.hpp"

#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <sstream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <GLFW/glfw3.h>
#include <thread>
#include <list>

ParticleEmitter::ParticleEmitter(GLuint size, bool autoEmit) : size(size), autoEmit(autoEmit) {
	lastTimes = glfwGetTime();
	loadShaders( "assets/Particle.vertexshader", "assets/Particle.fragmentshader" );
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	CameraRight_worldspace_ID  = glGetUniformLocation(program, "CameraRight_worldspace");
	CameraUp_worldspace_ID  = glGetUniformLocation(program, "CameraUp_worldspace");
	ViewProjMatrixID = glGetUniformLocation(program, "VP");

	textureID  = glGetUniformLocation(program, "sampleTexture");


	this->particulePositionData	= new GLfloat[this->size * 4];
	this->particuleColorData	= new GLubyte[this->size * 4];

	this->particles.reserve(size);
	for (GLuint i = 0; i < size; ++i)
		this->particles.emplace_back();

	glUseProgram(program);
	texture = loadTexture("assets/dollarwhite.bmp");
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, size * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, size * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glLinkProgram(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);

}

void ParticleEmitter::update(glm::mat4 viewMatrix, glm::mat4 projMatrix, glm::vec3 pos) {
	double currentTime = glfwGetTime();
	double delta = currentTime - lastTimes;
	lastTimes = currentTime;

	glUseProgram(program);
	glBindVertexArray(VertexArrayID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glm::mat4 ViewProjectionMatrix = projMatrix * viewMatrix;
	glm::vec3 CameraPosition(glm::inverse(viewMatrix)[3]);

	if (autoEmit) {
		int newparticles = (int)(delta*(float)size);
		if (newparticles > (int)(0.016f*(float)size))
			newparticles = (int)(0.016f*(float)size);

		int particleIndex = FindUnusedParticle();
		for(int i=0; i<newparticles && particleIndex != -1; i++){
			particleIndex = FindUnusedParticle();
			particles[particleIndex].life = rand() % 7; // This particle will live 5 seconds.
			particles[particleIndex].pos = pos;

			float spread = 1.5f;
			glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);
			glm::vec3 randomdir = glm::vec3(
				(rand()%2000 - 1000.0f)/1000.0f,
				(rand()%2000 - 1000.0f)/1000.0f,
				(rand()%2000 - 1000.0f)/1000.0f
			);

			particles[particleIndex].speed = maindir + randomdir*spread;

			// Very bad way to generate a random color
			particles[particleIndex].r = 133;//rand() % 256;
			particles[particleIndex].g = 187;//rand() % 256;
			particles[particleIndex].b = 101;//rand() % 256;
			particles[particleIndex].a = 255;//(rand() % 256) / 3;

			particles[particleIndex].size = (rand()%1000)/2000.0f + 0.1f;

		}
	}

	int ParticlesCount = 0;
	for(int i=0; i<size; i++){

		Particle& p = particles[i]; // shortcut

		if(p.life > 0.0f){

			// Decrease life
			p.life -= delta;
			if (p.life > 0.0f){

				p.speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)delta * 0.5f;
				p.pos += p.speed * (float)delta;
				p.cameradistance = glm::length2( p.pos - CameraPosition );

				particulePositionData[4*i+0] = p.pos.x;
				particulePositionData[4*i+1] = p.pos.y;
				particulePositionData[4*i+2] = p.pos.z;

				particulePositionData[4*i+3] = p.size;

				particuleColorData[4*i+0] = p.r;
				particuleColorData[4*i+1] = p.g;
				particuleColorData[4*i+2] = p.b;
				particuleColorData[4*i+3] = p.a;

			} else {
				p.cameradistance = - 1.0f;
			}

			ParticlesCount++;

		}
	}

	std::sort(&particles[0], &particles[size]);

	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, size * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, particulePositionData);

	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, size * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, particuleColorData);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	GLuint TextureID  = glGetUniformLocation(program, "sampleTexture");
	glUniform1i(TextureID, 0);

	glUniform3f(CameraRight_worldspace_ID, viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
	glUniform3f(CameraUp_worldspace_ID   , viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

	glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glVertexAttribPointer(
		1,
		4,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glVertexAttribPointer(
		2,
		4,
		GL_UNSIGNED_BYTE,
		GL_TRUE,
		0,
		(void*)0
	);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void ParticleEmitter::loadShaders(std::string vertex, std::string fragment) {
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex.c_str(), std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		getchar();
		return;
	}

	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment.c_str(), std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	}

	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	}

	program = glCreateProgram();
	glUseProgram(program);
	glAttachShader(program, VertexShaderID);
	glAttachShader(program, FragmentShaderID);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &Result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	}

	glDetachShader(program, VertexShaderID);
	glDetachShader(program, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
}

GLuint ParticleEmitter::loadTexture(const char *imagepath) {
	GLuint _texture;
	unsigned char header[54];
	unsigned int imageSize;
	unsigned int width;
	unsigned int height;
	unsigned char *data;

	FILE * file = fopen(imagepath ,"rb");
	if (!file)
		return 0;

	if (fread(header, 1, 54, file)!=54 || (header[0]!='B' || header[1]!='M')) {
		fclose(file);
		return 0;
	}

	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);
	if (imageSize==0)
		imageSize=width*height*4;

	data = new unsigned char [imageSize];
	fread(data,1,imageSize,file);
	fclose (file);
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	delete [] data;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	return _texture;
}

int ParticleEmitter::FindUnusedParticle() {

	for(int i=LastUsedParticle; i<size; i++){
		if (particles[i].life <= 0.f){
			LastUsedParticle = i;
			return i;
		}
	}

	for(int i=0; i<LastUsedParticle; i++){
		if (particles[i].life <= 0.f){
			LastUsedParticle = i;
			return i;
		}
	}
	return -1;
}

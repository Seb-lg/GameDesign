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

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint loadDDS(const char * imagepath){

	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(imagepath, "rb");
	if (fp == NULL){
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height      = *(unsigned int*)&(header[8 ]);
	unsigned int width	     = *(unsigned int*)&(header[12]);
	unsigned int linearSize	 = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC      = *(unsigned int*)&(header[80]);


	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch(fourCC)
	{
		case FOURCC_DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		case FOURCC_DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case FOURCC_DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		default:
			free(buffer);
			return 0;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
				       0, size, buffer + offset);

		offset += size;
		width  /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if(width < 1) width = 1;
		if(height < 1) height = 1;

	}

	free(buffer);

	return textureID;


}

ParticleEmitter::ParticleEmitter(GLuint size) : size(size) {
	lastTimes = glfwGetTime();
	loadShaders( "assets/Particle.vertexshader", "assets/Particle.fragmentshader" );
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	CameraRight_worldspace_ID  = glGetUniformLocation(program, "CameraRight_worldspace");
	CameraUp_worldspace_ID  = glGetUniformLocation(program, "CameraUp_worldspace");
	ViewProjMatrixID = glGetUniformLocation(program, "VP");

	textureID  = glGetUniformLocation(program, "myTextureSampler");


	this->particulePositionData	= new GLfloat[this->size * 4];
	this->particuleColorData	= new GLubyte[this->size * 4];

	this->particles.reserve(size);
	for (GLuint i = 0; i < size; ++i)
		this->particles.emplace_back();

	glUseProgram(program);
//	texture = loadTexture("assets/oui32.bmp");
	texture = loadDDS("assets/particle.DDS");
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

void ParticleEmitter::update(glm::mat4 viewMatrix, glm::mat4 projMatrix) {
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

	int newparticles = (int)(delta*(float)size);
	if (newparticles > (int)(0.016f*(float)size))
		newparticles = (int)(0.016f*(float)size);

	int particleIndex = FindUnusedParticle();
	for(int i=0; i<newparticles && particleIndex != -1; i++){
		particleIndex = FindUnusedParticle();
		particles[particleIndex].life = rand() % 7; // This particle will live 5 seconds.
		particles[particleIndex].pos = glm::vec3(0.f,0.f,0.0f);

		float spread = 1.5f;
		glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);
		glm::vec3 randomdir = glm::vec3(
			(rand()%2000 - 1000.0f)/1000.0f,
			(rand()%2000 - 1000.0f)/1000.0f,
			(rand()%2000 - 1000.0f)/1000.0f
		);

		particles[particleIndex].speed = maindir + randomdir*spread;

		// Very bad way to generate a random color
		particles[particleIndex].r = rand() % 256;
		particles[particleIndex].g = rand() % 256;
		particles[particleIndex].b = rand() % 256;
		particles[particleIndex].a = (rand() % 256) / 3;

		particles[particleIndex].size = (rand()%1000)/2000.0f + 0.1f;

	}
	int ParticlesCount = 0;

	/*std::vector<std::thread> pool;
	std::list<int> poolRet;
	int spart = size / NB_THREAD;
	auto it = particles.begin();
	int particle = 0;
	for (int i = 0; i < NB_THREAD - 1; ++i) {
		poolRet.push_back(0);
		int &ret = poolRet.back();
		auto end = std::next(it, spart);
		pool.emplace_back([this, &ret, it, end, delta, CameraPosition, particle]() mutable {
			for (it; it != end; ++it) {
				if(it->life > 0.0f){

					// Decrease life
					it->life -= delta;
					if (it->life > 0.0f){

						it->speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)delta * 0.5f;
						it->pos += it->speed * (float)delta;
						it->cameradistance = glm::length2( it->pos - CameraPosition );

						particulePositionData[4*particle+0] = it->pos.x;
						particulePositionData[4*particle+1] = it->pos.y;
						particulePositionData[4*particle+2] = it->pos.z;

						particulePositionData[4*particle+3] = it->size;

						particuleColorData[4*particle+0] = it->r;
						particuleColorData[4*particle+1] = it->g;
						particuleColorData[4*particle+2] = it->b;
						particuleColorData[4*particle+3] = it->a;

					} else {
						it->cameradistance = - 1.0f;
					}
					ret++;
				}
			}
			++particle;
		});
		particle += spart;
	}
	poolRet.push_back(0);
	int &ret = poolRet.back();
	auto end = particles.end();
	pool.emplace_back([this, &ret, it, end, delta, CameraPosition, particle]() mutable {
		for (it; it != end; ++it) {
			if(it->life > 0.0f){

				// Decrease life
				it->life -= delta;
				if (it->life > 0.0f){

					it->speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)delta * 0.5f;
					it->pos += it->speed * (float)delta;
					it->cameradistance = glm::length2( it->pos - CameraPosition );

					particulePositionData[4*particle+0] = it->pos.x;
					particulePositionData[4*particle+1] = it->pos.y;
					particulePositionData[4*particle+2] = it->pos.z;

					particulePositionData[4*particle+3] = it->size;

					particuleColorData[4*particle+0] = it->r;
					particuleColorData[4*particle+1] = it->g;
					particuleColorData[4*particle+2] = it->b;
					particuleColorData[4*particle+3] = it->a;

				} else {
					it->cameradistance = - 1.0f;
				}
				ret++;
			}
		}
		++particle;
	});
	for (auto &elem : pool)
		elem.join();

	for (auto elem : poolRet)
		ParticlesCount += elem;*/

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


//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	GLuint TextureID  = glGetUniformLocation(program, "myTextureSampler");
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
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex.c_str(), std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex.c_str());
		getchar();
		return;
	}

	// Read the Fragment Shader code from the file
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


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex.c_str());
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment.c_str());
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	program = glCreateProgram();
	glUseProgram(program);
	glAttachShader(program, VertexShaderID);
	glAttachShader(program, FragmentShaderID);
	glLinkProgram(program);

	// Check the program
	glGetProgramiv(program, GL_LINK_STATUS, &Result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(program, VertexShaderID);
	glDetachShader(program, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
}

GLuint ParticleEmitter::loadTexture(const char *imagepath) {
	GLuint _texture;
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// RGB data
	unsigned char *data;

	FILE * file = fopen(imagepath ,"rb");
	if (!file)
		return 0;
//		return printError("Impossible to open file!");

	// Read the header, i.e. the 54 first bytes
	// If less than 54 bytes are read, problem
	if (fread(header, 1, 54, file)!=54 ||
	    // A BMP files always begins with "BM"
	    (header[0]!='B' || header[1]!='M') /*||
	    // Make sure this is a 24bpp file
	    *(int*)&(header[0x1E])!=0 || *(int*)&(header[0x1C])!=24*/) {
//		printError("Not a correct BMP file (24 bits encoding needed)\n");
		std::cout << "jtebaise" << std::endl;
		fclose(file);
		return 0;
	}

	// Read the information about the image
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0) // 3 : one byte for each Red, Green and Blue component
		imageSize=width*height*4;
	if (dataPos==0)  // The BMP header is done that way
		dataPos=54;

	// Create a buffer
	data = new unsigned char [imageSize];
	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);
	// Everything is in memory now, the file can be closed.
	fclose (file);
	// Create one OpenGL texture
	glGenTextures(1, &_texture);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, _texture);
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	// OpenGL has now copied the data. Free our own version
	delete [] data;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (true) {  // Trilinear filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		// Requires mipmaps. Generate them automatically.
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	return _texture;
}

int ParticleEmitter::FindUnusedParticle(){

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

	return -1; // All particles are taken, override the first one
}

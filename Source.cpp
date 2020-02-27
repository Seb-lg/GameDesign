// Simplified Renderer application for GP course
// Features:
// Reduced OpenGL version from 4.5 to 3.3 to allow it to render in older laptops.
// Added Shapes library for rendering cubes, spheres and vectors.
// Added examples of matrix multiplication on Update.
// Added resize screen and keyboard callbacks.
// Added FPS camera functionality
// Update 2019/01 updated libraries and created project for VS2017 including directory dependant links to libraries.
// Update 2020/01 updated libraries for x64 and for VS2020, also adding MAC compiled Libraries.

// Suggestions or extra help please do email me at S.Padilla@hw.ac.uk

// Standard C++ libraries
#include <iostream>
#include <vector>

using namespace std;

#include "Ecs.hpp"
#include "interfaces/LoadObject.hpp"

// Helper graphic libraries
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/transform.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <ecs/Time.hpp>
#include <interfaces/Movement.hpp>
#include <components/Speed3D.hpp>
#include <components/Keyboard.hpp>
#include "Graphics.hpp"
#include "components/Shapes.hpp"
#include "components/Particle.hpp"

// MAIN FUNCTIONS
void startup();
void updateCamera();
void renderScene();
// CALLBACK FUNCTIONS
void onResizeCallback(GLFWwindow *window, int w, int h);
void onKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void onMouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
void onMouseMoveCallback(GLFWwindow *window, double x, double y);
void onMouseWheelCallback(GLFWwindow *window, double xoffset, double yoffset);

// VARIABLES
float deltaTime = 0.0f;    // Keep track of time per frame.
float lastTime = 0.0f;    // variable to keep overall time.
bool *keyStatus;    // Hold key status.
bool mouseEnabled = true; // keep track of mouse toggle.

// MAIN GRAPHICS OBJECT
Graphics myGraphics;
ParticleEmitter *emit;

// Some global variable to do the animation.
float t = 0.001f;
int main(int argc, char* argv[]) {
	keyStatus = new bool[1024];
	if (myGraphics.Init()) return 0;
	Ecs &ecs = Ecs::get();
	startup();

	emit = new ParticleEmitter();
	ID id = LoadObject::Cube({0.f, 0.5f, 2.f});
	Movement::WASD(id);

	while (glfwWindowShouldClose(myGraphics.window) != GL_TRUE) {
		ecs.update();
	}

	myGraphics.endProgram();
	return 0;
}

void startup() {
	Ecs &ecs = Ecs::get();
	auto &obj = ecs.getComponentMap<GraphicalObject>();
	ecs.addUpdate(01, []() { updateCamera(); });
	ecs.addUpdate(02, []() { glfwPollEvents(); });
	ecs.addUpdate(10, [&]() {
		auto &ecs = Ecs::get();
		auto &keybs = ecs.getComponentMap<Keyboard>();

		for (auto &keyb: keybs) {
			auto it = keyb.second.keys.begin();
			for (int i = 0; i < 1024; ++i, ++it) {
				if (*it) {
					(*it)(keyStatus[i]);
				}
			}
		}
	});
	ecs.addUpdate(12, [&]() {
		auto &ecs = Ecs::get();
		auto &speeds = ecs.getComponentMap<Speed3D>();
		auto &positions = ecs.getComponentMap<Position3D>();
		auto ids = ecs.filter<Speed3D, Position3D>();

		for (const auto &id: ids) {
			auto &dir = speeds[id].direction;
			auto &speed = speeds[id].speed;
			auto &pos = positions[id].trans;
			float total = abs(dir.x) + abs(dir.y) + abs(dir.z);

			if (total == 0.f) {
				return;
			}

			std::cout << total << std::endl;
			pos.x += (dir.x == 0.f? 0.f: speed * deltaTime * (total /dir.x));
			pos.y += (dir.y == 0.f? 0.f: speed * deltaTime * (total /dir.y));
			pos.z += (dir.z == 0.f? 0.f: speed * deltaTime * (total /dir.z));
		}
	});
	ecs.addUpdate(21, []() { renderScene(); });
	ecs.addUpdate(22, [&]() {
		auto &ecs = Ecs::get();
		auto &positions = ecs.getComponentMap<Position3D>();
		auto &graphicals = ecs.getComponentMap<GraphicalObject>();

		for (auto &graphical: graphicals) {
			graphical.second.mv_matrix = positions[graphical.first].mv_matrix;
			graphical.second.proj_matrix = myGraphics.projMatrix;
			graphical.second.Draw();
		}
	});
	ecs.addUpdate(23, [&]() {
		emit->update(myGraphics.viewMatrix, myGraphics.projMatrix);
	});
	ecs.addUpdate(31, []() { glfwSwapBuffers(myGraphics.window); });

	// Keep track of the running time
	GLfloat currentTime = (GLfloat) glfwGetTime();
	deltaTime = currentTime;
	lastTime = currentTime;

	glfwSetWindowSizeCallback(myGraphics.window, onResizeCallback);
	glfwSetKeyCallback(myGraphics.window, onKeyCallback);
	glfwSetMouseButtonCallback(myGraphics.window, onMouseButtonCallback);
	glfwSetCursorPosCallback(myGraphics.window, onMouseMoveCallback);
	glfwSetScrollCallback(myGraphics.window, onMouseWheelCallback);

	// Calculate proj_matrix for the first time.
	myGraphics.aspect = (float) myGraphics.windowWidth / (float) myGraphics.windowHeight;
	myGraphics.projMatrix = glm::perspective(glm::radians(50.0f), myGraphics.aspect, 0.1f, 1000.0f);

	ID myFloor = LoadObject::Cube(glm::vec3(0.0f, 0.0f, 0.0f), DEFAULTROT, glm::vec3(1000.0f, 0.001f, 1000.0f));
	obj[myFloor].fillColor = glm::vec4(130.0f / 255.0f, 96.0f / 255.0f, 61.0f / 255.0f, 1.0f);
	obj[myFloor].lineColor = glm::vec4(130.0f / 255.0f, 96.0f / 255.0f, 61.0f / 255.0f, 1.0f);

	// Optimised Graphics
	myGraphics.SetOptimisations();        // Cull and depth testing
}

void updateCamera() {

	// calculate movement for FPS camera
	GLfloat xoffset = myGraphics.mouseX - myGraphics.cameraLastX;
	GLfloat yoffset = myGraphics.cameraLastY - myGraphics.mouseY;    // Reversed mouse movement
	myGraphics.cameraLastX = (GLfloat) myGraphics.mouseX;
	myGraphics.cameraLastY = (GLfloat) myGraphics.mouseY;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	myGraphics.cameraYaw += xoffset;
	myGraphics.cameraPitch += yoffset;

	// check for pitch out of bounds otherwise screen gets flipped
	if (myGraphics.cameraPitch > 89.0f) myGraphics.cameraPitch = 89.0f;
	if (myGraphics.cameraPitch < -89.0f) myGraphics.cameraPitch = -89.0f;

	// Calculating FPS camera movement (See 'Additional Reading: Yaw and Pitch to Vector Calculations' in VISION)
	glm::vec3 front;
	front.x = cos(glm::radians(myGraphics.cameraYaw)) * cos(glm::radians(myGraphics.cameraPitch));
	front.y = sin(glm::radians(myGraphics.cameraPitch));
	front.z = sin(glm::radians(myGraphics.cameraYaw)) * cos(glm::radians(myGraphics.cameraPitch));

	myGraphics.cameraFront = glm::normalize(front);

	// Update movement using the keys
	deltaTime = glfwGetTime() - lastTime;
	lastTime = glfwGetTime();
	GLfloat cameraSpeed = 3.1f * deltaTime;
	if (keyStatus[GLFW_KEY_W]) myGraphics.cameraPosition += cameraSpeed * myGraphics.cameraFront;
	if (keyStatus[GLFW_KEY_S]) myGraphics.cameraPosition -= cameraSpeed * myGraphics.cameraFront;
	if (keyStatus[GLFW_KEY_A])
		myGraphics.cameraPosition -=
			glm::normalize(glm::cross(myGraphics.cameraFront, myGraphics.cameraUp)) *
			cameraSpeed;
	if (keyStatus[GLFW_KEY_D])
		myGraphics.cameraPosition +=
			glm::normalize(glm::cross(myGraphics.cameraFront, myGraphics.cameraUp)) *
			cameraSpeed;
	//up N down
    if (keyStatus[GLFW_KEY_SPACE])
        myGraphics.cameraPosition +=cameraSpeed * myGraphics.cameraUp;
    if (keyStatus[GLFW_KEY_LEFT_CONTROL])
        myGraphics.cameraPosition -=cameraSpeed * myGraphics.cameraUp;

	// IMPORTANT PART
	// Calculate my view matrix using the lookAt helper function
	if (mouseEnabled) {
		myGraphics.viewMatrix = glm::lookAt(myGraphics.cameraPosition,                        // eye
						    myGraphics.cameraPosition +
						    myGraphics.cameraFront,                                        // centre
						    myGraphics.cameraUp);                                                                                                // up
	}
}

void recursion(glm::mat4 parent, std::unordered_map<ID, Position3D> &map, SceneTree& node){
	glm::mat4 mv_matrix =
		parent *
		glm::translate(map[node.id].trans) *
		map[node.id].rot *
		glm::scale(map[node.id].scale) *
		glm::mat4(1.0f);
	map[node.id].mv_matrix = myGraphics.viewMatrix * mv_matrix;
	/*map[node.id].proj_matrix = myGraphics.projMatrix;
	map[node.id].Draw();*/

	for (auto &elem : node.childs)
		recursion(mv_matrix, map, elem);
}

void renderScene() {
	myGraphics.ClearViewport();
	Ecs &ecs = Ecs::get();
	auto &obj = ecs.getComponentMap<Position3D>();

	for (auto &elem : ecs.tree.childs) {
		recursion(DEFAULTROT, obj, elem);
	}
}


// CallBack functions low level functionality.
void onResizeCallback(GLFWwindow *window, int w, int h) {    // call everytime the window is resized
	//myGraphics.windowWidth = w;
	//myGraphics.windowHeight = h;

	glfwGetFramebufferSize(window, &myGraphics.windowWidth, &myGraphics.windowHeight);

	myGraphics.aspect = (float) w / (float) h;
	myGraphics.projMatrix = glm::perspective(glm::radians(50.0f), myGraphics.aspect, 0.1f, 1000.0f);
}

void onKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) { // called everytime a key is pressed
	if (action == GLFW_PRESS) keyStatus[key] = true;
	else if (action == GLFW_RELEASE) keyStatus[key] = false;

	// toggle showing mouse.
	if (keyStatus[GLFW_KEY_M]) {
		mouseEnabled = !mouseEnabled;
		myGraphics.ToggleMouse();
	}
	// If exit key pressed.
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void onMouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {

}

void onMouseMoveCallback(GLFWwindow *window, double x, double y) {
	int mouseX = static_cast<int>(x);
	int mouseY = static_cast<int>(y);

	myGraphics.mouseX = mouseX;
	myGraphics.mouseY = mouseY;

	// helper variables for FPS camera
	if (myGraphics.cameraFirstMouse) {
		myGraphics.cameraLastX = (GLfloat) myGraphics.mouseX;
		myGraphics.cameraLastY = (GLfloat) myGraphics.mouseY;
		myGraphics.cameraFirstMouse = false;
	}
}

void onMouseWheelCallback(GLFWwindow *window, double xoffset, double yoffset) {
	int yoffsetInt = static_cast<int>(yoffset);
}

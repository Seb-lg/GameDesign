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
#include "LoadObject.hpp"

// Helper graphic libraries
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/transform.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <ecs/Time.hpp>
#include "Graphics.hpp"
#include "Shapes.hpp"
#include "Particle.hpp"

// MAIN FUNCTIONS
void startup();
void updateCamera();
void updateSceneElements();
void renderScene();
// CALLBACK FUNCTIONS
void onResizeCallback(GLFWwindow *window, int w, int h);
void onKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void onMouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
void onMouseMoveCallback(GLFWwindow *window, double x, double y);
void onMouseWheelCallback(GLFWwindow *window, double xoffset, double yoffset);

// VARIABLES
bool quit = false;
float deltaTime = 0.0f;    // Keep track of time per frame.
float lastTime = 0.0f;    // variable to keep overall time.
bool keyStatus[1024];    // Hold key status.
bool mouseEnabled = true; // keep track of mouse toggle.

// MAIN GRAPHICS OBJECT
Graphics myGraphics;        // Runing all the graphics in this object
ParticleEmitter *emit;

// Some global variable to do the animation.
float t = 0.001f;            // Global variable for animation
int main(int argc, char* argv[]) {
	int errorGraphics = myGraphics.Init();                        // Launch window and graphics context
	emit = new ParticleEmitter();

	if (errorGraphics) return 0;                                        // Close if something went wrong...
	Ecs &ecs = Ecs::get();

	startup();

	LoadObject::Cube({0.f, 0.f, 2.f});
	while (!quit) {
		ecs.update();

		if (glfwWindowShouldClose(myGraphics.window) == GL_TRUE)
			quit = true;
	}

	myGraphics.endProgram();            // Close and clean everything up...

	return 0;
}

void startup() {
	Ecs &ecs = Ecs::get();
	auto &obj = ecs.getComponentMap<GraphicalObject>();
	ecs.addUpdate(01, []() { updateCamera(); });
	ecs.addUpdate(11, []() { updateSceneElements(); });
	ecs.addUpdate(21, []() { renderScene(); });
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

	// Load Geometry examples
	/*myCube = LoadObject::Cube(glm::vec3(2.0f, 0.5f, 0.0f));

	ID mySphere = LoadObject::Sphere(glm::vec3(0.0f, 4.f, 0.0f), DEFAULTROT, DEFAULTSCALE, myCube);
	obj[mySphere].fillColor = glm::vec4(0.0f, 1.0f, 0.0f,
					    1.0f);    // You can change the shape fill colour, line colour or linewidth

	ID arrowY = LoadObject::Arrow(glm::vec3(3.0f, 0.0f, 0.0f), DEFAULTROT, glm::vec3(0.2f, 0.5f, 0.2f), mySphere);
	ID arrowX = LoadObject::Arrow(glm::vec3(0.0f, 0.0f, 0.0f), DEFAULTROT, DEFAULTSCALE, arrowY);
	ID arrowZ = LoadObject::Arrow(glm::vec3(0.0f, 0.0f, 0.0f), DEFAULTROT, DEFAULTSCALE, arrowY);
	obj[arrowX].fillColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	obj[arrowX].lineColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	obj[arrowX].rot = glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	obj[arrowY].fillColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	obj[arrowY].lineColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
//	obj[arrowY].rot = glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	obj[arrowZ].fillColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	obj[arrowZ].lineColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	obj[arrowZ].rot = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	*/ID myFloor = LoadObject::Cube(glm::vec3(0.0f, 0.0f, 0.0f), DEFAULTROT, glm::vec3(1000.0f, 0.001f, 1000.0f));
	obj[myFloor].fillColor = glm::vec4(130.0f / 255.0f, 96.0f / 255.0f, 61.0f / 255.0f, 1.0f);    // Sand Colour
	obj[myFloor].lineColor = glm::vec4(130.0f / 255.0f, 96.0f / 255.0f, 61.0f / 255.0f, 1.0f);   /* // Sand again

	ID myCylinder = LoadObject::Cylinder(glm::vec3(-1.0f, 0.5f, 2.0f));
	obj[myCylinder].fillColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	obj[myCylinder].lineColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	ID myLine = LoadObject::Line(glm::vec3(1.0f, 0.5f, 2.0f));
	obj[myLine].fillColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	obj[myLine].lineColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	obj[myLine].lineWidth = 5.0f;
*/
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

void updateSceneElements() {
	glfwPollEvents();
}

void recursion(glm::mat4 parent, std::unordered_map<ID,GraphicalObject> &map, SceneTree& node){
	glm::mat4 mv_matrix =
		parent *
		glm::translate(map[node.id].trans) *
		map[node.id].rot *
		glm::scale(map[node.id].scale) *
		glm::mat4(1.0f);
	map[node.id].mv_matrix = myGraphics.viewMatrix * mv_matrix;
	map[node.id].proj_matrix = myGraphics.projMatrix;
	map[node.id].Draw();

	for (auto &elem : node.childs)
		recursion(mv_matrix, map, elem);
}

void renderScene() {
	myGraphics.ClearViewport();
	Ecs &ecs = Ecs::get();
	auto &obj = ecs.getComponentMap<GraphicalObject>();

	for (auto &elem : ecs.tree.childs) {
		recursion(DEFAULTROT, obj, elem);
	}
	emit->update(myGraphics.viewMatrix, myGraphics.projMatrix);
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

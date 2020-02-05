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
#include "Graphics.h"
#include "Shapes.h"

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

// Some global variable to do the animation.
float t = 0.001f;            // Global variable for animation

int main() {
	int errorGraphics = myGraphics.Init();                        // Launch window and graphics context
	if (errorGraphics) return 0;                                        // Close if something went wrong...

	startup();                                                                                // Setup all necessary information for startup (aka. load texture, shaders, models, etc).
	Ecs &ecs = Ecs::get();
	// MAIN LOOP run until the window is closed
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
	GLfloat currentTime = (GLfloat) glfwGetTime();    // retrieve timelapse
	deltaTime = currentTime;                        // start delta time
	lastTime = currentTime;                            // Save for next frame calculations.

	// Callback graphics and key update functions - declared in main to avoid scoping complexity.
	// More information here : https://www.glfw.org/docs/latest/input_guide.html
	glfwSetWindowSizeCallback(myGraphics.window, onResizeCallback);            // Set callback for resize
	glfwSetKeyCallback(myGraphics.window, onKeyCallback);                    // Set Callback for keys
	glfwSetMouseButtonCallback(myGraphics.window, onMouseButtonCallback);    // Set callback for mouse click
	glfwSetCursorPosCallback(myGraphics.window, onMouseMoveCallback);        // Set callback for mouse move
	glfwSetScrollCallback(myGraphics.window, onMouseWheelCallback);            // Set callback for mouse wheel.

	// Calculate proj_matrix for the first time.
	myGraphics.aspect = (float) myGraphics.windowWidth / (float) myGraphics.windowHeight;
	myGraphics.proj_matrix = glm::perspective(glm::radians(50.0f), myGraphics.aspect, 0.1f, 1000.0f);

	// Load Geometry examples
	ID myCube = LoadObject::Cube(glm::vec3(2.0f, 0.5f, 0.0f));

	ID mySphere = LoadObject::Sphere(glm::vec3(-2.0f, 0.5f, 0.0f));
	obj[mySphere].fillColor = glm::vec4(0.0f, 1.0f, 0.0f,
					    1.0f);    // You can change the shape fill colour, line colour or linewidth

	ID arrowX = LoadObject::Arrow(glm::vec3(0.0f, 0.0f, 0.0f), DEFAULTMAT4, glm::vec3(0.2f, 0.5f, 0.2f));
	ID arrowY = LoadObject::Arrow(glm::vec3(0.0f, 0.0f, 0.0f), DEFAULTMAT4, glm::vec3(0.2f, 0.5f, 0.2f));
	ID arrowZ = LoadObject::Arrow(glm::vec3(0.0f, 0.0f, 0.0f), DEFAULTMAT4, glm::vec3(0.2f, 0.5f, 0.2f));
	obj[arrowX].fillColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	obj[arrowX].lineColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	obj[arrowX].rot = glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	obj[arrowY].fillColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	obj[arrowY].lineColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
//	obj[arrowY].rot = glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	obj[arrowZ].fillColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	obj[arrowZ].lineColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	obj[arrowZ].rot = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	ID myFloor = LoadObject::Cube(glm::vec3(0.0f, 0.0f, 0.0f), DEFAULTMAT4, glm::vec3(1000.0f, 0.001f, 1000.0f));
	obj[myFloor].fillColor = glm::vec4(130.0f / 255.0f, 96.0f / 255.0f, 61.0f / 255.0f, 1.0f);    // Sand Colour
	obj[myFloor].lineColor = glm::vec4(130.0f / 255.0f, 96.0f / 255.0f, 61.0f / 255.0f, 1.0f);    // Sand again

	ID myCylinder = LoadObject::Cylinder(glm::vec3(-1.0f, 0.5f, 2.0f));
	obj[myCylinder].fillColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	obj[myCylinder].lineColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	ID myLine = LoadObject::Line(glm::vec3(1.0f, 0.5f, 2.0f));
	obj[myLine].fillColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	obj[myLine].lineColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	obj[myLine].lineWidth = 5.0f;

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
	GLfloat cameraSpeed = 1.0f * deltaTime;
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
	auto &ecs = Ecs::get();
	auto &obj = ecs.getComponentMap<GraphicalObject>();
	glfwPollEvents();                                // poll callbacks

	// Calculate frame time/period -- used for all (physics, animation, logic, etc).
	GLfloat currentTime = (GLfloat) glfwGetTime();    // retrieve timelapse
	deltaTime = currentTime - lastTime;                // Calculate delta time
	lastTime = currentTime;                            // Save for next frame calculations.

	// Do not forget your ( T * R * S ) http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

	/*// Calculate Cube position
	glm::mat4 mv_matrix_cube =
		glm::translate(glm::vec3(2.0f, 0.5f, 0.0f)) *
		glm::mat4(1.0f);
	obj[myCube].mv_matrix = myGraphics.viewMatrix * mv_matrix_cube;
	obj[myCube].proj_matrix = myGraphics.proj_matrix;

	// calculate Sphere movement
	glm::mat4 mv_matrix_sphere =
		glm::translate(glm::vec3(-2.0f, 0.5f, 0.0f)) *
		glm::rotate(-t, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(-t, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::mat4(1.0f);
	obj[mySphere].mv_matrix = myGraphics.viewMatrix * mv_matrix_sphere;
	obj[mySphere].proj_matrix = myGraphics.proj_matrix;

	//Calculate Arrows translations (note: arrow model points up)
	glm::mat4 mv_matrix_x =
		glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(glm::vec3(0.2f, 0.5f, 0.2f)) *
		glm::mat4(1.0f);
	obj[arrowX].mv_matrix = myGraphics.viewMatrix * mv_matrix_x;
	obj[arrowX].proj_matrix = myGraphics.proj_matrix;

	glm::mat4 mv_matrix_y =
		glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)) *
		//glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *    // already model pointing up
		glm::scale(glm::vec3(0.2f, 0.5f, 0.2f)) *
		glm::mat4(1.0f);
	obj[arrowY].mv_matrix = myGraphics.viewMatrix * mv_matrix_y;
	obj[arrowY].proj_matrix = myGraphics.proj_matrix;

	glm::mat4 mv_matrix_z =
		glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::scale(glm::vec3(0.2f, 0.5f, 0.2f)) *
		glm::mat4(1.0f);
	obj[arrowZ].mv_matrix = myGraphics.viewMatrix * mv_matrix_z;
	obj[arrowZ].proj_matrix = myGraphics.proj_matrix;

	// Calculate floor position and resize
	obj[myFloor].mv_matrix = myGraphics.viewMatrix *
				 glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)) *
				 glm::scale(glm::vec3(1000.0f, 0.001f, 1000.0f)) *
				 glm::mat4(1.0f);
	obj[myFloor].proj_matrix = myGraphics.proj_matrix;

	// Calculate cylinder
	obj[myCylinder].mv_matrix = myGraphics.viewMatrix *
				    glm::translate(glm::vec3(-1.0f, 0.5f, 2.0f)) *
				    glm::mat4(1.0f);
	obj[myCylinder].proj_matrix = myGraphics.proj_matrix;

	// Calculate Line
	obj[myLine].mv_matrix = myGraphics.viewMatrix *
				glm::translate(glm::vec3(1.0f, 0.5f, 2.0f)) *
				glm::mat4(1.0f);
	obj[myLine].proj_matrix = myGraphics.proj_matrix;*/


	t += 0.01f; // increment movement variable


	 // If quit by pressing x on window.

}

void renderScene() {
	// Clear viewport - start a new frame.
	myGraphics.ClearViewport();
	Ecs &ecs = Ecs::get();
	auto &obj = ecs.getComponentMap<GraphicalObject>();
	for (auto &elem : obj) {
		glm::mat4 mv_matrix =
			glm::translate(elem.second.trans) *
			elem.second.rot *
			glm::scale(elem.second.scale) *
			glm::mat4(1.0f);
		elem.second.mv_matrix = myGraphics.viewMatrix * mv_matrix;
		elem.second.proj_matrix = myGraphics.proj_matrix;
		elem.second.Draw();
	}

	// Draw objects in screen
	/*Ecs::get().getComponentMap<Object>()[myFloor].Draw();
	Ecs::get().getComponentMap<Object>()[myCube].Draw();


	Ecs::get().getComponentMap<Object>()[arrowX].Draw();
	Ecs::get().getComponentMap<Object>()[arrowY].Draw();
	Ecs::get().getComponentMap<Object>()[arrowZ].Draw();

	Ecs::get().getComponentMap<Object>()[myLine].Draw();
	Ecs::get().getComponentMap<Object>()[myCylinder].Draw();*/
}


// CallBack functions low level functionality.
void onResizeCallback(GLFWwindow *window, int w, int h) {    // call everytime the window is resized
	//myGraphics.windowWidth = w;
	//myGraphics.windowHeight = h;

	glfwGetFramebufferSize(window, &myGraphics.windowWidth, &myGraphics.windowHeight);

	myGraphics.aspect = (float) w / (float) h;
	myGraphics.proj_matrix = glm::perspective(glm::radians(50.0f), myGraphics.aspect, 0.1f, 1000.0f);
}

void
onKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) { // called everytime a key is pressed
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

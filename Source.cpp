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
#include <components/Hitbox.hpp>
#include <components/Flock.hpp>
#include <components/pathfinding.hpp>
#include "Graphics.hpp"
#include "components/Shapes.hpp"
#include "components/Particle.hpp"
//#include "components/pathfinding.hpp"

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
//ParticleEmitter *emit;

// Some global variable to do the animation.
float t = 0.001f;
ID player;
std::array<std::array<Node, 12>, 12> mapp;

int main(int argc, char *argv[]) {
	keyStatus = new bool[1024];
	if (myGraphics.Init()) return 0;
	Ecs &ecs = Ecs::get();
	startup();
	auto &obj = ecs.getComponentMap<GraphicalObject>();


	mapp = pathfinding::map();
//	int mapp[12][12];
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			int v2 = rand() % 100 + 1;
			if (mapp[i][j].walled) {
//			if(v2>80||i==0||i==11||j==0||j==11){
				ID od = LoadObject::Cube({i + 2, 0.5f, j + 2});
//				ID od = LoadObject::FromSource("neotank.obj");
				ecs.addComponent<Hitbox>(od, ecs.getComponentMap<GraphicalObject>()[od].obj_vertices);
				obj[od].fillColor = glm::vec4(0.0f / 255.0f, 96.0f / 255.0f, 61.0f / 255.0f, 1.0f);
			}
		}
	}

//	emit = new ParticleEmitter();
//	player = LoadObject::Cube({10.f, 0.5f, 10.f}, DEFAULTROT, {0.25f, 0.25f, 0.25f});
	player = LoadObject::FromSource("/home/lulutor/CLionProjects/GameDesign/components/heavytriangletank.obj",{10.f, 0.5f, 10.f}, DEFAULTROT,{0.25f, 0.25f, 0.25f});
	Movement::WASD(player);
	ecs.addComponent<Hitbox>(player, player);
	ID part = Entity::getId();
	ecs.addComponent<ParticleEmitter>(part, 200);
	ecs.addComponent<Position3D>(part, glm::vec3({0.f, 0.5f, 0.f}));
	SceneTree::addSceneNode(part, player);

	ID id = Entity::getId();
	auto pos = glm::vec3((float) (10.f, 4.5f, 10.f));
	ecs.addComponent<Position3D>(id, pos);
	ecs.addComponent<Speed3D>(id, glm::vec3(0.f, 0.f, 0.f), 3.f);
	SceneTree::addSceneNode(id, 0);
	ecs.addComponent<Flock>(player);
//	ecs.addComponent<AStar>(id);
//	ecs.getComponentMap<AStar>()[id].chain = pathfinding::Astar(Node(pos.x, pos.z), Node(10, 10), mapp);
	auto &flock = ecs.getComponentMap<Flock>()[player];
	for (int i = 0; i < 10; ++i) {
		auto elem = LoadObject::FromSource("/home/lulutor/CLionProjects/GameDesign/components/heavytriangletank.obj",
			glm::vec3((float) (rand() % 100) / 10.f, 0.5f, (float) (rand() % 100) / 10.f),DEFAULTROT,{0.125f,0.125f,0.125f});
		ecs.addComponent<Speed3D>(elem, glm::vec3(0.f, 0.f, 0.f), 5.f);
		ecs.addComponent<Hitbox>(elem, elem);
		flock.childs.push_back(elem);
	}


	while (glfwWindowShouldClose(myGraphics.window) != GL_TRUE) {
		ecs.update();
	}

	myGraphics.endProgram();
	return 0;
}

void startup() {
	Ecs &ecs = Ecs::get();
	auto &obj = ecs.getComponentMap<GraphicalObject>();
	int event = 0;
	ecs.addUpdate(++event, []() { updateCamera(); });
	ecs.addUpdate(++event, []() { glfwPollEvents(); });
	/*ecs.addUpdate(++event, []() {
		auto &ecs = Ecs::get();
		auto &astars = ecs.getComponentMap<AStar>();
		auto &pos = ecs.getComponentMap<Position3D>();
		auto &speed = ecs.getComponentMap<Speed3D>();

		for (auto &astari : astars) {
			auto &astar = astari.second;

			if (astar.chain.empty()) {
				std::cout << "ca me pete les boules" << std::endl;
				astar.chain = pathfinding::Astar(Node(trunc(pos[astari.first].trans.x + 0.5f), trunc(pos[astari.first].trans.z + 0.5f)), Node(trunc(pos[player].trans.x + 0.5f), trunc(pos[player].trans.z + 0.5f)), mapp);
			}

			if (trunc(pos[astari.first].trans.x + 0.5f) == astar.chain.begin()->pos.x &&
				trunc(pos[astari.first].trans.z + 0.5f) == astar.chain.begin()->pos.y) {
				astar.chain.pop_front();
			}

			std::cout << astar.chain.begin()->pos.x << " " << astar.chain.begin()->pos.y << std::endl;
			speed[astari.first].direction.x = (float)astar.chain.begin()->pos.x - pos[astari.first].trans.x;
			speed[astari.first].direction.z = (float)astar.chain.begin()->pos.y - pos[astari.first].trans.z;
			speed[astari.first].direction.y = 0.f;
			auto total = abs(speed[astari.first].direction.x) + abs(speed[astari.first].direction.y) + abs(speed[astari.first].direction.z);
			speed[astari.first].sped.x = (total == 0.f ? 0.f : ((speed[astari.first].direction.x * speed[astari.first].speed * deltaTime) / total));
			speed[astari.first].sped.y = (total == 0.f ? 0.f : ((speed[astari.first].direction.y * speed[astari.first].speed * deltaTime) / total));
			speed[astari.first].sped.z = (total == 0.f ? 0.f : ((speed[astari.first].direction.z * speed[astari.first].speed * deltaTime) / total));
//			std::cout << pos[astari.first].trans.x << " " << pos[astari.first].trans.z << std::endl;
//			std::cout << speed[astari.first].direction.x << " " << speed[astari.first].direction.z << std::endl << std::endl;
		}
	});*/
	ecs.addUpdate(++event, [&]() {
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
	}); ///Update Keyboard
	ecs.addUpdate(++event, []() {
		auto &ecs = Ecs::get();
		auto &position = ecs.getComponentMap<Position3D>();
		auto &speed = ecs.getComponentMap<Speed3D>();
		auto &flocks = ecs.getComponentMap<Flock>();

		for (auto &flock : flocks) {
			auto &goal = position[flock.first];
			for (auto const &id : flock.second.childs) {
				speed[id].direction.x = goal.trans.x - position[id].trans.x;
				speed[id].direction.z = goal.trans.z - position[id].trans.z;
				auto total = abs(speed[id].direction.x) + abs(speed[id].direction.y) +
					     abs(speed[id].direction.z);
				speed[id].sped.x = (total == 0.f ? 0.f : (
					(speed[id].direction.x * speed[id].speed * deltaTime) / total));
				speed[id].sped.y = (total == 0.f ? 0.f : (
					(speed[id].direction.y * speed[id].speed * deltaTime) / total));
				speed[id].sped.z = (total == 0.f ? 0.f : (
					(speed[id].direction.z * speed[id].speed * deltaTime) / total));
			}
			for (auto id = flock.second.childs.begin(); id != flock.second.childs.end(); ++id) {
				auto &pos1 = position[*id];
				for (auto od = id; od != flock.second.childs.end(); ++od) {
					if (*od == *id)
						continue;
					auto &pos2 = position[*od];
					float dist =
						abs(pos2.trans.x - pos1.trans.x) + abs(pos2.trans.y - pos1.trans.y) +
						abs(pos2.trans.z - pos1.trans.z);
					float repulsion = 0.5f * (1 / exp(dist));

					speed[*id].sped.x += repulsion / dist * abs(pos2.trans.x - pos1.trans.x);
					speed[*id].sped.y += repulsion / dist * abs(pos2.trans.y - pos1.trans.y);
					speed[*id].sped.z += repulsion / dist * abs(pos2.trans.z - pos1.trans.z);
				}
			}
		}
	}); ///Update Flocks
	ecs.addUpdate(++event, [&]() {
		auto &ecs = Ecs::get();
		auto &speeds = ecs.getComponentMap<Speed3D>();
		auto &poss = ecs.getComponentMap<Position3D>();
		auto &box = ecs.getComponentMap<Hitbox>();
		auto ids = ecs.filter<Speed3D, Position3D>();
		auto boxids = ecs.filter<Hitbox, Speed3D, Position3D>();
		auto boxidalls = ecs.filter<Hitbox, Position3D>();

		for (auto &mov : boxids) {
			for (auto &st : boxidalls) {
				if (mov == st)
					continue;

				if (poss[mov].trans.x + speeds[mov].sped.x + box[mov].minX >
				    poss[st].trans.x + box[st].maxX)
					continue;

				if (poss[mov].trans.x + speeds[mov].sped.x + box[mov].maxX <
				    poss[st].trans.x + box[st].minX)
					continue;

				if (poss[mov].trans.y + speeds[mov].sped.y + box[mov].minZ >
				    poss[st].trans.y + box[st].maxZ)
					continue;

				if (poss[mov].trans.y + speeds[mov].sped.y + box[mov].maxZ <
				    poss[st].trans.y + box[st].minZ)
					continue;

				if (poss[mov].trans.z + speeds[mov].sped.z + box[mov].minZ >
				    poss[st].trans.z + box[st].maxZ)
					continue;

				if (poss[mov].trans.z + speeds[mov].sped.z + box[mov].maxZ <
				    poss[st].trans.z + box[st].minZ)
					continue;


				if (poss[mov].trans.x + box[mov].minX > poss[st].trans.x + box[st].maxX ||
				    poss[mov].trans.x + box[mov].maxX < poss[st].trans.x + box[st].minX)
					speeds[mov].sped.x = 0;

				if (poss[mov].trans.y + box[mov].minZ > poss[st].trans.y + box[st].maxZ ||
				    poss[mov].trans.y + box[mov].maxZ < poss[st].trans.y + box[st].minZ)
					speeds[mov].sped.y = 0;

				if (poss[mov].trans.z + box[mov].minZ > poss[st].trans.z + box[st].maxZ ||
				    poss[mov].trans.z + box[mov].maxZ < poss[st].trans.z + box[st].minZ)
					speeds[mov].sped.z = 0;
			}
		}

		for (const auto &id: ids) {
			auto &speed = speeds[id].speed;
			auto &sped = speeds[id].sped;
			auto &pos = poss[id].trans;

			pos.x += sped.x;
			pos.y += sped.y;
			pos.z += sped.z;
		}
	}); ///Update Speed and Hitbox
	ecs.addUpdate(++event, []() { renderScene(); });
	ecs.addUpdate(++event, [&]() {
		auto &ecs = Ecs::get();
		auto &positions = ecs.getComponentMap<Position3D>();
		auto &graphicals = ecs.getComponentMap<GraphicalObject>();

		for (auto &graphical: graphicals) {
			graphical.second.mv_matrix = positions[graphical.first].mv_matrix;
			graphical.second.proj_matrix = myGraphics.projMatrix;
			graphical.second.Draw();
		}
	}); ///Draw GraphicalObjects
	ecs.addUpdate(++event, [&]() {
		auto &ecs = Ecs::get();
		auto &positions = ecs.getComponentMap<Position3D>();
		auto &particles = ecs.getComponentMap<ParticleEmitter>();
		auto ids = ecs.filter<ParticleEmitter, Position3D>();

		for (auto &id : ids) {
			particles[id].update(myGraphics.viewMatrix, myGraphics.projMatrix, positions[id].worldTrans);
		}
	}); ///Update and Draw Particles
	ecs.addUpdate(++event, []() { glfwSwapBuffers(myGraphics.window); });

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

	ID myFloor = LoadObject::Cube(glm::vec3(7.5f, 0.0f, 7.5f), DEFAULTROT, glm::vec3(12.0f, 0.001f, 12.0f));
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
	/*if (keyStatus[GLFW_KEY_W]) myGraphics.cameraPosition += cameraSpeed * myGraphics.cameraFront;
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
		myGraphics.cameraPosition += cameraSpeed * myGraphics.cameraUp;
	if (keyStatus[GLFW_KEY_LEFT_CONTROL])
		myGraphics.cameraPosition -= cameraSpeed * myGraphics.cameraUp;*/

	// IMPORTANT PART
	// Calculate my view matrix using the lookAt helper function
	if (mouseEnabled) {
		myGraphics.viewMatrix = glm::lookAt(myGraphics.cameraPosition,                        // eye
						    myGraphics.cameraPosition +
						    myGraphics.cameraFront,                                        // centre
						    myGraphics.cameraUp);                                                                                                // up
	}
}

void recursion(glm::mat4 parent, std::unordered_map<ID, Position3D> &map, SceneTree &node) {
	glm::mat4 mv_matrix =
		parent *
		glm::translate(map[node.id].trans) *
		map[node.id].rot *
		glm::scale(map[node.id].scale) *
		glm::mat4(1.0f);
	map[node.id].mv_matrix = myGraphics.viewMatrix * mv_matrix;
	map[node.id].worldTrans = mv_matrix[3];

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

//
// Created by seb on 26/02/2020.
//

#include <GLFW/glfw3.h>
#include <components/Keyboard.hpp>
#include <components/Speed3D.hpp>
#include <Config.hpp>

#include "Movement.hpp"
#include "Ecs.hpp"

void Movement::ZQSD(ID id) {
	fromKeys(id, {GLFW_KEY_Z, GLFW_KEY_Q, GLFW_KEY_S, GLFW_KEY_D});
}

void Movement::WASD(ID id) {
	fromKeys(id, {GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D});
}

void Movement::ARROW(ID id) {
	fromKeys(id, {GLFW_KEY_UP, GLFW_KEY_LEFT, GLFW_KEY_DOWN, GLFW_KEY_RIGHT});
}

void Movement::fromKeys(ID id, std::vector<int> keys) {
	auto &ecs = Ecs::get();

	ecs.addComponent<Keyboard>(id);
	ecs.addComponent<Speed3D>(id, glm::vec3(), SPEED);

	auto &keyb = ecs.getComponentMap<Keyboard>()[id];

	/// Forward
	keyb.keys[keys[0]] = [&ecs, id, keys](bool down){
		auto &speed = ecs.getComponentMap<Speed3D>()[id];
		if (!down || keyStatus[keys[2]]) {
			if (!keyStatus[keys[2]])
				speed.direction.x = 0;
			return;
		}
		speed.direction.x = 1;
	};

	/// Left
	keyb.keys[keys[1]] = [&ecs, id, keys](bool down){
		auto &speed = ecs.getComponentMap<Speed3D>()[id];
		if (!down || keyStatus[keys[3]]) {
			if (!keyStatus[keys[3]]) {
				speed.direction.z= 0;
			}
			return;
		}
		speed.direction.z = -1;
	};

	/// Backward
	keyb.keys[keys[2]] = [&ecs, id, keys](bool down){
		auto &speed = ecs.getComponentMap<Speed3D>()[id];
		if (!down || keyStatus[keys[0]]) {
			if (!keyStatus[keys[0]]) {
				speed.direction.x = 0;
			}
			return;
		}
		speed.direction.x = -1;
	};

	/// Right
	keyb.keys[keys[3]] = [&ecs, id, keys](bool down){
		auto &speed = ecs.getComponentMap<Speed3D>()[id];
		if (!down || keyStatus[keys[1]]) {
			if (!keyStatus[keys[1]]) {
				speed.direction.z = 0;
			}
			return;
		}
		speed.direction.z = 1;
	};
}

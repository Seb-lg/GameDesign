//
// Created by seb on 26/02/2020.
//

#include <components/Keyboard.hpp>
#include <components/Speed3D.hpp>
#include <Config.hpp>

#include "Movement.hpp"
#include "Ecs.hpp"
#include <components/Position3D.hpp>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>

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

	auto &keyb = ecs.getComponent<Keyboard>(id);
	auto &player = ecs.getComponent<Position3D>(id);

//	/// RELATIVE MOVEMENT
//
//
//	/// Forward
//	keyb.keys[keys[0]] = [&ecs, id, keys](bool down){
//		auto &speed = ecs.getComponentMap<Speed3D>()[id];
//		if (!down || keyStatus[keys[2]]) {
//			if (!keyStatus[keys[2]])
//				speed.direction.x = 0;
//			auto total = abs(speed.direction.x) + abs(speed.direction.y) + abs(speed.direction.z);
//			speed.sped.x = (total == 0.f ? 0.f : ((speed.direction.x * speed.speed * deltaTime) / total));
//			speed.sped.y = (total == 0.f ? 0.f : ((speed.direction.y * speed.speed * deltaTime) / total));
//			speed.sped.z = (total == 0.f ? 0.f : ((speed.direction.z * speed.speed * deltaTime) / total));
//			return;
//		}
//		speed.direction.x = 1;
//		auto total = abs(speed.direction.x) + abs(speed.direction.y) + abs(speed.direction.z);
//		speed.sped.x = (total == 0.f ? 0.f : ((speed.direction.x * speed.speed * deltaTime) / total));
//		speed.sped.y = (total == 0.f ? 0.f : ((speed.direction.y * speed.speed * deltaTime) / total));
//		speed.sped.z = (total == 0.f ? 0.f : ((speed.direction.z * speed.speed * deltaTime) / total));
//	};
//
//	/// Backward
//	keyb.keys[keys[2]] = [&ecs, id, keys, &player](bool down){
//		auto &speed = ecs.getComponentMap<Speed3D>()[id];
//		if (!down || keyStatus[keys[0]]) {
//			if (!keyStatus[keys[0]]) {
//				speed.direction.x = 0;
//				speed.direction.z = 0;
//			}
//			auto total = abs(speed.direction.x) + abs(speed.direction.y) + abs(speed.direction.z);
//			speed.sped.x = (total == 0.f ? 0.f : ((speed.direction.x * speed.speed * deltaTime) / total));
//			speed.sped.y = (total == 0.f ? 0.f : ((speed.direction.y * speed.speed * deltaTime) / total));
//			speed.sped.z = (total == 0.f ? 0.f : ((speed.direction.z * speed.speed * deltaTime) / total));
//			return;
//		}
//		auto &playerpos = glm::translate(player.rot);
//		if(player.rot[0][0]==1 ){
//			speed.direction.x = -1;}
//		else{speed.direction.x=-1;}
//		if(player.rot[2][2] ==1 ){
//			speed.direction.z = 1;
//		}
//		else{speed.direction.z=-1;}
//		auto total = abs(speed.direction.x) + abs(speed.direction.y) + abs(speed.direction.z);
//		speed.sped.x = (total == 0.f ? 0.f : ((speed.direction.x * speed.speed * deltaTime) / total));
//		speed.sped.y = (total == 0.f ? 0.f : ((speed.direction.y * speed.speed * deltaTime) / total));
//		speed.sped.z = (total == 0.f ? 0.f : ((speed.direction.z * speed.speed * deltaTime) / total));
//	};

//	///Left
//	keyb.keys[keys[1]] = [&ecs, id, keys, &player] (bool down) {
//		if (down) {
//			player.rot = glm::rotate(player.rot, .05f, glm::vec3(0, 1, 0));
//		}
//	};
//
//	///Right
//	keyb.keys[keys[3]] = [&ecs, id, keys, &player] (bool down) {
//		if (down) {
//			player.rot = glm::rotate(player.rot, -.05f, glm::vec3(0, 1, 0));
//		}
//	};

 /// ABSOLUTE MOVEMENT
	/// Forward
	keyb.keys[keys[0]] = [&ecs, id, keys](bool down){
		auto &speed = ecs.getComponentMap<Speed3D>()[id];
		if (!down || keyStatus[keys[2]]) {
			if (!keyStatus[keys[2]])
				speed.direction.x = 0;
			auto total = abs(speed.direction.x) + abs(speed.direction.y) + abs(speed.direction.z);
			speed.sped.x = (total == 0.f ? 0.f : ((speed.direction.x * speed.speed * deltaTime) / total));
			speed.sped.y = (total == 0.f ? 0.f : ((speed.direction.y * speed.speed * deltaTime) / total));
			speed.sped.z = (total == 0.f ? 0.f : ((speed.direction.z * speed.speed * deltaTime) / total));
			return;
		}
		speed.direction.x = 1;
		auto total = abs(speed.direction.x) + abs(speed.direction.y) + abs(speed.direction.z);
		speed.sped.x = (total == 0.f ? 0.f : ((speed.direction.x * speed.speed * deltaTime) / total));
		speed.sped.y = (total == 0.f ? 0.f : ((speed.direction.y * speed.speed * deltaTime) / total));
		speed.sped.z = (total == 0.f ? 0.f : ((speed.direction.z * speed.speed * deltaTime) / total));
	};

	/// Left
	keyb.keys[keys[1]] = [&ecs, id, keys](bool down){
		auto &speed = ecs.getComponentMap<Speed3D>()[id];
		if (!down || keyStatus[keys[3]]) {
			if (!keyStatus[keys[3]]) {
				speed.direction.z= 0;
			}
			auto total = abs(speed.direction.x) + abs(speed.direction.y) + abs(speed.direction.z);
			speed.sped.x = (total == 0.f ? 0.f : ((speed.direction.x * speed.speed * deltaTime) / total));
			speed.sped.y = (total == 0.f ? 0.f : ((speed.direction.y * speed.speed * deltaTime) / total));
			speed.sped.z = (total == 0.f ? 0.f : ((speed.direction.z * speed.speed * deltaTime) / total));
			return;
		}
		speed.direction.z = -1;
		auto total = abs(speed.direction.x) + abs(speed.direction.y) + abs(speed.direction.z);
		speed.sped.x = (total == 0.f ? 0.f : ((speed.direction.x * speed.speed * deltaTime) / total));
		speed.sped.y = (total == 0.f ? 0.f : ((speed.direction.y * speed.speed * deltaTime) / total));
		speed.sped.z = (total == 0.f ? 0.f : ((speed.direction.z * speed.speed * deltaTime) / total));
	};

	/// Backward
	keyb.keys[keys[2]] = [&ecs, id, keys](bool down){
		auto &speed = ecs.getComponentMap<Speed3D>()[id];
		if (!down || keyStatus[keys[0]]) {
			if (!keyStatus[keys[0]]) {
				speed.direction.x = 0;
			}
			auto total = abs(speed.direction.x) + abs(speed.direction.y) + abs(speed.direction.z);
			speed.sped.x = (total == 0.f ? 0.f : ((speed.direction.x * speed.speed * deltaTime) / total));
			speed.sped.y = (total == 0.f ? 0.f : ((speed.direction.y * speed.speed * deltaTime) / total));
			speed.sped.z = (total == 0.f ? 0.f : ((speed.direction.z * speed.speed * deltaTime) / total));
			return;
		}
		speed.direction.x = -1;
		auto total = abs(speed.direction.x) + abs(speed.direction.y) + abs(speed.direction.z);
		speed.sped.x = (total == 0.f ? 0.f : ((speed.direction.x * speed.speed * deltaTime) / total));
		speed.sped.y = (total == 0.f ? 0.f : ((speed.direction.y * speed.speed * deltaTime) / total));
		speed.sped.z = (total == 0.f ? 0.f : ((speed.direction.z * speed.speed * deltaTime) / total));
	};

	/// Right
	keyb.keys[keys[3]] = [&ecs, id, keys](bool down){
		auto &speed = ecs.getComponentMap<Speed3D>()[id];
		if (!down || keyStatus[keys[1]]) {
			if (!keyStatus[keys[1]]) {
				speed.direction.z = 0;
			}
			auto total = abs(speed.direction.x) + abs(speed.direction.y) + abs(speed.direction.z);
			speed.sped.x = (total == 0.f ? 0.f : ((speed.direction.x * speed.speed * deltaTime) / total));
			speed.sped.y = (total == 0.f ? 0.f : ((speed.direction.y * speed.speed * deltaTime) / total));
			speed.sped.z = (total == 0.f ? 0.f : ((speed.direction.z * speed.speed * deltaTime) / total));
			return;
		}
		speed.direction.z = 1;
		auto total = abs(speed.direction.x) + abs(speed.direction.y) + abs(speed.direction.z);
		speed.sped.x = (total == 0.f ? 0.f : ((speed.direction.x * speed.speed * deltaTime) / total));
		speed.sped.y = (total == 0.f ? 0.f : ((speed.direction.y * speed.speed * deltaTime) / total));
		speed.sped.z = (total == 0.f ? 0.f : ((speed.direction.z * speed.speed * deltaTime) / total));
	};


}

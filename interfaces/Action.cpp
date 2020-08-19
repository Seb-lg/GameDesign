//
// Created by lulutor on 18/08/2020.
//

#include <components/Keyboard.hpp>
#include <components/Speed3D.hpp>
#include <Config.hpp>

#include "Action.hpp"
#include "Ecs.hpp"
#include <components/Position3D.hpp>
#include <GLFW/glfw3.h>
#include "components/Particle.hpp"
#include "components/Shapes.hpp"
#include <glm/ext.hpp>
#include "interfaces/LoadObject.hpp"
#include "components//Hitbox.hpp"

void Shoot::ZQSD(ID id) {
	fromKeys(id, {GLFW_KEY_UP, GLFW_KEY_DOWN,GLFW_KEY_LEFT,GLFW_KEY_RIGHT});
}

void Shoot::WASD(ID id) {
	fromKeys(id, {GLFW_KEY_UP, GLFW_KEY_DOWN,GLFW_KEY_LEFT,GLFW_KEY_RIGHT, GLFW_KEY_SPACE});
}

void Shoot::ARROW(ID id) {
	fromKeys(id, {GLFW_KEY_UP, GLFW_KEY_DOWN,GLFW_KEY_LEFT,GLFW_KEY_RIGHT});
}

void Shoot::fromKeys(ID id,std::vector<int> keys) {
///get orientation of player
	auto &ecs = Ecs::get();

	ecs.addComponent<Keyboard>(id);
	ecs.addComponent<Speed3D>(id, glm::vec3(), SPEED);
	auto &keyb = ecs.getComponent<Keyboard>(id);
	auto &playerpos = ecs.getComponent<Position3D>(id);


	keyb.keys[keys[0]] = [&ecs, id, keys, &playerpos](bool down) {
		if (!down) {
		} else {
//			auto bullet = LoadObject::Cube(playerpos.trans, playerpos.rot, {0.25f, 0.25f, 0.5f});
			ID bulletid = Entity::getId();
			auto bullet = LoadObject::FromSource("./assets/heavytriangletank.obj",playerpos.trans, playerpos.rot,{0.25f, 0.25f, 0.25f});
			ecs.addComponent<Hitbox>(bullet, bullet);
			ecs.addComponent<ParticleEmitter>(bullet, 10);
			SceneTree::addSceneNode(bullet, id);
		}
	};

	keyb.keys[keys[1]] = [&ecs, id, keys, &playerpos](bool down) {
		if (!down) {
			if (!ecs.idHasComponents<Hitbox>(id))
				ecs.addComponent<Hitbox>(id);
		} else {
			if (ecs.idHasComponents<Hitbox>(id))
				ecs.deleteComponentforId<Hitbox>(id);
		}
	};
	///Left
	keyb.keys[keys[2]] = [&ecs, id, keys, &playerpos](bool down) {
		if (down) {
			playerpos.rot = glm::rotate(playerpos.rot, .05f, glm::vec3(0, 1, 0));
		}
	};

	///Right
	keyb.keys[keys[3]] = [&ecs, id, keys, &playerpos](bool down) {
		if (down) {
			playerpos.rot = glm::rotate(playerpos.rot, -.05f, glm::vec3(0, 1, 0));
		}
	};
	keyb.keys[keys[4]] = [&ecs, id, keys, &playerpos](bool down) {
		if (!down) {
		} else {
			///for each bullet : destroy whatever touch the hitbox then destroy bullet.
			auto &mines = ecs.getComponentMap<ParticleEmitter>();
			for(auto &mine: mines){
				
			}
		}
	};
}
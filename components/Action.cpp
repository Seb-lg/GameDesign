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
#include "Shapes.hpp"
#include <glm/ext.hpp>

void Shoot::ZQSD(ID id) {
	fromKeys(id, {GLFW_KEY_P});
}

void Shoot::WASD(ID id) {
	fromKeys(id, {GLFW_KEY_P});
}

void Shoot::ARROW(ID id) {
	fromKeys(id, {GLFW_KEY_SPACE});
}

void Shoot::fromKeys(ID id,GraphicalObject player ,std::vector<int> keys) {
///get orientation of player
	auto &ecs = Ecs::get();

	ecs.addComponent<Keyboard>(id);
	ecs.addComponent<Speed3D>(id, glm::vec3(), SPEED);
	auto &keyb = ecs.getComponent<Keyboard>(id);
	auto &playerpos = ecs.getComponent<Position3D>(id);


///add particule component
	ID part = Entity::getId();
	ecs.addComponent<ParticleEmitter>(part, 200);
	ecs.addComponent<Position3D>(part, glm::vec3({0.f, 0.5f, 0.f}));
	SceneTree::addSceneNode(part, player);


///destroy object boids in front of

///stop particule
	ecs.deleteComponentforId<ParticleEmitter>(part)
}

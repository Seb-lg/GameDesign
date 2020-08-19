//
// Created by lulutor on 18/08/2020.
//

#include <components/Keyboard.hpp>
#include <components/Speed3D.hpp>
#include <Config.hpp>
#include <queue>

#include "SceneTree.hpp"
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
			ID bullet = Entity::getId();
			bullet = LoadObject::FromSource("./assets/heavytriangletank.obj", playerpos.trans, playerpos.rot,
			                                {0.25f, 0.25f, 0.25f});
			ecs.addComponent<Position3D>(bullet);
			ecs.addComponent<Hitbox>(bullet, bullet);
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

			std::queue<SceneTree *> queue;
			SceneTree *parentNode = &Ecs::get().tree;
			queue.push(parentNode);;
			auto &speeds = ecs.getComponentMap<Speed3D>();
			auto &poss = ecs.getComponentMap<Position3D>();
			auto &box = ecs.getComponentMap<Hitbox>();
			auto ids = ecs.filter<Speed3D, Position3D>();
			auto boxids = ecs.filter<Hitbox, Speed3D, Position3D>();
			auto boxidalls = ecs.filter<Hitbox, Position3D>();

			while (!queue.empty()) {
				auto node = queue.front();
				queue.pop();
				if (node->id == id) {
					parentNode = node;
					break;
				}
				for (auto &elem : node->childs) {
					///check if has hitbox and take the X,Y,Z to compare if any object is in contact.
					if (ecs.idHasComponents<Hitbox>(node->id)) {
						for (auto &mov : boxids) {
							for (auto &st : boxidalls) {
								if (mov == st)
									continue;
								if (poss[mov].trans.x + box[mov].minX > poss[st].trans.x + box[st].maxX ||
								    poss[mov].trans.x + box[mov].maxX < poss[st].trans.x + box[st].minX) {
									ecs.deleteId(st);
									cout<<ecs.isDeleted(st)<<endl;
								}
								if (poss[mov].trans.y + box[mov].minZ > poss[st].trans.y + box[st].maxZ ||
								    poss[mov].trans.y + box[mov].maxZ < poss[st].trans.y + box[st].minZ) {
									ecs.deleteId(st);
									cout<<ecs.isDeleted(st)<<endl;
								}
								if (poss[mov].trans.z + box[mov].minZ  > poss[st].trans.z + box[st].maxZ ||
								    poss[mov].trans.z + box[mov].maxZ < poss[st].trans.z + box[st].minZ) {
									ecs.deleteId(st);
									cout<<ecs.isDeleted(st)<<endl;
								}
							}
						}

					}
				}

			}
		};
	};
}
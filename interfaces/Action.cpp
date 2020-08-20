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
#include "ecs/Time.hpp"
#include "components/Flock.hpp"
void Action::ZQSD(ID id) {
	fromKeys(id, {GLFW_KEY_UP, GLFW_KEY_DOWN,GLFW_KEY_LEFT,GLFW_KEY_RIGHT, GLFW_KEY_SPACE});
}

void Action::WASD(ID id) {
	fromKeys(id, {GLFW_KEY_UP, GLFW_KEY_DOWN,GLFW_KEY_LEFT,GLFW_KEY_RIGHT, GLFW_KEY_SPACE});
}

void Action::ARROW(ID id) {
	fromKeys(id, {GLFW_KEY_UP, GLFW_KEY_DOWN,GLFW_KEY_LEFT,GLFW_KEY_RIGHT, GLFW_KEY_SPACE});
}

void Action::fromKeys(ID id,std::vector<int> keys) {
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
			auto elem = LoadObject::FromSource("./assets/soucoupe-support.obj",
			                                   glm::vec3((float) (rand() % 100) / 10.f, 0.5f,
			                                             (float) (rand() % 100) / 10.f), DEFAULTROT,
			                                   {0.125f, 0.125f, 0.125f});
			ecs.addComponent<Speed3D>(elem, glm::vec3(0.f, 0.f, 0.f), 5.f);
			ecs.addComponent<Hitbox>(elem, elem);
			auto flock = ecs.getComponent<Flock>(id);
			ID part = Entity::getId();
			ecs.addComponent<ParticleEmitter>(part, 100);
			ecs.addComponent<Position3D>(part, glm::vec3({0.f, 0.5f, 0.f}));
			SceneTree::addSceneNode(part, elem);
			SceneTree::addSceneNode(elem, id);
			flock.childs.push_back(elem);

		}
	};
	/*		std::queue<SceneTree *> queue;
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
					for (auto &mov : boxids) {
						for (auto &st : boxidalls) {
							cout<<"encule1"<<endl;
							if (mov == st)
								continue;
							if (poss[mov].trans.x + box[mov].minX -5> poss[st].trans.x + box[st].maxX +5||
							    poss[mov].trans.x + box[mov].maxX +5< poss[st].trans.x + box[st].minX-5) {

								if (ecs.idHasComponents<Hitbox>(st))
									ecs.deleteComponentforId<Hitbox>(st);
								if (ecs.idHasComponents<GraphicalObject>(st))
									ecs.deleteComponentforId<GraphicalObject>(st);
								cout<<"encule1"<<endl;
							}
							if (poss[mov].trans.y + box[mov].minZ -5> poss[st].trans.y + box[st].maxZ +5||
							    poss[mov].trans.y + box[mov].maxZ +5< poss[st].trans.y + box[st].minZ-5) {
								if (ecs.idHasComponents<Hitbox>(st))
									ecs.deleteComponentforId<Hitbox>(st);
								if (ecs.idHasComponents<GraphicalObject>(st))
									ecs.deleteComponentforId<GraphicalObject>(st);
								cout<<"encule2"<<endl;
							}
							if (poss[mov].trans.z + box[mov].minZ +5> poss[st].trans.z + box[st].maxZ -5||
							    poss[mov].trans.z + box[mov].maxZ -5< poss[st].trans.z + box[st].minZ -5) {
								if (ecs.idHasComponents<Hitbox>(st))
									ecs.deleteComponentforId<Hitbox>(st);
								if (ecs.idHasComponents<GraphicalObject>(st))
									ecs.deleteComponentforId<GraphicalObject>(st );
								cout<<"encul3"<<endl;
							}

						}

					}
				}

			}
		};
	};*/
}
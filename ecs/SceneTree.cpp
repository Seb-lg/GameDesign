//
// Created by seb on 05/02/2020.
//

#include "SceneTree.hpp"
#include "Ecs.hpp"

#include <queue>

SceneTree::SceneTree(ID id, ID parent): id(id), parent(parent) {
}

void SceneTree::addSceneNode(ID id, ID parent) {
	std::queue<SceneTree*> queue;
	SceneTree* parentNode = &Ecs::get().tree;

	queue.push(parentNode);

	while (!queue.empty()) {
		auto node = queue.front();
		queue.pop();
		if (node->id == parent) {
			parentNode = node;
			break;
		}
		for (auto &elem : node->childs)
			queue.push(&elem);
	}

	parentNode->childs.emplace_back(id, parent);
}

//
// Created by seb on 05/02/2020.
//

#pragma once

#include <list>
#include "Entity.hpp"

class SceneTree {
public:
	SceneTree(ID id = 0, ID parent = 0);

	static void addSceneNode(ID id = 0, ID parent = 0);

public:
	ID parent;
	ID id;
	std::list<SceneTree> childs;
};
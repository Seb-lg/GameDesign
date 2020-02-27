//
// Created by seb on 26/02/2020.
//

#pragma once

#include <vector>
#include <functional>

class Keyboard {
public:
	Keyboard();

public:
	std::vector<std::function<void(bool)>> keys;
};
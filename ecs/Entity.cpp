//
// Created by seble on 02/02/2020.
//

#include "Entity.hpp"

ID Entity::getId() {
    static ID id = 1;
    ++id;
    return (id);
}
//
// Created by lucienR on 15/02/2020.
//
#include <iostream>
#include <list>
using namespace std;

#ifndef GAMEDESIGN_PATHFINDING_HPP
#define GAMEDESIGN_PATHFINDING_HPP

#endif //GAMEDESIGN_PATHFINDING_HPP

struct Position {
    int x;
    int y;
    int value;
public:
    Position() = default;
    Position(int x,int y,int value);
    bool equal(Position pos);
};

bool operator<(std::list<Position>::iterator a, std::list<Position>::iterator b){ return a->value<b->value;}

class pathfinding{
public:
    void map();

    void heuristics( Position actuel , Position goal );

    void Astar( Position actuel , Position goal,int map[12][12]);

};
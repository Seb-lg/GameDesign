//
// Created by lucienR on 15/02/2020.
//
#include <iostream>
#include <list>
#include <array>
using namespace std;

#ifndef GAMEDESIGN_PATHFINDING_HPP
#define GAMEDESIGN_PATHFINDING_HPP

#endif //GAMEDESIGN_PATHFINDING_HPP

struct Position {
    int x;
    int y;
//    float value;
public:
    Position() = default;
    Position(int x,int y);
//    Position(int x,int y,int value);
    bool equal(Position pos);
    float distance(Position pos);
};
//bool operator ==(Position pos,Position pos2){return pos.x==pos2.x && pos.y==pos2.y ;}

struct Node{
    Position pos;
    Position parent;
    float f;
    float g;
    float h;
    bool walled = false;
public:
//    Node(Position p, Position parent,float f, float g,float h,bool walled);
    Node(int x=-1,int y=-1, int px=0, int py=0);
    void heuristic(Node G);
    void refresh(Node G);
    void update(int px, int py,float f, float g, float h);
};
bool static operator==(Node n1,Node n2){return n1.pos.x==n2.pos.x&&n1.pos.y==n2.pos.y;}
bool static operator<(Node a , Node b){ return a.f<b.f;}
bool static operator>(Node a , Node b){ return a.f>b.f;}
bool static operator<(std::list<Node>::iterator a, std::list<Node>::iterator b){ return a->f<b->f;}
bool static operator>(std::list<Node>::iterator a, std::list<Node>::iterator b){ return a->f>b->f;}

class pathfinding{
public:
    static std::array<std::array<Node, 12>, 12> map();

    //float heuristics( Position actuel , Position goal);

    static std::list<Node> Astar(Node agent , Node goal, array<array<Node,12>,12> map);

    static std::list<Node> path(array<array<Node,12>,12> map,Node goal);
};

class AStar {
public:
	AStar() = default;
	std::list<Node> chain;
};
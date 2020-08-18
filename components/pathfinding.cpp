//
// Created by lucienR on 15/02/2020.
//

#include "pathfinding.hpp"
#include <math.h>
#include <list>
#include <algorithm>
#include <vector>
#include <array>


Position::Position(int x, int y) {
	this->x = x;
	this->y = y;
//    this->value=value;
}

bool Position::equal(Position pos) {
	return this->x == pos.x && this->y == pos.y;
}

float Position::distance(Position pos) {
	return pow(pos.x - this->x, 2) + pow(pos.y - this->y, 2);
}



//Node::Node(Position p,Position parent,float f=0,float g=0,float h=0,bool walled=false ){
//
//}

void Node::heuristic(Node G) {
	this->h = this->pos.distance(G.pos);
	this->f = this->h + this->g;
}

void Node::refresh(Node G) {
	heuristic(G);
	this->g++;
	this->f = this->h + this->f;
}

Node::Node(int x, int y, int px, int py) {
	this->pos.x = x;
	this->pos.y = y;
	this->parent.x = px;
	this->parent.y = py;
}

void Node::update(int px, int py, float f, float g, float h) {
	this->parent.x = px;
	this->parent.y = py;
	this->f = f;
	this->g = g;
	this->h = h;
}

std::array<std::array<Node, 12>, 12> pathfinding::map() {
//make the map XbyX array with 1 = walls 0=road
//    int map[12][12];
	std::array<std::array<Node, 12>, 12> mapp;
//    std::vector<std::vector<Node>> mapp(12);
//    for (auto &elem : mapp) {
//        elem.resize(12);
//    }
	//float heuristicmap[12][12];
	int v2;
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			v2 = rand() % 100 + 1;
			if (v2 > 66 || i == 0 || i == 11 || j == 0 || j == 11) {
				//34% to place a wall on the case
				mapp[i][j].walled = true;
			} else {
				//0=road
				mapp[i][j].walled = false;
			}
			mapp[i][j].pos = Position(i, j);
		}
	}


	for(int i=0;i<12;i++){
		for(int j=0;j<12;j++){
			std::cout<<mapp[i][j].walled<<" ";
		}
		cout<< endl;
	}
	//validate the map through A*
	//initiate variables to validate the A*
	Node pos = Node(10, 10);
	Node goal = Node(1, 1);
//    return;
	if (Astar(pos, goal, mapp).empty()) {
		cout<<"invalid map"<<endl;
		return (map());
	}
	return mapp;
//   LoadObject::Cube({7.f, 0.5f, 7.f});
}


std::list<Node> pathfinding::path(array<array<Node, 12>, 12> map, Node goal) {

	int x = goal.pos.x;
	int y = goal.pos.y;
	std::list<Node> path;
	std::list<Node> usablePath;

	while (map[x][y].parent.x != -1 && map[x][y].parent.y != -1/* && map[x][y].pos.x != -1 && map[x][y].pos.y != -1*/) {
		path.push_back(map[x][y]);
		int tempX = map[x][y].parent.x;
		int tempY = map[x][y].parent.y;
		x = tempX;
		y = tempY;
	cout<<"next point "<<x <<" "<<y<<endl;
	}
	path.push_back(map[x][y]);

	while (!path.empty()) {
		Node top = *path.end();
		path.pop_back();
		usablePath.push_back(top);
	}
	return usablePath;
}


std::list<Node> pathfinding::Astar(Node agent, Node goal, array<array<Node, 12>, 12> map) {
	std::list<Node> neighbor;
	Node newneighbor;

	float maxdist = 200.f;
	int direction[4][2] = {{-1, 0},
			       {1,  0},
			       {0,  -1},
			       {0,  1}};

	std::list<Node> Openlist;
	std::list<Node> Closedlist;

	//init
	for (int x = 0; x < 12; x++) {
		for (int y = 0; y < 12; y++) {
			map[x][y].pos.x = x;
			map[x][y].pos.y = y;
			map[x][y].update(-1, -1, 0.f, 0.f, 0.f);
		}
	}
	//init initial point
	int x = agent.pos.x;
	int y = agent.pos.y;
	map[x][y].heuristic(goal);
	Openlist.push_back(map[x][y]);
	Node current;

	while (!Openlist.empty()) {
		Openlist.sort();
		current = *Openlist.begin();
		Openlist.pop_front();
		current.heuristic(goal);

		if (current.pos.equal(goal.pos)) {
			map[goal.pos.x][goal.pos.y].parent.x = current.parent.x;
			map[goal.pos.x][goal.pos.y].parent.y = current.parent.y;
			return path(map, goal);
		}

		for (int i = 0; i < 4; i++) {
			newneighbor.pos.x = current.pos.x + direction[i][0];
			newneighbor.pos.y = current.pos.y + direction[i][1];
			newneighbor.g = current.g + 1;
			newneighbor.heuristic(goal);

			if (!map[newneighbor.pos.x][newneighbor.pos.y].walled) {
				_List_iterator<Node> itO = std::find(Openlist.begin(), Openlist.end(), newneighbor);
				_List_iterator<Node> itC = std::find(Closedlist.begin(), Closedlist.end(), newneighbor);

				if (itO != Openlist.end()) {
					if (itO->g >= newneighbor.g) {
						itO->g = newneighbor.g;
						itO->parent.x = current.pos.x;
						itO->parent.y = current.pos.y;
						itO->heuristic(goal);
						map[newneighbor.pos.x][newneighbor.pos.y].parent = current.pos;
					}
				} else if (itC != Closedlist.end()) {
					if (itC->g >= current.g) {
						Node temp = *itC;
						temp.g = newneighbor.g;
						temp.parent.x = current.pos.x;
						temp.parent.y = current.pos.y;
						temp.heuristic(goal);
						Openlist.push_back(temp);
						Closedlist.remove(*itC);
						map[newneighbor.pos.x][newneighbor.pos.y].parent = current.pos;
					}
				} else {
					map[newneighbor.pos.x][newneighbor.pos.y].parent = current.pos;
					Openlist.push_back(newneighbor);
				}
			}
		}
		Closedlist.push_back(current);
	}

	if (!current.pos.equal(goal.pos)) {
		return std::list<Node>();
	}

	return std::list<Node>();
}
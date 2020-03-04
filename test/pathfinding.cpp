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
    return sqrt(pow(pos.x - this->x, 2) + pow(pos.y - this->y, 2));
}



//Node::Node(Position p,Position parent,float f=0,float g=0,float h=0,bool walled=false ){
//
//}

void Node::heuristic(Node G) {
    this->h = this->pos.distance(G.pos);
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

void pathfinding::map() {
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
            std::cout << i << std::endl;
            if (v2 > 99 || i == 0 || i == 11 || j == 0 || j == 11) {
                //34% to place a wall on the case
                mapp[i][j].walled = true;
            } else {
                //0=road
                mapp[i][j].walled = false;
            }
            mapp[i][j].pos = Position(i, j);
        }
    }

    //validate the map through A*
    //initiate variables to validate the A*
    Node pos = Node(10, 10);
    Node goal = Node(1, 1);
    std::list<Node> empty;
    for (int i = 0; i < mapp.size(); i++) {
        cout << " " << endl;
        for (int j = 0; j < mapp.size(); j++) {
//            if(mapp[i][j].walled){cout<<"1";}else{cout<<mapp[i][j].walled;}
            cout << mapp[i][j].walled << " ";
        }
    }
    cout << true << endl;
//    return;
    if (Astar(pos, goal, mapp) == empty) {
        cout << "test" << endl;
        return (map());
    }
    cout << "done" << endl;
//   LoadObject::Cube({7.f, 0.5f, 7.f});
}


std::list<Node> pathfinding::path(array<array<Node, 12>, 12> map, Node goal) {

    int x = goal.pos.x;
    int y = goal.pos.y;
    std::list<Node> path;
    std::list<Node> usablePath;

    while (!(map[x][y].parent.x != -1 && map[x][y].parent.y != -1)/* && map[x][y].pos.x != -1 && map[x][y].pos.y != -1*/) {
        path.push_back(map[x][y]);
        int tempX = map[x][y].parent.x;
        int tempY = map[x][y].parent.y;
        x = tempX;
        y = tempY;
        std::cout << x << " " << y << std::endl;
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
    //empty for case of failure
    std::list<Node> empty;
//    bool visited[12][12];
//    Node fringe;
//    //max dist (arbitrary even thought it could be calculated)
    float maxdist = 200.f;
//    //
//    int maxiteration = 500;
//    int iteration = 0;
//    Node current;
//    bool foundgoal = false;
    int direction[4][2] = {{-1, 0},
                           {1,  0},
                           {0,  -1},
                           {0,  1}};

    std::list<Node> Openlist;
    std::list<Node> Closedlist;

    if (agent.walled) {
        return empty;
    }

    cout << "a" << endl;
    //init
    for (int x = 0; x < 12; x++) {
        for (int y = 0; y < 12; y++) {
            map[x][y].pos.x = x;
            map[x][y].pos.y = y;
            map[x][y].update(-1, -1, maxdist, maxdist, maxdist);
        }
    }
    //init initial point
    int x = agent.pos.x;
    int y = agent.pos.y;
    map[x][y].heuristic(goal);
    map[x][y].update(x, y, map[x][y].h, 0, map[x][y].h);
    map[x][y].parent.x = -1;
    map[x][y].parent.y = -1;
    Openlist.push_back(map[x][y]);
    Node current;
//    potentialneighbor.push_back(map[x][y]);

    cout << "b" << endl;
//    while the OPEN list is not empty {
    while (!Openlist.empty()) {
        std::cout << "LOOK AT ME " << Openlist.size() << std::endl;
//           Take from the open list the node node_current with the lowest
        Openlist.sort();
        current = *Openlist.begin();
        Openlist.pop_front();
        current.heuristic(goal);
        current.f = current.g + current.h;
//            4 f(node_current) = g(node_current) + h(node_current)
//            5 if node_current is node_goal we have found the solution; break
        if (current.pos.equal(goal.pos)) {
            return path(map, goal);
        }
//            6 Generate each state node_successor that come after node_current
//    for each node_successor of node_current {
        for (int i = 0; i < 4; i++) {
            newneighbor.pos.x = current.pos.x + direction[i][0]; /// Valide
            newneighbor.pos.y = current.pos.y + direction[i][1]; /// Valide
            //if neighbor is a wall -> skip
            if (newneighbor.pos.x < 11 && newneighbor.pos.x > 0 && /// Valide
                newneighbor.pos.y < 11 && newneighbor.pos.y > 0 &&
                !map[newneighbor.pos.x][newneighbor.pos.y].walled) { /// Valide
//          8 Set successor_current_cost = g(node_current) + w(node_current, node_successor)
                current.g++; /// Valide
//          neighbor.push_back(newneighbor);
                _List_iterator<Node> itO = std::find(Openlist.begin(), Openlist.end(), newneighbor);
                _List_iterator<Node> itC = std::find(Closedlist.begin(), Closedlist.end(), newneighbor);
//          9 if node_successor is in the OPEN list {
                if (itO != Openlist.end()) {
//              10 if g(node_successor) ≤ successor_current_cost continue (to line 20)
                    if (itO->g >= current.g) {
                        //              18 Set g(node_successor) = successor_current_cost (changed-> in every if)
                        itO->g = current.g;
                        //              19 Set the parent of node_successor to node_current(changed -> in every if)
                        itO->parent.x = current.pos.x;
                        itO->parent.y = current.pos.y;
                    }
                } else if (itC != Closedlist.end()) {
//              11 } else if node_successor is in the CLOSED list {
//              12 if g(node_successor) ≤ successor_current_cost continue (to line 20)
//              13 Move node_successor from the CLOSED list to the OPEN list
                    if (itC->g >= current.g) {
                        Node temp = *itC;
//                      18 Set g(node_successor) = successor_current_cost (changed-> in every if)
                        itC->g = current.g;
//                      19 Set the parent of node_successor to node_current(changed -> in every if)
                        itC->parent.x = current.pos.x;
                        itC->parent.y = current.pos.y;
                        Openlist.push_back(temp);
                        Closedlist.remove(temp);
                    }
                } else {
                    newneighbor.g = current.g;
                    newneighbor.parent.x = current.pos.x;
                    newneighbor.parent.y = current.pos.y;
                    newneighbor.heuristic(goal);
                    Openlist.push_back(newneighbor);
                }
            } else {
//              14 } else {
//              15 Add node_successor to the OPEN list
//              16 Set h(node_successor) to be the heuristic distance to node_goal
//              17 }

//              18 Set g(node_successor) = successor_current_cost (changed-> in every if)
//              19 Set the parent of node_successor to node_current(changed -> in every if)
            }
//           18 Set g(node_successor) = successor_current_cost (changed-> in every if)
//           19 Set the parent of node_successor to node_current(changed -> in every if)
//      20 }
        }
//    21 Add node_current to the CLOSED list
        Closedlist.push_back(current);
    }
//    }

//    22 }
//23 if(node_current != node_goal) exit with error (the OPEN list is empty)

    cout << "c" << endl;
    if (!current.pos.equal(goal.pos)) {
        return empty;
    }
    return std::list<Node>();
}





//
//    //loop of action
//    while (!potentialneighbor.empty() && iteration < maxiteration) {
//        iteration++;
//        Node node;
////        //reset the f value of the potential neighbor
////        do {
////            float temp = maxdist;
////            std::list<Node>::iterator itNode;
////            std::cout << "init" << std::endl;
////            for (std::list<Node>::iterator it = potentialneighbor.begin();it != potentialneighbor.end(); it = next(it)) {
////                Node n = *it;
////                if (n.f < temp) {
////                    temp = n.f;
////                    std::cout << "j ai chope le run" << std::endl;
////                    itNode = it;
////                }
////            }
////            std::cout << "zenit" << std::endl;
////            node = *itNode;
////            potentialneighbor.erase(itNode);
////        } while (!node.walled);
//
//        std::cout<<"aaa"<<std::endl;
//        current=node;
//        if(current.pos.equal(goal.pos)){
//            std::list<Node> path;path.push_back(current);
//            return path;
//        }
//
//
//        //for each neighbor and update f + goal check.
//        //on the left side
//        if (!map[current.pos.x - 1][current.pos.y].walled) {//if not a wall
//            fringe.pos.x = current.pos.x;
//            fringe.pos.y = current.pos.y - 1;
//            if (!visited[fringe.pos.x][fringe.pos.y]) {
//                fringe.refresh(goal);
//                if (fringe == goal) {
//                    path(map, goal);
//                    foundgoal = true;
//                }
//                if (fringe.f > map[fringe.pos.x][fringe.pos.y].f) {
//                    map[fringe.pos.x][fringe.pos.y].update(fringe.parent.x, fringe.parent.y, fringe.f, fringe.g,fringe.h);
//                    potentialneighbor.push_back(map[fringe.pos.x][fringe.pos.y]);
//                }
//            }
//        }
//        //right side
//        if (!map[current.pos.x + 1][current.pos.y].walled) {
//            fringe.pos.x = current.pos.x;
//            fringe.pos.y = current.pos.y - 1;
//            if (!visited[fringe.pos.x][fringe.pos.y]) {
//                fringe.refresh(goal);
//                if (fringe == goal) {
//                    path(map, goal);
//                    foundgoal = true;
//                }
//                if (fringe.f > map[fringe.pos.x][fringe.pos.y].f) {
//                    map[fringe.pos.x][fringe.pos.y].update(fringe.parent.x, fringe.parent.y, fringe.f, fringe.g,fringe.h);
//                    potentialneighbor.push_back(map[fringe.pos.x][fringe.pos.y]);
//                }
//            }
//        }
//        //down
//        if (!map[current.pos.x][current.pos.y - 1].walled) {
//            fringe.pos.x = current.pos.x;
//            fringe.pos.y = current.pos.y - 1;
//            if (!visited[fringe.pos.x][fringe.pos.y]) {
//                fringe.refresh(goal);
//                if (fringe == goal) {
//                    path(map, goal);
//                    foundgoal = true;
//                }
//                if (fringe.f > map[fringe.pos.x][fringe.pos.y].f) {
//                    map[fringe.pos.x][fringe.pos.y].update(fringe.parent.x, fringe.parent.y, fringe.f, fringe.g,fringe.h);
//                    potentialneighbor.push_back(map[fringe.pos.x][fringe.pos.y]);
//                }
//            }
//        }
//        //up
//        if (!map[current.pos.x][current.pos.y + 1].walled) {
//            fringe.pos.x, fringe.pos.y = current.pos.x, current.pos.y + 1;
//            if (!visited[fringe.pos.x][fringe.pos.y]) {
//                fringe.refresh(goal);
//                if (fringe == goal) {
//                    path(map, goal);
//                    foundgoal = true;
//
//                }
//                if (fringe.f > map[fringe.pos.x][fringe.pos.y].f) {
//                    map[fringe.pos.x][fringe.pos.y].update(fringe.parent.x, fringe.parent.y, fringe.f, fringe.g,fringe.h);
//                    potentialneighbor.push_back(map[fringe.pos.x][fringe.pos.y]);
//                }
//            }
//        }
//    }
//    if (!foundgoal) {
//        return (empty);
//    }
//    return empty;
//}

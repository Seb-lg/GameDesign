//
// Created by lucienR on 15/02/2020.
//

#include "pathfinding.hpp"
#include <math.h>
#include <list>
#include <algorithm>
#include <vector>


Position::Position(int x, int y, int value=0){
    this->x = x;
    this->y = y;
    this->value=value;
}

bool Position::equal(Position pos) {
    if(this->x==pos.x && this->y==pos.y){
        return true;
    }
    return false;
}

void pathfinding::map() {
//make the map XbyX array with 1 = walls 0=road
    int map[12][12];
    int heuristicmap[12][12];
    int v2;
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            v2 = rand() % 100 + 1;
            if (v2 > 34 || i == 0 || i == 11 || j == 0 || j == 11) {
                //34% to place a wall on the case
                map[i][j] = 1;
            } else {
                //0=road
                map[i][j] = 0;
            }
            //indicate the heuristicmap
            heuristicmap[i][j] = sqrt(pow(i, 2) + pow(j, 2));
        }
    }
    //initiate variables to validate the A*
    Position pos = Position(10, 10);
    Position goal = Position(1, 1);
    Astar(pos, goal, map);
}


void pathfinding::heuristics(Position pos, Position goal) {
//get map /rigidbody
//map the map
}


void pathfinding::Astar(Position pos, Position goal, int map[12][12]) {
    //validate the map through A*
    int cost=0;
//    std::list<int[2]> neighbor;
    std::list<Position> neighbor;
    Position newneighbor;
    //loop of action
    while (pos.equal(goal)!= true) {
        //neighbor
        //on the left side on the map

        if (map[pos.x - 1, pos.y] == 0) {
            newneighbor.x , newneighbor.y = pos.x - 1, pos.y;
            if (std::find(neighbor.begin(), neighbor.end(), newneighbor) != neighbor.end()) {
                neighbor.push_back(newneighbor);
            }
        }
        if (map[pos.x + 1 , pos.y] == 0) {
            newneighbor.x , newneighbor.y = pos.x + 1, pos.y;
            if (std::find(neighbor.begin(), neighbor.end(), newneighbor) != neighbor.end()) {
                neighbor.push_back(newneighbor);
            }
        }
        if (map[pos.x , pos.y - 1] == 0) {
            newneighbor.x, newneighbor.y = pos.x, pos.y - 1 ;
            if (std::find(neighbor.begin(), neighbor.end(), newneighbor) != neighbor.end()) {
                neighbor.push_back(newneighbor);
            }
        }
        if (map[pos.x , pos.y + 1] == 0) {
            newneighbor.x, newneighbor.y = pos.x, pos.y + 1;
            if (std::find(neighbor.begin(), neighbor.end(), newneighbor) != neighbor.end()) {
                neighbor.push_back(newneighbor);
            }
        }


        //choice of neighbor to go + depop in list
        auto pos = *std::min(neighbor.begin(),neighbor.end());
        neighbor.remove(*std::min(neighbor.begin(),neighbor.end()));
        cost++;

        //update neighbor value
        for(int it=0;it<neighbor.size();it++){
            //change
        }

//load the map / item to avoid

//load the heuristic to the player for each point

//issue next point to go to.
    }
}

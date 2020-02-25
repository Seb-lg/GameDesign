//
// Created by lucienR on 15/02/2020.
//

#include "pathfinding.hpp"

void pathfinding::map(){
//make the map XbyX array with 1 = walls 0=road
    int map [10][10];
    int heuristicmap[10][10];
    int v2;
    for(int i;i<length;i++) {
        for(int j;map[i][j];j++){
            v2 = rand() % 100 + 1;
            if(v2>50){
                map[i][j]=1;
            } else{
                map[i][j]=0;
            }
            heuristicmap[i][j]=squrt(pow(i,2)+pow(j,2);
        }
    }
//validate the map through A*
    int path [];
    int pos[2]={9,9};
//loop of action
    while(pos!={0,0}){
        
    }
}

void pathfinding::heuristics(){
//get map /rigidbody
//map the map


}


void pathfiding::Astar(){
//load the map / item to avoid

//load the heuristic to the player for each point

//issue next point to go to.
}
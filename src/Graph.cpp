//
// Created by glevy on 11/4/2020.
//

#include "../include/Graph.h"
#include "../include/Session.h"

Graph::Graph(): edges(),matrixToReturn(){} //empty constructor

Graph::Graph(std::vector<std::vector<int>> matrix): edges(), matrixToReturn(){
    //creates adjancy list for all agents, the first column represents the agent's status
    int size = matrix.size();
    for(int i=0; i<size; i++){
        std::vector<int> v;
        v.push_back(0); // init healty node, the first value is an indicator for infected node
        for (int j=0; j<size; j++){
            if(matrix[i][j] == 1){
                v.push_back(j);}}
        edges.push_back(v);}
    matrixToReturn = matrix;} //constructor

void Graph::infectNode(int nodeInd) { //changes the agents status to occ -1 or sick -2
    if(edges[nodeInd][0] == 0){
        edges[nodeInd][0] =1;}
    else
        edges[nodeInd][0] = 2;}

bool Graph::isInfected(int nodeInd) {
    //first column represents the health condition of the node -> 2-sick  1-occ 0-healthy
    if (edges[nodeInd][0] != 0)
        return true;
    return false;}

int Graph::infectNeighbour(int nodeInd) {
    //finds the closest neighbour to infect
    int size = edges[nodeInd].size();
    for (int i=1 ; i< size ; i++){
        if(!isInfected(edges[nodeInd][i])){
            return edges[nodeInd][i];}}
    return -1;}

int Graph::getAgentStatus(int nodeInd) {
    return edges[nodeInd][0];
}

void Graph::disconnectEdges(int nodeInd) {
    // erase nodeInd's neighboures from his vector
    int size = edges[nodeInd].size();
    for (int i = 1; i < size; ++i) {
        edges[nodeInd].pop_back();
    }
    //erase nodeInd from his neighbours's vector
    int edgesSize = edges.size();
    for(int i=0; i<edgesSize; i++){
        //matrixToreturn edit
        matrixToReturn[nodeInd][i]=0;
        matrixToReturn[i][nodeInd]=0;
        //edges edit
        int sizei = edges[i].size();
        for(int j=1; j<sizei; j++){
            if(edges[i][j] == nodeInd){
                edges[i].erase(edges[i].begin()+(j));}}}}

std::vector<std::vector<int>> Graph::getEdges() const {
    return edges;}

void Graph::changeStatusForBfs(int nodeInd) {
    edges[nodeInd][0] = 3;
}

std::vector<std::vector<int>> Graph::getMatrix() const {
    return matrixToReturn;
}

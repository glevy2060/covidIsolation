//
// Created by glevy on 11/4/2020.
//

#include "../include/Session.h"
#include <fstream>
#include "../include/Agent.h"
#include <iostream>
#include "../include/json.hpp"
using json = nlohmann::json ;
using namespace std;

//constructor
Session::Session(const std::string &path): g(), treeType(), agents(), infectedQueue(), cycleC(0){
    ifstream temp(path);
    json j;
    temp >> j;
    //treeType init
    std:: string tree = j["tree"];
    if(tree == "M")
        treeType = MaxRank;
    if(tree == "R")
        treeType = Root;
    if(tree == "C")
        treeType = Cycle;

    //graph init
    g = Graph(graphReader(j));
    std::vector<std::pair<std::string, int>> jsonAgents = j["agents"];
    infectedQueue = std::vector<int>();

    //agents init
    for(std::pair<std::string,int> h:jsonAgents){
        if(h.first.compare("C") == 0)
            addAgent(ContactTracer());
        else {
            addAgent(Virus(h.second));
            g.infectNode(h.second);
        }}
}

//destructor
Session::~Session() {
    int size = agents.size();
    for(int i=size-1 ; i >= 0; i--){
        if (agents[i])
            delete agents[i];
    }
    agents.clear();
    infectedQueue.clear();
}

//copy Constructor
Session::Session(const Session &other): g(other.g), treeType(other.treeType), agents(), infectedQueue(), cycleC(other.cycleC){
    int size = other.agents.size();
    for(int i=0; i<size; i++){
        infectedQueue.push_back(other.infectedQueue[i]);
        agents.push_back(other.agents[i]->clone());}
}

//copy Assignment
Session & Session::operator=(const Session &other) {
    if (this != &other) {
        if (!agents.empty()) {
            int size = agents.size();
            for (int i = size - 1; i >= 0; i--) {
                if (agents[i])
                    delete agents[i];
            }
            agents.clear();
        }
        int size = other.agents.size();
        for (int i = 0; i < size; i++) {
            Agent *a = other.agents[i]->clone();
            agents.push_back(a);
        }
        g = other.g;
        treeType = other.treeType;
        cycleC = other.cycleC;
        int sizeO = other.infectedQueue.size();
        for (int i = 0; i < sizeO; i++)
            infectedQueue.push_back(other.infectedQueue[i]);
    }
    return *this;
}

//moveCTR
Session::Session(Session &&other): g(other.g), treeType(other.treeType), agents(), infectedQueue(), cycleC(other.cycleC) {
    int size = other.agents.size();
    for(int i = 0; i<size; i++){
        agents.push_back(other.agents[i]);
        other.agents[i] = nullptr;
    }
    other.agents.clear();
    int sizeO = other.infectedQueue.size();
    for (int i = 0; i < sizeO; i++)
        infectedQueue.push_back(other.infectedQueue[i]);
}

//move assignment
Session & Session::operator=(Session &&other) {
    if(this != &other){
        if(!agents.empty()) {
            int size = agents.size();
            for (int i = size - 1; i >= 0; i--) {
                if (agents[i])
                    delete agents[i];
            }
            agents.clear();
        }
            int otherSize = other.agents.size();
            for(int i=0; i<otherSize; i++){
                agents.push_back(other.agents[i]);}
            other.agents.clear();
        g = other.g;
        treeType = other.treeType;
        infectedQueue = other.infectedQueue;
        cycleC = other.cycleC;
    }
    return *this;
}

std::vector<std::vector<int>> Session::graphReader(nlohmann::json j) {
    // called from session, Graph init
    std::vector<std::vector<int>> matrix;
    nlohmann::json input = j["graph"];
    for(auto& value: input.items()){
        nlohmann::json row = value.value();
        std::vector<int> currRow;
        for(auto &cell: row.items()){
            int edge = cell.value();
            currRow.push_back(edge);}
        matrix.push_back(currRow);}
    return matrix;
}

void Session::enqueueInfected(int nodeInd) {
    infectedQueue.push_back(nodeInd);
}

int Session::dequeueInfected() {
    if(!infectedQueue.empty()){
        int firstTogo = infectedQueue[0];
        infectedQueue.erase(infectedQueue.begin());
        return firstTogo;
    }
    return -1;
}

void Session::addAgent(const Agent &agent) {
    Agent* clone = agent.clone();
    agents.push_back(clone);
}

void Session::simulate() {
    while(!gameOver()){
        int size = agents.size();
        for(int i =0; i<size; i++){
            agents[i]->act(*this);}
        cycleC++;}

    nlohmann::json j;
    j["graph"] = g.getMatrix();
    std::vector<int> infectedOutput;
    int size = agents.size();
    for(int i=0; i<size; i++){
        if(agents[i]->getNode()!=-1)
            infectedOutput.push_back(agents[i]->getNode());}

    j["infected"] = infectedOutput;
    ofstream i("./output.json");
    i << j;
}

Graph Session::getGraph() {
    return g;
}

Graph & Session::getGraphPointer() {
    return g;
}

void Session::disconnectGraph(int nodeToDisconnect) {
    g.disconnectEdges(nodeToDisconnect);
}

void Session::setGraph(const Graph &graph) {
    g = graph;};

TreeType Session::getTreeType() const {
    return treeType;}

int Session::getCycleC() const {
    return cycleC;}

bool Session::gameOver() const{
    //checks if all graph's components are infected or healthy
    int size = g.getEdges().size();
    for(int i =0; i< size; i++){
        int status = g.getEdges()[i][0];
        if(status == 1) return false;
        int sizeI = g.getEdges()[i].size();
        for(int j=1; j<sizeI; j++){
            int neighbour = g.getEdges()[i][j];
            if(g.getEdges()[neighbour][0] != status)
                return false;}}
    return true;
}
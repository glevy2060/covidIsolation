#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>

class Graph{
public:
    Graph(); //empty constructor
    Graph(std::vector<std::vector<int>> matrix); //constructor
    void infectNode(int nodeInd); // changes node's status to infected
    bool isInfected(int nodeInd); // checks if the node is infected
    int infectNeighbour(int nodeInd); // returns the next neighbour to infect
    int getAgentStatus(int nodeInd); //returns the agent status 1- occ 2- sick
    void disconnectEdges(int nodeInd);
    std::vector<std::vector<int>> getEdges() const;
    void changeStatusForBfs(int nodeInd);
    std::vector<std::vector<int>> getMatrix() const;

private:
    std::vector<std::vector<int>> edges;
    std::vector<std::vector<int>> matrixToReturn;
};

#endif

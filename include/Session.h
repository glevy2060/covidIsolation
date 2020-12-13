#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <string>
#include "Graph.h"
#include "json.hpp"

class Agent;

enum TreeType{
  Cycle,
  MaxRank,
  Root
};

class Session{
public:

    Session(const std::string& path); //constructor
    Session& operator= (const Session& other); // copy assingment
    virtual ~Session(); //destructorr
    Session(const  Session& other); //copyCTR
    Session(Session&& other); //moveCTR
    Session& operator= (Session&& other); //move assignment
    void simulate(); // runs in loop, until determination
    void addAgent(const Agent& agent);
    void setGraph(const Graph& graph);
    Graph getGraph ();
    Graph& getGraphPointer();
    void disconnectGraph(int nodeToDisconnect);
    void enqueueInfected(int);
    int dequeueInfected();
    void setInfected();
    TreeType getTreeType() const;
    int getCycleC() const;
    bool gameOver() const;
    std::vector<std::vector<int>> graphReader(nlohmann::json j);

private:
    Graph g;
    TreeType treeType;
    std::vector<Agent*> agents;
    std::vector<int> infectedQueue;
    int cycleC;
};

#endif

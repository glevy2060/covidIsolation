#ifndef AGENT_H_
#define AGENT_H_

#include <vector>
#include "Session.h"
#include "Tree.h"
class Agent{
public:
    Agent();
    virtual void act(Session& session)=0;
    virtual Agent* clone() const =0;
    virtual int getNode()=0;
    virtual ~Agent() = default;
};

class ContactTracer: public Agent{
public:
    ContactTracer(); //constructor
    virtual Agent* clone() const;
    virtual int getNode();
    virtual void act(Session& session); //dequeue the infected node, runs bfs algorithm from the infected node (creates tree)

private:
    void bfs(Graph g, Tree& tree, Session& session);
    void buildSonsVector(Tree& tree, std::vector<Tree*>& queue, Graph& g, Session& session);
};

class Virus: public Agent{
public:
    Virus(int nodeInd); //constructor
    virtual Agent* clone() const;
    virtual int getNode();
    virtual void act(Session& session); // infected the closest node

private:
    const int nodeInd;
};

#endif

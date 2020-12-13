//
// Created by glevy on 11/4/2020.
//

#include "../include/Agent.h"
#include "../include/Graph.h"

Agent::Agent() {};

//------------------------------------------------ContactTracer-------------------------------------------

ContactTracer::ContactTracer() {};//constructor

Agent * ContactTracer::clone() const {
    return new ContactTracer(*this);
}

int ContactTracer::getNode() {
    return -1;
}

void ContactTracer::act(Session &session) {
    int popedNode = session.dequeueInfected();
    if(popedNode != -1){
        Tree* tree = Tree::createTree(session, popedNode);
        bfs(session.getGraph(), *tree, session);
        int nodeToDisconnect = tree->traceTree();
        session.disconnectGraph(nodeToDisconnect);
        if(tree) delete tree;
    }

}

void ContactTracer::buildSonsVector(Tree &tree, std::vector<Tree*> &queue, Graph& g, Session& session) {
    int size = g.getEdges()[tree.getNode()].size();;
    for(int i=1 ; i<size; i++){
        int childInd = g.getEdges()[tree.getNode()][i];
        if(g.getEdges()[childInd][0] != 3){
            Tree* childTree = Tree::createTree(session, childInd);
            tree.addChild(*childTree);
            g.changeStatusForBfs(childInd);
            queue.push_back(tree.getChildren()[tree.getChildren().size()-1]);
            if (childTree) delete childTree;
        }
    }

}

void ContactTracer::bfs(Graph g, Tree &tree, Session& session) {
    std::vector<Tree*> bfsQueue;
    int root = tree.getNode();
    g.changeStatusForBfs(root);
    bfsQueue.push_back(&tree);

    while (!bfsQueue.empty()){
        Tree* popedNode = bfsQueue[0];
        bfsQueue.erase(bfsQueue.begin());
        buildSonsVector(*popedNode, bfsQueue, g, session);
        g.disconnectEdges(popedNode->getNode());
    }
    g.disconnectEdges(root);
}

//------------------------------------------------Virus-------------------------------------------
Virus::Virus(int nodeInd): nodeInd(nodeInd) {} //constructor

Agent * Virus::clone() const {
    return new Virus(*this);
}

int Virus::getNode() {
    return nodeInd;
}

void Virus::act(Session &session) {
    if(session.getGraphPointer().getAgentStatus(nodeInd) == 1){
        session.getGraphPointer().infectNode(nodeInd);
        session.enqueueInfected(nodeInd);
    }
    int neighbourToInfect = session.getGraphPointer().infectNeighbour(nodeInd);
    if(neighbourToInfect!= -1) {
        Virus const v =  Virus(neighbourToInfect);
        session.addAgent(v);
        session.getGraphPointer().infectNode(neighbourToInfect);
    }
}


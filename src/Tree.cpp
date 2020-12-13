//
// Created by glevy on 11/4/202
#include "../include/Tree.h"
#include "../include/Session.h"

//constructor
Tree::Tree(int rootLabel): children(), node(rootLabel) {};

//destructor
Tree::~Tree()  {
    int size = children.size();
    for(int i=size-1 ; i >= 0; i--){
        if(children[i]){
            delete children[i];}
    }
    children.clear();
}

//copy CTR
Tree::Tree(const Tree &other): children(), node(other.node){
    int size = other.children.size();
    for(int i=0; i<size; i++){
        children.push_back(other.children[i]->clone());}
}

//copy Assignment
Tree & Tree::operator=(const Tree &other) {
    if(this!=&other){
        if(!children.empty()){
            int size = children.size();
            for(int i=size-1 ; i >= 0; i--){
                if(children[i])
                    delete children[i];}}
        children.clear();
        int otherChildrenSize = other.children.size();
        for(int i=0; i<otherChildrenSize; i++){
            Tree* t = other.children[i];
            addChild(*t);}}
    return *this;
}

//moveCTR
Tree::Tree(Tree &&other): children(), node(other.node){
    other.node = 0;
    int size = other.children.size();
    for(int i=0;i<size;i++)
        children.push_back(other.children[i]);
    for(int i = 0; i<size; i++){
        other.children[i] = nullptr;
    }
    other.children.clear();
}

//move assignment
Tree & Tree::operator=(Tree &&other) {
    if(this != &other) {
        if (!children.empty()) {
            int size = children.size();
            for (int i = size - 1; i >= 0; i--) {
                if (children[i])
                    delete children[i];
            }
            children.clear();
        }
        int otherSize = other.children.size();
        for (int i = 0; i < otherSize; i++) {
            children.push_back(other.children[i]);
        }
        other.children.clear();
        node = other.node;
    }
    return *this;
}

void Tree::addChild(const Tree &child) {
    Tree* copy = child.clone();
    children.push_back(copy);
}

Tree * RootTree::clone() const {
    return new RootTree(*this);
}

std::vector<Tree *> Tree::getChildren() {
    return children;
}

//------------------------------------------------RootTree-------------------------------------------
RootTree::RootTree(int rootLabel) : Tree(rootLabel) {}

int RootTree::traceTree() {
    return getNode();
}

//MaxRankTree
MaxRankTree::MaxRankTree(int rootLabel) :Tree(rootLabel){}

int MaxRankTree::traceTree() {
    int maxSize = -1;
    int nodeToReturn = -1;
    std::vector<Tree*> queue;
    queue.push_back(this);
    while(!queue.empty()){
        Tree* currTree = queue[0];
        queue.erase(queue.begin());
        int childrenVectorSize = currTree->getChildren().size();
        for(int i=0; i<childrenVectorSize; i++){
            queue.push_back(currTree->getChildren()[i]);
        }

        if(childrenVectorSize > maxSize){
            maxSize = childrenVectorSize;
            nodeToReturn = currTree->getNode();
        }
    }
    return nodeToReturn;
};

Tree* MaxRankTree::clone() const {
    return new MaxRankTree(*this);
}

Tree * Tree::createTree(const Session &session, int rootLabel) {
    Tree * treeToReturn;
    if(session.getTreeType() == 0) // Cycle
       treeToReturn = new CycleTree(rootLabel, session.getCycleC());
    else if(session.getTreeType() == 1)
        treeToReturn = new MaxRankTree(rootLabel);
    else
        treeToReturn = new RootTree(rootLabel);

    return treeToReturn;
}

//------------------------------------------------CycleTree-------------------------------------------
//constructor
CycleTree::CycleTree(int rootLabel, int currCycle): Tree(rootLabel), currCycle(currCycle){}

int CycleTree::traceTree() {
    Tree* treeToUse = this;
    for(int i=0; i<currCycle; i++){
        if(treeToUse->getChildren().empty())
            return treeToUse->getNode();
        else
            treeToUse = treeToUse->getChildren()[0];
    }
    return treeToUse->getNode();
}
//CycleTree::CycleTree(CycleTree &&other) :Tree(other),currCycle(other.currCycle){}
//CycleTree::CycleTree(const CycleTree &other) :Tree(other),currCycle(other.currCycle){}
//CycleTree & CycleTree::operator=(CycleTree &&other) {
//    Tree::operator=(other);
//    currCycle=other.currCycle;
//    return *this;
//}
//CycleTree & CycleTree::operator=(const CycleTree &other) {
//    Tree::operator=(other);
//    currCycle=other.currCycle;
//    return *this;
//}
Tree* CycleTree::clone() const {
    return new CycleTree(*this);
}
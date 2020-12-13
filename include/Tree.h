#ifndef TREE_H_
#define TREE_H_

#include <vector>

class Session; // check

class Tree{
public:
    Tree(int rootLabel); //source node, bfs started from it
    virtual ~ Tree(); // destructor
    Tree& operator=(const Tree& other); //copy Assignment
    Tree(const Tree& other); //copy CTR
    Tree(Tree&& other); // moveCTR
    Tree& operator= (Tree&& other); //move assignment
    void addChild(const Tree& child);
    static Tree* createTree(const Session& session, int rootLabel);
    virtual int traceTree()=0; //return the detected node's index
    std::vector<Tree*> getChildren();
    virtual Tree* clone() const=0;

    int getNode(){
        return node;
    }

protected:
    std::vector<Tree*> children;
    int node;

};

class CycleTree: public Tree{
public:
    CycleTree(int rootLabel, int currCycle);
//    CycleTree& operator=(const CycleTree& other); //copy Assignment
//    CycleTree(const CycleTree& other); //copy CTR
//    CycleTree(CycleTree&& other); // moveCTR
//    CycleTree& operator= (CycleTree&& other); //move assignment
    virtual int traceTree(); // goes left c times (c is number of iteration)
    virtual Tree * clone() const;
private:
    int currCycle;
};

class MaxRankTree: public Tree{
public:
    MaxRankTree(int rootLabel);
    virtual int traceTree(); // returns the node with the largest amount of sons
    // if there is a tie: returns the one with the smallest depth
    // if there is a tie returns the left one
    virtual Tree * clone() const;

};

class RootTree: public Tree{
public:
    RootTree(int rootLabel);
    virtual int traceTree(); // returns root's index
    virtual Tree * clone() const;

};

#endif

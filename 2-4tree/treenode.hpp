//
//  treenode.hpp
//  2-4tree
//
//  Created by 黄梓原 on 01/10/2017.
//  Copyright © 2017 MotaWolley. All rights reserved.
//

#ifndef treenode_hpp
#define treenode_hpp

#include <stdio.h>
#include "studentrecord.hpp"
class TreeNode
{
public:
    
    int n; //number of keys -1!!!
    int keys[4] = {};
    TreeNode *childs[4] = {}; //the fourth one if for leaves only, maybe not necessary, can just use the first of the leaves
    bool isLeaf;
    studentRecord *records[4] = {}; //this one only used if the treenode is a leaf
    
    //constructor
    TreeNode();
    //destructor
    ~TreeNode();
    
    //split the ith node's child on this node, under the condition that this node is not full
    //usually split when the child node has 4 keys (0,1,2,3)
    void split(int i);
    
    void insertNonFull (int k);
    
    void insertMax(int k);
    
    struct searchReturn{
        TreeNode* x;
        int i;
    };
    
    //only apply this function when the treenode is a root!!!
    searchReturn search (int k);
    
    void printNode();
    bool verifyNode();
    bool verifyChild();
    
private:
    
};

#endif /* treenode_hpp */

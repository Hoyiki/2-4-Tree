//
//  tree.hpp
//  2-4tree
//
//  Created by 黄梓原 on 01/10/2017.
//  Copyright © 2017 MotaWolley. All rights reserved.
//

#ifndef tree_hpp
#define tree_hpp

#include <stdio.h>
#include "treenode.hpp"
#include <map>
#include <queue>
#include <set>

class Tree
{
public:
    Tree();
    ~Tree();
    TreeNode *root;
    TreeNode *firstLeaf;
    int layers;
    
    void printTree();
    
    void verify();
    
    void printLeaves();
    
    void popularCourses (int k);
    
    void insert(int k, string courseID, string courseName, string grade);
    
    void find(int k);
    
    void del(int k, string cID);
    
    void gpaOne (int k);
    
    void gpaAverage (int a, int b);
    
    void findRange(int a, int b);
};

#endif /* tree_hpp */

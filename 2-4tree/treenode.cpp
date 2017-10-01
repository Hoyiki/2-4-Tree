//
//  treenode.cpp
//  2-4tree
//
//  Created by 黄梓原 on 01/10/2017.
//  Copyright © 2017 MotaWolley. All rights reserved.
//

#include "treenode.hpp"

//constructor
TreeNode :: TreeNode(){
    n = -1;
}
//destructor
TreeNode :: ~TreeNode(){
    for (int i=0; i<=n; i++){
        if (isLeaf){
            delete records[i];
        }
        else{
            delete childs[i];
        }
    }
}

//split the ith node's child on this node, under the condition that this node is not full
//usually split when the child node has 4 keys (0,1,2,3)
void TreeNode :: split(int i){
    TreeNode *z = new TreeNode();
    TreeNode *y = childs[i];
    int t = (y -> n + 1)/2;
    z -> isLeaf = y -> isLeaf;
    z -> n = y -> n - t;
    for (int j = 0; j <= t-1; j++){
        z -> keys[j] = y -> keys[j+t];
        if (z -> isLeaf){
            z -> records[j] = y -> records[j+t];
        }
    }
    if (not y -> isLeaf){
        for (int j = 0; j <= t-1; j++){
            z -> childs[j] = y -> childs[j+t];
        }
    }
    y -> n = t-1; //just ignore the other two in this way, without deleting
    for (int j = n+1; j >= i+1; j--){
        keys[j] = keys[j-1];
    }
    for (int j = n+1; j >= i+2; j--){
        childs[j] = childs[j-1];
    }
    childs[i+1] = z;
    keys[i] = y -> keys[t-1];
    n = n+1;
    if (y -> isLeaf){
        if (y -> childs[0] == NULL){
            y -> childs[0] = z;
        }
        else{
            z -> childs[0] = y -> childs[0];
            y -> childs[0] = z;
        }
    }
}

void TreeNode :: insertNonFull (int k){
    int i = n;
    if (isLeaf){
        while (i >= 0 && k < keys[i]){
            keys[i+1] = keys[i];
            records[i+1] = records[i];
            //also need to move the student information here!!!!!
            i = i-1;
        }
        keys[i+1] = k;
        n = n+1;
    }
    else{
        while (i >= 0 && k < keys[i]){
            i = i-1;
        }
        i = i+1;
        if (childs[i] -> n == 3){ //the case that the child node is full
            split(i);
            if (k > keys[i]){
                i = i+1;
            }
        }
        childs[i] -> insertNonFull (k);
    }
}

void TreeNode :: insertMax(int k){
    int i = n;
    if (isLeaf){
        keys[i+1] = k;
        n = n+1;
    }
    else{
        if (childs[i] -> n == 3){
            split(i);
            i = i+1;
        }
        keys[i] = k; //update the parent node first!!!
        childs[i] -> insertMax(k);
    }
}

TreeNode::searchReturn TreeNode:: search (int k){
    TreeNode::searchReturn notExist = {NULL, -1};
    if (n == -1){ //this is an empty tree
        return notExist;
    }
    if (k>keys[n]){
        return notExist;
    }
    
    int i = 0;
    while (i<=n-1 && k>keys[i]){
        i = i + 1;
    }
    
    if (i<=n && k==keys[i]){
        TreeNode *currentNode = this;
        if (not currentNode -> isLeaf){
            while (not currentNode -> isLeaf){
                i = currentNode -> n;
                currentNode = currentNode -> childs[i];
            }
            i = currentNode -> n;
        }
        TreeNode::searchReturn find = {currentNode, i};
        return find;
    }
    if (isLeaf){
        return notExist;
    }
    else{
        return (childs[i] -> search(k));
    }
}

void TreeNode :: printNode(){
    cout << "--\n";
    for (int i = 0; i <= n; i++){
        cout << keys[i] << endl;
    }
    cout << "--\n";
}

bool TreeNode :: verifyNode(){
    bool vn = true;
    for (int i=0; i<=n-1; i++){
        if (keys[i] >= keys[i+1]){
            vn = false;
        }
    }
    return vn;
}
bool TreeNode :: verifyChild(){
    bool vc = true;
    if (isLeaf){
        return true;
    }
    for (int i=0; i<=n; i++){
        if (keys[i] != childs[i] -> keys[childs[i]->n]){
            vc = false;
        }
        if (not childs[i]->verifyChild()){
            vc = false;
        }
    }
    return vc;
}


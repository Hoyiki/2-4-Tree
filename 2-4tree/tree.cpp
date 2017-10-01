//
//  tree.cpp
//  2-4tree
//
//  Created by 黄梓原 on 01/10/2017.
//  Copyright © 2017 MotaWolley. All rights reserved.
//

#include "tree.hpp"

typedef function<bool(pair<string, int>, pair<string, int>)> Comparator;
Comparator compFunctor = [](pair<string, int> elem1 ,pair<string, int> elem2){
    return elem1.second > elem2.second;
};

Tree :: Tree() {
    layers = 1;
};
Tree :: ~Tree(){
    delete root;
}

void Tree :: printTree(){
    cout << "start printing a tree...";
    queue<TreeNode*> currentLayer;
    currentLayer.push(root);
    for (int i=1; i <= layers; i++){
        cout << "######\n";
        int nodesNumber = currentLayer.size(); //implicit data type of size() is long
        for (int j=1; j<=nodesNumber; j++){
            TreeNode* currentNode = currentLayer.front();
            currentNode -> printNode();
            if (currentNode -> isLeaf){
                for (int w=0; w<=currentNode->n; w++){
                    currentNode -> records[w] -> printRecord();
                    cout << "--\n";
                }
            }
            currentLayer.pop();
            for (int k=0; k<=currentNode->n; k++){
                currentLayer.push(currentNode->childs[k]);
            }
        }
    }
}

void Tree :: verify(){
    bool v = true;
    int tempK;
    //first to verify all the leaves
    TreeNode* currentNode = firstLeaf;
    while (currentNode -> childs[0] != NULL) {
        if (not currentNode -> verifyNode()){
            v = false;
        }
        tempK = currentNode -> keys[currentNode->n];
        currentNode = currentNode -> childs[0];
        if (tempK >= currentNode->keys[0]){
            v = false;
        }
    }
    if (not currentNode -> verifyNode()){
        v = false;
    }
    //then check if all the parent node are the largest value of the child node
    if (not root -> verifyChild()){
        v = false;
    }
    if (v){
        cout << "verified :D \n";
    }
    else{
        cout << "not verified :(\n";
    }
    
}

void Tree :: printLeaves(){
    cout << "printing leaves...\n";
    TreeNode* currentNode = firstLeaf;
    while (currentNode -> childs[0] != NULL) {
        currentNode -> printNode();
        currentNode = currentNode -> childs[0];
    }
    currentNode -> printNode();
}

void Tree :: popularCourses (int k){ //the the k most popular courses
    map<string, int> m;
    TreeNode* currentNode = firstLeaf;
    while (currentNode -> childs[0] != NULL) {
        for (int i=0; i<=currentNode->n; i++){
            for (courseRecord* r : *(currentNode -> records[i] -> courseList)){
                if (m.count(r->courseName) == 0){
                    m.insert(make_pair(r->courseName, 1));
                }
                else{
                    m[r->courseName] += 1;
                }
            }
        }
        currentNode = currentNode -> childs[0];
    }
    for (int i=0; i<=currentNode->n; i++){
        for (courseRecord* r : *(currentNode -> records[i] -> courseList)){
            if (m.count(r->courseName) == 0){
                m.insert(make_pair(r->courseName, 1));
            }
            else{
                m[r->courseName] += 1;
            }
        }
    }
    set<pair<string, int>, Comparator> mSet(m.begin(), m.end(), compFunctor);
    int counter = 1;
    for (pair<string, int> element : mSet){
        if (counter <= k){
            cout << element.first << " taken by " << element.second <<" students"<< endl;
        }
        else{
            break;
        }
        counter ++;
    }
    //m has all the records
}

void Tree :: insert(int k, string courseID, string courseName, string grade){
    TreeNode::searchReturn s = root -> search(k);
    courseRecord *r = new courseRecord;
    *r = {courseID, courseName, grade};
    if (s.x != NULL){ //key already in the tree
        s.x -> records[s.i] -> courseList -> push_back(r);
    }
    else { //when the key is not in the tree
        int i = root -> n;
        if (i == 3){
            layers += 1;
            TreeNode *s = new TreeNode();
            s -> isLeaf = false;
            s -> n = 0;
            s -> childs[0] = root;
            s -> keys[0] = root -> keys[3];
            root = s;
            root -> split(0);
            if (k > root -> keys[1]){
                root -> insertMax(k);
            }
            else{
                root -> insertNonFull(k);
            }
        }
        else{
            if (k > root -> keys[i]){
                root -> insertMax(k);
            }
            else{
                root -> insertNonFull(k);
            }
        }
        s = root -> search(k); //after inserting, searching position
        studentRecord *nrecord = new studentRecord();
        s.x -> records[s.i] = nrecord;
        nrecord -> courseList -> push_back(r);
    }
}

void Tree :: find(int k){ //according to student ID k, print information about the student
    TreeNode::searchReturn s = root -> search(k);
    cout << "printing information about student "<< k << endl;
    if (s.x == NULL){
        cout << "this student is not in the database\n";
    }
    else{
        studentRecord *r = s.x -> records[s.i];
        r -> printRecord();
    }
}

void Tree :: del(int k, string cID){
    TreeNode::searchReturn s = root -> search(k);
    if (s.x == NULL){
        cout << "this student is not in the database\n";
    }
    else{
        studentRecord *r = s.x -> records[s.i];
        r -> deleteCourse(cID);
    }
}

void Tree :: gpaOne (int k){
    TreeNode::searchReturn s = root -> search(k);
    if (s.x == NULL){
        cout << "this student is not in the database\n";
    }
    else{
        studentRecord *r = s.x -> records[s.i];
        cout << "gpa of student "<< k << " is "<< r->getGPA()<< endl;
    }
}

void Tree :: gpaAverage (int a, int b){
    if (b < a){
        int c = b;
        b = a;
        a = c;
    }
    TreeNode::searchReturn sa = root -> search(a);
    TreeNode::searchReturn sb = root -> search(b);
    int studentNumber = 0;
    float totalGPA = 0;
    if (sa.x == NULL or sb.x == NULL){
        cout << "range invalid, enter existing student IDs\n";
    }
    else{
        TreeNode *currentX = sa.x;
        int currentI = sa.i;
        while (currentX != sb.x){ //print everything until the tree node b
            for (currentI; currentI <= currentX -> n; currentI ++){
                studentNumber += 1;
                totalGPA += currentX -> records[currentI] -> getGPA();
            }
            currentX = currentX -> childs[0];
            currentI = 0;
        }
        //printing students in treenode that contains b
        if (sa.x == sb.x){
            for (int i = sa.i; i <= sb.i; i++){
                studentNumber += 1;
                totalGPA += currentX -> records[i] -> getGPA();
            }
        }
        else{
            for (int i = 0; i <= sb.i; i++){
                studentNumber += 1;
                totalGPA += currentX -> records[i] -> getGPA();
            }
        }
        float averageGPA = totalGPA / studentNumber;
        cout << "the average gpa in the range from " << a <<" to "<<b<<" is "<<averageGPA<<endl;
    }
}
void Tree :: findRange(int a, int b){ //under the assumption that a, b are already in the system
    if (b < a){
        int c = b;
        b = a;
        a = c;
    }
    TreeNode::searchReturn sa = root -> search(a);
    TreeNode::searchReturn sb = root -> search(b);
    if (sa.x == NULL or sb.x == NULL){
        cout << "range invalid, enter existing student IDs\n";
    }
    else{
        TreeNode *currentX = sa.x;
        int currentI = sa.i;
        while (currentX != sb.x){ //print everything until the tree node b
            for (currentI; currentI <= currentX -> n; currentI ++){
                cout << "printing information about student " << currentX -> keys[currentI] << endl;
                currentX -> records[currentI] -> printRecord();
                cout << "\n";
            }
            currentX = currentX -> childs[0];
            currentI = 0;
        }
        //printing students in treenode that contains b
        if (sa.x == sb.x){
            for (int i = sa.i; i <= sb.i; i++){
                cout << "printing information about student " << currentX -> keys[i] << endl;
                currentX -> records[i] -> printRecord();
            }
        }
        else{
            for (int i = 0; i <= sb.i; i++){
                cout << "printing information about student " << currentX -> keys[i] << endl;
                currentX -> records[i] -> printRecord();
            }
        }
    }
}

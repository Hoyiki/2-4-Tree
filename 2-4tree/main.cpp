#include <iostream>
#include <queue>
#include <list>
#include <algorithm>
#include <sstream>
#include <string>
#include <map>
#include <functional>
#include <set>
#include <fstream>
#include <new>
using namespace std;

string grades[13] = {"A", "B", "C", "D","A+", "B+", "C+", "D+", "A-", "B-", "C-", "D-","NP"};
float points[13]= {4, 3, 2, 1, 4.3, 3.3, 2.3, 1.3, 3.7, 2.7, 1.7, 0.7, 0};

struct courseRecord{
    string courseID;
    string courseName;
    string grade;
};

typedef function<bool(pair<string, int>, pair<string, int>)> Comparator;
Comparator compFunctor = [](pair<string, int> elem1 ,pair<string, int> elem2){
    return elem1.second > elem2.second;
};

class studentRecord{
public:
    list<courseRecord*> *courseList = new list<courseRecord*>(); //pointer to a list of pointers
    ~studentRecord (){
        list<courseRecord*>::iterator it;
        if (courseList -> size() > 0){
            it = courseList -> begin();
            for (int i=0; i<courseList->size();i++){
                delete *it;
                ++it;
            }
        }
        delete courseList;
    }
    void printRecord(){
        for (courseRecord* r : *courseList){
            cout << " " << r->courseID << " " << r->courseName << " " << r->grade << endl;
        }
    }
    float getGPA(){
        int courseNumber = courseList -> size();
        float gpa = 0;
        for (courseRecord* r : *courseList){
            string grade = r -> grade;
            int index = distance(grades, find(grades, grades + 13, grade));
            if (index < 0 or index > 12){
                index = 12;
            }
            gpa += points[index] / courseNumber;
        }
        return gpa;
    }
    void deleteCourse(string cID){
        list<courseRecord*>::iterator it;
        it = courseList -> begin();
        if ((*it) != NULL){
            int courseNumber = courseList -> size();
            int cc = 1;
            while ((*it)->courseID != cID and cc < courseNumber){
                ++it;
                cc ++;
            }
            if ((*it)->courseID == cID){
                courseList -> erase(it);
                cout << "deleted!\n" << endl;
            }
            else{
                cout << "this course is not in the record." <<endl;
            }
        }
        else{
            cout << "this course is not in the record." <<endl;
        }
    }
};

//check if the class record already in
//check if grade valid


class TreeNode
{
public:
    
    int n; //number of keys -1!!!
    int keys[4] = {};
    TreeNode *childs[4] = {}; //the fourth one if for leaves only, maybe not necessary, can just use the first of the leaves
    bool isLeaf;
    studentRecord *records[4] = {}; //this one only used if the treenode is a leaf
    
    //constructor
    TreeNode(){
        n = -1;
    }
    //destructor
    ~TreeNode(){
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
    void split(int i){
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
    
    void insertNonFull (int k){
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
    
    void insertMax(int k){
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
    
    struct searchReturn{
        TreeNode* x;
        int i;
    };
    
    //only apply this function when the treenode is a root!!!
    searchReturn search (int k){
        searchReturn notExist = {NULL, -1};
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
            searchReturn find = {currentNode, i};
            return find;
        }
        if (isLeaf){
            return notExist;
        }
        else{
            return (childs[i] -> search(k));
        }
    }
    
    void printNode(){
        cout << "--\n";
        for (int i = 0; i <= n; i++){
            cout << keys[i] << endl;
        }
        cout << "--\n";
    }
    
    bool verifyNode(){
        bool vn = true;
        for (int i=0; i<=n-1; i++){
            if (keys[i] >= keys[i+1]){
                vn = false;
            }
        }
        return vn;
    }
    bool verifyChild(){
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
    
private:
    
};

class Tree
{
public:
    Tree() {
        layers = 1;
    };
    ~Tree(){
        delete root;
    }
    TreeNode *root;
    TreeNode *firstLeaf;
    int layers;
    
    void printTree(){
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
    
    void verify(){
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
    
    void printLeaves(){
        cout << "printing leaves...\n";
        TreeNode* currentNode = firstLeaf;
        while (currentNode -> childs[0] != NULL) {
            currentNode -> printNode();
            currentNode = currentNode -> childs[0];
        }
        currentNode -> printNode();
    }
    
    void popularCourses (int k){ //the the k most popular courses
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
    
    void insert(int k, string courseID, string courseName, string grade){
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
    
    void find(int k){ //according to student ID k, print information about the student
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
    
    void del(int k, string cID){
        TreeNode::searchReturn s = root -> search(k);
        if (s.x == NULL){
            cout << "this student is not in the database\n";
        }
        else{
            studentRecord *r = s.x -> records[s.i];
            r -> deleteCourse(cID);
        }
    }
    
    void gpaOne (int k){
        TreeNode::searchReturn s = root -> search(k);
        if (s.x == NULL){
            cout << "this student is not in the database\n";
        }
        else{
            studentRecord *r = s.x -> records[s.i];
            cout << "gpa of student "<< k << " is "<< r->getGPA()<< endl;
        }
    }
    
    void gpaAverage (int a, int b){
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
    void findRange(int a, int b){ //under the assumption that a, b are already in the system
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
};

int main(){
    string command;
    Tree *tft = new Tree();
    TreeNode *root = new TreeNode();
    root -> isLeaf = true;
    tft -> root = root;
    tft -> firstLeaf = root;
    string buf;
    vector<string> tokens;
    int a;
    int b;
    int c;
    int d;
    string filename;
    while (1) {
        getline(cin, command);
        stringstream ss(command);
        while (ss >> buf){
            tokens.push_back(buf);
        }
        if (command == "init"){
            delete tft;
            tft = new Tree();
            TreeNode *root = new TreeNode();
            root -> isLeaf = true;
            tft -> root = root;
            tft -> firstLeaf = root;
        }
        if (tokens[0] == "ins"){
            if (tokens.size() == 5){
                try {
                    c = stoi(tokens[1]);
                    tft -> insert(c, tokens[2], tokens[3], tokens[4]);
                }
                catch(exception aiya){
                    cout << "not valid input\n";
                }
            }
        }
        //this is for debugging purpose mainly
        if (tokens[0] == "print"){
            tft -> printTree();
        }
        if (tokens[0] == "find"){
            if(tokens.size() == 2){
                try {
                    d = stoi(tokens[1]);
                    tft -> find(d);
                }
                catch(exception aiya){
                    cout << "not valid input\n";
                }
            }
        }
        if (tokens[0] == "range"){
            if(tokens.size() == 3){
                try {
                    a = stoi(tokens[1]);
                    b = stoi(tokens[2]);
                    tft -> findRange(a, b);
                }
                catch(exception aiya){
                    cout << "not valid input\n";
                }
            }
        }
        if (tokens[0] == "gpa"){
            if(tokens.size() == 2){
                try {
                    a = stoi(tokens[1]);
                    tft -> gpaOne(a);
                }
                catch(exception aiya){
                    cout << "not valid input\n";
                }
            }
            if(tokens.size() == 3){
                try {
                    a = stoi(tokens[1]);
                    b = stoi(tokens[2]);
                    tft -> gpaAverage(a, b);
                }
                catch(exception aiya){
                    cout << "not valid input\n";
                }
            }
        }
        if (tokens[0] == "del"){
            if(tokens.size() == 3){
                try {
                    b = stoi(tokens[1]);
                    tft -> del(b, tokens[2]);
                }
                catch(exception aiya){
                    cout << "not valid input\n";
                }
            }
        }
        
        if (tokens[0] == "top"){
            if(tokens.size() == 2){
                try {
                    b = stoi(tokens[1]);
                    tft -> popularCourses(b);
                }
                catch(exception aiya){
                    cout << "not valid input\n";
                }
            }
        }
        
        if (tokens[0] == "verify"){
            tft -> verify();
        }
        
        if (tokens[0] == "exit"){
            delete tft;
            return(0);
        }
        
        if (tokens[0] == "load"){
            filename = tokens[1];
            ifstream input(filename);
            string line;
            while (getline(input, line)){
                command = line;
                stringstream ss(command);
                while (ss >> buf){
                    tokens.push_back(buf);
                }
                if (command == "init"){
                    tft = new Tree();
                    TreeNode *root = new TreeNode();
                    root -> isLeaf = true;
                    tft -> root = root;
                    tft -> firstLeaf = root;
                }
                if (tokens[0] == "ins"){
                    if (tokens.size() == 5){
                        try {
                            c = stoi(tokens[1]);
                            tft -> insert(c, tokens[2], tokens[3], tokens[4]);
                        }
                        catch(exception aiya){
                            cout << "not valid input\n";
                        }
                    }
                }
                if (tokens[0] == "find"){
                    if(tokens.size() == 2){
                        try {
                            d = stoi(tokens[1]);
                            tft -> find(d);
                        }
                        catch(exception aiya){
                            cout << "not valid input\n";
                        }
                    }
                }
                if (tokens[0] == "range"){
                    if(tokens.size() == 3){
                        try {
                            a = stoi(tokens[1]);
                            b = stoi(tokens[2]);
                            tft -> findRange(a, b);
                        }
                        catch(exception aiya){
                            cout << "not valid input\n";
                        }
                    }
                }
                if (tokens[0] == "gpa"){
                    if(tokens.size() == 2){
                        try {
                            a = stoi(tokens[1]);
                            tft -> gpaOne(a);
                        }
                        catch(exception aiya){
                            cout << "not valid input\n";
                        }
                    }
                    if(tokens.size() == 3){
                        try {
                            a = stoi(tokens[1]);
                            b = stoi(tokens[2]);
                            tft -> gpaAverage(a, b);
                        }
                        catch(exception aiya){
                            cout << "not valid input\n";
                        }
                    }
                }
                if (tokens[0] == "del"){
                    if(tokens.size() == 3){
                        try {
                            b = stoi(tokens[1]);
                            tft -> del(b, tokens[2]);
                        }
                        catch(exception aiya){
                            cout << "not valid input\n";
                        }
                    }
                }
                
                if (tokens[0] == "top"){
                    if(tokens.size() == 2){
                        try {
                            b = stoi(tokens[1]);
                            tft -> popularCourses(b);
                        }
                        catch(exception aiya){
                            cout << "not valid input\n";
                        }
                    }
                }
                
                if (tokens[0] == "verify"){
                    tft -> verify();
                }
                tokens.clear();
            }
        }
        tokens.clear();
    }
}
    
    
//testing part!!!!!!!!!!!!!
    
    
//    Tree *tft = new Tree();
//    TreeNode *root = new TreeNode();
//    root -> isLeaf = true;
//    tft -> root = root;
//    tft -> firstLeaf = root;
//    tft -> insert(30, "cs111", "computerOh","A+");
//    tft -> insert(30, "cs110", "computerAh","A+");
//    tft -> insert(40, "cs111", "computerOh","A+");
//    tft -> insert(50, "cs1", "computerAh","A+");
//    tft -> insert(90, "cs121", "computerOh","A+");
//    tft -> insert(31, "cs14", "computerAh","A+"); //something went wrong here about the records!!!
//    tft -> insert(41, "cs1", "computerOh","C");
////    tft -> insert(51, "cs10", "computerAh","B");
////    tft -> insert(31, "cs1", "computerOh","A+");
////    tft -> insert(31, "cs10", "computerAh","A+");
////    tft -> insert(41, "cs3", "computerOh","A+");
////    tft -> insert(51, "cs60", "computerAh","B");
//    tft -> del(41, "cs1");
//    tft -> del(41, "cs1");
//    tft -> printTree();
//    tft -> find(90);
////////    tft -> printLeaves();
//////    tft -> findRange(90, 90);
//    tft -> gpaOne(41);


//if the student doesn't take any course, the gpa is zero / maybe should just ignore the student??
//no load/exit in load
//if format incorrect, just ignore it





















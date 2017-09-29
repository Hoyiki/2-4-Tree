#include <iostream>
#include <queue>
#include <list>
using namespace std;

struct courseRecord{
    char *courseID;
    char *courseName;
    char *grade;
};

class studentRecord{
public:
    list<courseRecord*> courseList;
    void printRecord(){
        for (courseRecord* r : courseList){
            cout << r->courseID << r->courseName << r->grade << endl;
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
            while (not currentNode -> isLeaf){
                currentNode = currentNode -> childs[currentNode -> n];
            }
            searchReturn find = {this, i};
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
    
private:
    
};


class Tree
{
public:
    Tree() {
        layers = 1;
    };
    TreeNode *root;
    TreeNode *firstLeaf;
    int layers;
    
    void printTree(){
        cout << "start printing a tree...";
        queue<TreeNode*> currentLayer;
        currentLayer.push(root);
        for (int i=1; i <= layers; i++){
            cout << "######\n";
            long nodesNumber = currentLayer.size(); //implicit data type of size() is long
            for (int j=1; j<=nodesNumber; j++){
                TreeNode* currentNode = currentLayer.front();
                currentNode -> printNode();
                if (currentNode -> isLeaf){
                    
                }
                currentLayer.pop();
                for (int k=0; k<=currentNode->n; k++){
                    currentLayer.push(currentNode->childs[k]);
                }
            }
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
    
    void insert(int k, char* courseID, char* courseName, char* grade){
        TreeNode::searchReturn s = root -> search(k);
        courseRecord *r = new courseRecord;
        *r = {courseID, courseName, grade};
        if (s.x != NULL){ //key already in the tree
            s.x -> records[s.i] -> courseList.push_back(r);
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
                if (k > root -> keys[0]){
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
            nrecord -> courseList.push_back(r);
        }
    }
};

int main(){
    Tree *tft = new Tree();
    TreeNode *root = new TreeNode();
    root -> isLeaf = true;
    tft -> root = root;
    tft -> firstLeaf = root;
//    tft -> insert(30);


    tft -> printTree();
    tft -> printLeaves();
}
























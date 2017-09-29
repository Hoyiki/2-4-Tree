#include <iostream>
#include <queue>
using namespace std;


class TreeNode
{
public:
    
    int n; //number of keys -1!!!
    int keys[4] = {};
    TreeNode *childs[4] = {};
    bool isLeaf;
    
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
        }
        if (not y -> isLeaf){ //also need a function later on to move the student information here!!!!!
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
    
    //only apply this function when the treenode is a root!!!
    bool search (int k){
        if (n == -1){ //this is an empty tree
            return false;
        }
        if (k>keys[n]){
            return false;
        }
        
        int i = 0;
        while (i<=n-1 && k>keys[i]){
            i = i + 1;
        }

        if (i<=n && k==keys[i]){
            return true;
        }
        if (isLeaf){
            return false;
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
    
    void printTree(){ ///////!!!!!!!
        cout << "start printing a tree...";
        queue<TreeNode*> currentLayer;
        currentLayer.push(root);
        for (int i=1; i <= layers; i++){
            cout << "######\n";
            long nodesNumber = currentLayer.size(); //implicit data type of size() is long
            for (int j=1; j<=nodesNumber; j++){
                TreeNode* currentNode = currentLayer.front();
                currentNode -> printNode();
                currentLayer.pop();
                for (int k=0; k<=currentNode->n; k++){
                    currentLayer.push(currentNode->childs[k]);
                }
            }
        }
        
    }
    
    void insert(int k){
        if (not root -> search(k)){
            
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
                    //cout<<root->n <<endl;

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
        }}
};

int main(){
    Tree *tft = new Tree();
    TreeNode *root = new TreeNode();
    root -> isLeaf = true;
    tft -> root = root;
    tft -> insert(30);
    tft -> insert(20);
    tft -> insert(50);
    tft -> insert(40);
    tft -> insert(10);
    tft -> insert(5);
    tft -> insert(25);
    tft -> insert(55);
    tft -> insert(45);
    tft -> insert(15);
    tft -> insert(6);
    tft -> insert(26);
    tft -> insert(-55);
    tft -> insert(450);
    tft -> insert(150);
    tft -> insert(60);

    tft -> printTree();
}
























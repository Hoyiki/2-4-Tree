#include <algorithm>
#include <sstream>
#include <functional>
#include <fstream>
#include <new>
#include "studentrecord.hpp"
#include "treenode.hpp"
#include "tree.hpp"
using namespace std;


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

//if the student doesn't take any course, the gpa is zero / maybe should just ignore the student??
//no load/exit in load
//if format incorrect, just ignore it
//invalid format grade, 0





















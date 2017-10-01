//
//  studentrecord.cpp
//  2-4tree
//
//  Created by 黄梓原 on 01/10/2017.
//  Copyright © 2017 MotaWolley. All rights reserved.
//

#include "studentrecord.hpp"
using namespace std;

string grades[13] = {"A", "B", "C", "D","A+", "B+", "C+", "D+", "A-", "B-", "C-", "D-","NP"};
float points[13]= {4, 3, 2, 1, 4.3, 3.3, 2.3, 1.3, 3.7, 2.7, 1.7, 0.7, 0};

studentRecord :: studentRecord(){
    courseList = new list<courseRecord*>();
}

studentRecord :: ~studentRecord (){
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

void studentRecord::printRecord(){
    for (courseRecord* r : *courseList){
        cout << " " << r->courseID << " " << r->courseName << " " << r->grade << endl;
    }
}

float studentRecord:: getGPA(){
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

void studentRecord::deleteCourse(string cID){
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


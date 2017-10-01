//
//  studentrecord.hpp
//  2-4tree
//
//  Created by 黄梓原 on 01/10/2017.
//  Copyright © 2017 MotaWolley. All rights reserved.
//

#ifndef studentrecord_hpp
#define studentrecord_hpp

#include <iostream>
#include <stdio.h>
#include <list>
#include <string>
using namespace std;

struct courseRecord{
    string courseID;
    string courseName;
    string grade;
};

class studentRecord{
public:
    list<courseRecord*> *courseList; //pointer to a list of pointers
    ~studentRecord ();
    studentRecord();
    void printRecord();
    float getGPA();
    void deleteCourse(string cID);
};
#endif /* studentrecord_hpp */

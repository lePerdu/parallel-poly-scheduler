#include <iostream>
#include "ParseJson.cpp"
#include <string>

/* File used to test the ParseJSON class */

int main(int argc, char* argv[]){
    std::vector <Course> courseList;

    std::string fileName = "cs_major_courses.json";
    ParseJson parseMajor(fileName);
    courseList = parseMajor.get_course_list();

    for (int i = 0; i < courseList.size(); i++){
        courseList.at(i).print_course();
    }
    return 0;
}
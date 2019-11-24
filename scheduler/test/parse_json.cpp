#include "parse_json.hpp"

#include <iostream>
#include <string>

/* File used to test the ParseJSON class */

static const char* FILE_NAME = "data/cs_major_courses.json";

int main(int argc, char* argv[]) {
    std::vector<Course> courseList;

    ParseJson parseMajor(FILE_NAME);
    courseList = parseMajor.get_course_list();

    for (std::size_t i = 0; i < courseList.size(); i++) {
        courseList.at(i).print_course();
    }
    return 0;
}

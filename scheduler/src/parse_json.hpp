#ifndef PARSEJSON_HPP_
#define PARSEJSON_HPP_

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include "course.cpp"
#include <vector>
#include <fstream>


/*  Parses a JSON file into a vector of courses */
class ParseJson{
public:
    ParseJson(std::string& fileName);
    std::vector<Course>& get_course_list();

private:
    std::vector<Course> courseList;
};

#endif // PARSEJSON_HPP_
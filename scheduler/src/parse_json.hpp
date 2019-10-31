#ifndef PARSE_JSON_HPP_
#define PARSE_JSON_HPP_

#include "course.cpp"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

/* Parses a JSON file into a vector of courses */
class ParseJson {
public:
    ParseJson(const std::string& fileName);
    const std::vector<Course>& get_course_list() const;

private:
    std::vector<Course> course_list;
};

#endif // PARSE_JSON_HPP_

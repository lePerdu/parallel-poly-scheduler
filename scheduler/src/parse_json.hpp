#ifndef PARSE_JSON_HPP_
#define PARSE_JSON_HPP_

#include "course.hpp"
#include "student.hpp"

#include <vector>

std::vector<Course> parse_courses_json(const char* filename);

std::vector<Student> parse_student_json(
        const char* filename, const std::vector<Course>& all_courses);

#endif // PARSE_JSON_HPP_

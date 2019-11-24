#ifndef SETUP_HPP_
#define SETUP_HPP_

#include "course.hpp"
#include "parse_json.hpp"
#include "student.hpp"

#include <iostream>
#include <mpi.h>
#include <string>
#include <vector>

constexpr std::size_t TOTALSTUDENTS = 10;

extern const char* FILE_NAME;

/*
 * Parses the courses from the list of courses that a
 * CS major student must take && Generates a list of
 * students with the courses that they have already taken
 */

void setup(
        std::vector<Course>& available_courses,
        std::vector<Course::Ref>& course_pointers);

void extract_courses(
        std::vector<Course>& available_courses,
        std::vector<Course::Ref>& course_pointers);

#endif // SETUP_HPP_

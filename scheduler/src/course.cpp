
#include "course.hpp"

#include <algorithm>
#include <iostream>

Course::Course(std::string name, std::uint8_t credits) :
    name(name), credits(credits) {}

const std::string& Course::get_name() const {
    return name;
}

void Course::add_prereq(const Course* prereq) {
    prereqs.push_back(prereq);
}

bool Course::has_prereq(const Course* prereq) const {
    return std::find(prereqs.begin(), prereqs.end(), prereq) != prereqs.end();
}

void Course::print_course() {
    std::cout << "Course Title: [" << name.c_str() << "]"
              << "\t"
              << "Credits: [" << credits << "]" << std::endl;
}

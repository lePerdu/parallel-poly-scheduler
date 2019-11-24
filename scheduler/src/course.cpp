
#include "course.hpp"

#include <algorithm>
#include <iostream>

Course::Course(std::string name, std::uint8_t credits) :
    name(name), credits(credits) {}

const std::string& Course::get_name() const {
    return name;
}

std::uint8_t Course::get_credits() const {
    return credits;
}

void Course::add_prereq(Course::Ref prereq) {
    prereqs.push_back(prereq);
}

bool Course::has_prereq(Course::Ref prereq) const {
    return std::find(prereqs.begin(), prereqs.end(), prereq) != prereqs.end();
}

void Course::print_course() const {
    std::cout << "Course Title: [" << name << "]"
              << "\t"
              << "Credits: [" << static_cast<unsigned>(credits) << "]"
              << std::endl;
}


#include <algorithm>

#include "course.hpp"

Course::Course(const std::string &name) : name(name) {
}

const std::string &Course::get_name() const {
    return name;
}

void Course::add_prereq(const Course *prereq) {
    prereqs.push_back(prereq);
}

bool Course::has_prereq(const Course *prereq) const {
    return std::find(prereqs.begin(), prereqs.end(), prereq) != prereqs.end();
}

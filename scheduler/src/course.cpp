
#include "course.hpp"
#include <algorithm>

Course::Course(std::string name, std::uint8_t credits) :
    name(name), credits(credits) {}

const std::string& Course::get_name() const {
    return name;
}

const uint8_t Course::get_credits() const {
    return credits;
}

void Course::add_prereq(const Course* prereq) {
    prereqs.push_back(prereq);
}

bool Course::has_prereq(const Course* prereq) const {
    return std::find(prereqs.begin(), prereqs.end(), prereq) != prereqs.end();
}

void Course::print_course(){
    printf("Course Title: [%s]\tCredits: [%u]\n", Course::name.c_str(), Course::credits);
}

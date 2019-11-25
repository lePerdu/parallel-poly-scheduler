
#include "course.hpp"

#include <algorithm>
#include <iostream>

Course::Course(unsigned id, std::string name, std::uint8_t credits) :
    id(id), name(name), credits(credits) {}

unsigned Course::get_id() const {
    return id;
}

const std::string& Course::get_name() const {
    return name;
}

std::uint8_t Course::get_credits() const {
    return credits;
}

void Course::print() const {
    std::cout << "Course Title: [" << name << "]"
              << "\t"
              << "Credits: [" << static_cast<unsigned>(credits) << "]"
              << std::endl;
}

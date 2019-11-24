#include "section.hpp"

#include <iostream>

Section::Section(Course::Ref course, ClassLayout layout) :
    course(course), layout(layout) {}

bool Section::overlaps(const Section& other) const {
    return layout.overlaps(other.layout);
}

void Section::print() const {
    std::cout << course->get_name() << ':' << std::endl;
    for (auto& time : layout.get_times()) {
        std::cout << '\t';
        time.print();
        // ClassTime::print() outputs a new line after it
    }
}

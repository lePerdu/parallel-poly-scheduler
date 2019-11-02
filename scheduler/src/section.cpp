#include "section.hpp"

Section::Section(const Course* course, ClassLayout layout) :
    course(course), layout(layout) {}

bool Section::overlaps(const Section& other) const {
    return layout.overlaps(other.layout);
}

#ifndef SECTION_HPP_
#define SECTION_HPP_

#include <vector>

#include "class_time.hpp"
#include "course.hpp"

class Section {
public:
    Section(const Course *course, int number, ClassLayout layout);

    /**
     * Test for equality using the course and the number.
     * The class layout is not checked, for better performance.
     */
    bool operator==(const Section &other) const;
    bool operator!=(const Section &other) const;

private:
    const Course *course;
    int number;
    ClassLayout layout;
};

#endif // SECTION_HPP_

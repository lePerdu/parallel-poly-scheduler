#ifndef SECTION_HPP_
#define SECTION_HPP_

#include "class_time.hpp"
#include "course.hpp"

#include <vector>

struct Section {
    /**
     * TODO Use some form of ID instead of pointers for easier transport between
     * MPI nodes?
     */
    const Course* course;
    ClassLayout layout;

    Section(const Course* course, ClassLayout layout);

    bool overlaps(const Section& other) const;

    // TODO Remove these?

    // bool operator==(const Section& other) const;
    // bool operator!=(const Section& other) const;

    /**
     * Prints a section in a somewhat-human-readable format.
     */
    void print() const;
};

#endif // SECTION_HPP_

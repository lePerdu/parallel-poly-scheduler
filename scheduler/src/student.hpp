#ifndef STUDENT_HPP_
#define STUDENT_HPP_

#include "course.hpp"

#include <vector>

/*
 * Generates a list of students based upon the inputted number of students and
 * the list of available courses. Each individual students taken course list
 * is randomly generated using the list of available courses but does not
 * currently check if the student has met the prereqs required for a random
 * course be added into their completed list. This works by having a randomly
 * selected number of max credits the student had already achieved, and from
 * there randomly choosing a class based on their placement in the list of
 * available courses. The credits from the chosen course is added to their total
 * and if it is greater than or equal to the random number of max credits the
 * student should receive, consider the student's course list complete.
 */

/**
 * Total number of credits that a CS undergrad must get to get a degree.
 *
 * TODO Make this not a constant so that we can, e.g. use the same functions for
 * grad students.
 */
constexpr size_t TOTAL_CREDITS_POSSIBLE = 120;

struct Student {
    Student(std::uint16_t student_id,
            std::vector<const Course*> taken_courses) :
        student_id(student_id), taken_courses(taken_courses) {}

    /**
     * Added for more clarity in the print function.
     *
     * May be removed later.
     */
    std::uint16_t student_id;
    std::vector<const Course*> taken_courses;
};

std::vector<Student> generate_random_students(
        std::uint16_t total_students,
        const std::vector<const Course*>& available_courses);

void print_all_students_courses(const std::vector<Student>& students_list);

#endif // STUDENT_HPP_

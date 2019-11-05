#include "student.hpp"

#include <algorithm>
#include <iostream>
#include <random>

/**
 * Generates a student with random set of taken courses
 */
static Student generate_taken_courses(
        std::uint16_t student_id,
        const std::vector<const Course*>& available_courses) {
    /* Random Number Generator that generates a better distribution than srand
     */
    std::random_device dev;
    std::mt19937 rng(dev());

    // Pick a random value between (0 and the total number of credits possible
    // to achieve a degree)
    std::uniform_int_distribution<int> randomly_assigned_max_credits(
            0, TOTAL_CREDITS_POSSIBLE);
    int max_assigned_credits = randomly_assigned_max_credits(rng);

    // Pick a random value between (0 and the total number of available courses)
    // -- the picked number will correlate to the chosen course
    std::uniform_int_distribution<int> randomly_assigned_course(
            0, available_courses.size() - 1);

    // Keep generating courses until the number of taken credits is <= max that
    // was assigned Does not currently look at if the prereqs were met before
    // assigning
    //
    // TODO Shuffle (a copy of) the vector and take the first N instead? The
    // current code will loop forever, or at least for potentially a long time
    // if the number of assigned credits is close to or more than all available
    // courses.

    int taken_credits = 0;

    // Most courses have 3 credits, so max_assigned_credits / 3 is a good
    // estimate for how large the vector will get (it can still grow if it
    // needs to)
    std::vector<const Course*> taken_courses;
    taken_courses.reserve(max_assigned_credits / 3);

    while (taken_credits <= max_assigned_credits) {
        auto chosen_course =
                available_courses.at(randomly_assigned_course(rng));

        // The class was not previously assigned .. add it to the student's
        // courses
        if (std::find(
                    taken_courses.begin(),
                    taken_courses.end(),
                    chosen_course) == taken_courses.end()) {
            taken_credits += chosen_course->get_credits();
            taken_courses.push_back(chosen_course);
        }
    }

    return {student_id, taken_courses};
}

std::vector<Student> generate_random_students(
        const std::uint16_t total_students,
        const std::vector<const Course*>& available_courses) {
    // Create a list of all students based on the inputted number of students
    std::vector<Student> students_list;
    for (std::uint16_t i = 0; i < total_students; i++) {
        students_list.push_back(generate_taken_courses(i, available_courses));
    }

    return students_list;
}

// Prints all students along with their generated courses
void print_all_students_courses(const std::vector<Student>& students_list) {
    for (std::uint16_t i = 0; i < students_list.size(); i++) {
        const auto& printing_student = students_list.at(i);
        std::cout << "Student ID: [" << printing_student.student_id << "]"
                  << std::endl;
        for (auto& course : printing_student.taken_courses) {
            course->print_course();
        }
        std::cout << std::endl;
    }
}

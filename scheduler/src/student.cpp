#include "student.hpp"

// #include <algorithm>
#include <iostream>
#include <unordered_set>

std::unordered_map<Course::Ref, std::uint8_t> required_course_counts(
        std::size_t class_capacity, const std::vector<Student>& students) {
    // Number of students wanting each course
    std::unordered_multiset<Course::Ref> course_counts;

    for (auto& student : students) {
        for (auto course : student.wanted_courses) {
            course_counts.insert(course);
        }
    }

    std::unordered_map<Course::Ref, std::uint8_t> section_counts;
    section_counts.reserve(course_counts.size());
    for (auto course : course_counts) {
        const auto count = course_counts.count(course);
        // ceil(count / capacity)
        const std::uint8_t full_sects =
                count / class_capacity + ((count % class_capacity) ? 1 : 0);
        section_counts.insert({course, full_sects});
    }

    return section_counts;
}

// Prints all students along with their generated courses
void print_all_students_courses(const std::vector<Student>& students_list) {
    for (std::size_t i = 0; i < students_list.size(); i++) {
        const auto& printing_student = students_list.at(i);
        std::cout << "Student ID: [" << printing_student.id << "]" << std::endl
                  << "Taken courses:" << std::endl;
        for (auto& course : printing_student.taken_courses) {
            course->print();
        }

        std::cout << "Wanted courses:" << std::endl;

        for (auto& course : printing_student.wanted_courses) {
            course->print();
        }

        std::cout << std::endl;
    }
}

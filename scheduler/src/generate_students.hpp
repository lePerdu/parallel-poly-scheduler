#ifndef GENERATE_STUDENTS_HPP_
#define GENERATE_STUDENTS_HPP_

#include <iostream>
#include <vector>
#include "course.hpp"
#include <random>


    /*
    *   Generates a list of students based upon the inputted number of students and 
    *   the list of available courses. Each individual students taken course list is randomly 
    *   generated using the list of available courses but does not currently check if 
    *   the student has met the prereqs required for a random course be added into their 
    *   completed list. This works by having a randomly selected number of max credits the 
    *   student had already achieved, and from there randomly choosing a class based
    *   on their placement in the list of available courses. The credits from the
    *   chosen course is added to their total and if it is greater than or equal to the
    *   random number of max credits the student should receive, consider the student's
    *   course list complete.
    */


/* Total number of credits that a CS undergrad must get to get a degree -- Generated Classes cannot go above the max */
constexpr size_t TOTAL_CREDITS_POSSIBLE = 120;

struct Student{
    // Optional Variables that can be removed: 
    // student_id -> Added for more clarity in the print function

    std::uint16_t student_id; 
    std::uint16_t taken_credits;
    std::vector<Course> taken_courses;
 };

class GenerateStudents{
public:
    GenerateStudents(const std::uint16_t total_students, std::vector<Course>& available_courses);
    void print_all_students_courses();
    std::vector<Student>& get_student_list();

private:
    // Generates a list of all students given the inputted total_students
    void create_students();

    // Generate a list of taken courses for a specific student
    void generate_taken_courses(std::vector<Course>& available_courses, Student& created_student);

    // A list of all students that were generated
    std::vector<Student> students_list;
    std::uint16_t total_students;
};

#endif // GENERATE_STUDENTS_HPP_
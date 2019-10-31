#ifndef STUDENT_HPP_
#define STUDENT_HPP_

#include <iostream>
#include <vector>
#include "course.hpp"
#include <random>

class Student{
public:
    Student(const std::uint16_t student_id);
    void add_taken_course(const Course& course);
    std::vector<Course>& get_taken_courses();
    void generate_taken_courses(std::vector<Course>& available_courses);
    void print_taken_courses();

private:
    const std::uint16_t student_id;
    std::vector<Course> taken_student_courses;
    std::uint8_t taken_credits;
    
};

#endif // STUDENT_HPP_
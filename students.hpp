#ifndef STUDENTS_HPP_
#define STUDENTS_HPP_

#include <iostream>
#include <vector>
#include "student.hpp"
#include "course.hpp"
#include "parse_json.hpp"

class Students{
public:
    Students(const std::uint16_t total_students, std::vector<Course>& available_courses);
    void generate_all_students();
    void print_all_students();
    std::vector<Student>& get_all_students();

private:
    const std::uint16_t total_students;
    std::vector<Student> students;
    std::vector<Course> available_courses;
    
};

#endif // STUDENTS_HPP_

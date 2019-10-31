#include <iostream>
#include "student.hpp"
#include "course.hpp"
#include "parse_json.hpp"

//void generate_taken_courses(Student& student, std::vector<Course>& available_courses);

int main(){
    // Create a new student with id 1
    Student student(1);

    // Get a list of all courses available to cs majors
    ParseJson parsed_list("cs_major_courses.json");
    std::vector<Course> available_courses = parsed_list.get_course_list();

    // Randomly assigns courses to the student
    student.generate_taken_courses(available_courses);

    // Print all of the student's courses
    student.print_taken_courses();
    return 0;
}
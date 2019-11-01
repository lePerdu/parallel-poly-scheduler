#include <iostream>
#include "generate_students.cpp"
#include "course.cpp"
#include "parse_json.cpp"
//#include "generate_students.hpp"

int main(){


    // Get a list of all courses available to cs majors
    ParseJson parsed_list("cs_major_courses.json");
    std::vector<Course> available_courses = parsed_list.get_course_list();

    // Create a List of students based on the number of students and the list of available courses
    GenerateStudents Students(500, available_courses);
    Students.print_all_students_courses();

    std::vector<Student> students = Students.get_student_list();

    for (int i = 0; i < students.size(); i++){
        printf("Student id: [%u] -- Taken Credits: [%u]\n", students.at(i).student_id, students.at(i).taken_credits);
    }
    return 0;
}
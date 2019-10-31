#include "students.hpp"

Students::Students(const std::uint16_t total_students, std::vector<Course>& available_courses):
    total_students(total_students), available_courses(available_courses){}

void Students::generate_all_students(){
    for (int i = 0; i < total_students; i++){
        Student student(i);
        student.generate_taken_courses(available_courses);
        students.emplace_back(student);
    }
}

std::vector<Student>& Students::get_all_students() {
    return students;
}

void Students::print_all_students(){
    printf("Printing all [%u] Students\n", total_students);
    for (int i = 0; i < total_students; i++){
        students.at(i).print_taken_courses();
    }
}



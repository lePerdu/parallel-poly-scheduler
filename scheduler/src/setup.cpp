#include "setup.hpp"

const char* COURSES_FILE = "data/cs_major_courses.json";
const char* STUDENTS_FILE = "data/cs_students.json";

void setup(
        std::vector<Course>& available_courses,
        std::vector<Student>& students) {
    std::cout << "Extracting needed course list from " << COURSES_FILE
              << std::endl;
    available_courses = parse_courses_json(COURSES_FILE);

    std::cout << "Extracting needed student list from " << STUDENTS_FILE
              << std::endl;
    students = parse_student_json(STUDENTS_FILE, available_courses);

    std::cout << "Done extracting course and student lists" << std::endl;
}

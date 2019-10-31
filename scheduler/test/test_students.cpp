#include <iostream>
#include <random>
#include <vector>

#include "parse_json.cpp"
#include "students.cpp"
#include "student.cpp"
#include "course.cpp"

int main(){
    const uint16_t max_number_of_students = 10000;
    const uint16_t min_number_of_students = 500;

    /* Random Number Generator that generates a better distribution than srand */
    std::random_device dev;
    std::mt19937 rng(dev());

    // Pick a random number of students between (500 and 10000)
    std::uniform_int_distribution<std::mt19937::result_type> random_num_of_students(min_number_of_students, max_number_of_students);
    uint16_t total_students = random_num_of_students(rng);

    // Get a list of all courses available to cs majors
    ParseJson parsed_list("cs_major_courses.json");
    std::vector<Course> available_courses = parsed_list.get_course_list();

    Students students(total_students, available_courses);

    students.generate_all_students();

    std::vector<Student> student_list = students.get_all_students();
    students.print_all_students();
    return 0;
}
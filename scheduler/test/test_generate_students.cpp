#include "course.cpp"
#include "parse_json.cpp"
#include "student.cpp"

#include <iostream>

int main() {

    // Get a list of all courses available to cs majors
    ParseJson parsed_list("cs_major_courses.json");
    auto available_courses = parsed_list.get_course_list();

    // This is kinda annoying. Maybe have generate_random_students take the
    // vector a assume that poitners into it will not change?
    std::vector<const Course*> course_pointers(available_courses.size());
    for (auto& course : available_courses) {
        course_pointers.push_back(&course);
    }

    // Create a List of students based on the number of students and the list of
    // available courses
    auto students = generate_random_students(500, course_pointers);
    print_all_students_courses(students);

    return 0;
}

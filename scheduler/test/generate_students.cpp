#include "course.hpp"
#include "parse_json.hpp"
#include "student.hpp"

#include <iostream>

static const char* FILE_NAME = "data/cs_major_courses.json";

int main() {

    // Get a list of all courses available to cs majors
    ParseJson parsed_list(FILE_NAME);
    auto available_courses = parsed_list.get_course_list();

    // This is kinda annoying. Maybe have generate_random_students take the
    // vector a assume that poitners into it will not change?
    std::vector<Course::Ref> course_pointers;
    course_pointers.reserve(available_courses.size());
    for (size_t i = 0; i < available_courses.size(); ++i) {
        course_pointers.push_back({available_courses.data(), i});
    }

    // Create a List of students based on the number of students and the list of
    // available courses
    auto students = generate_random_students(500, course_pointers);
    print_all_students_courses(students);

    return 0;
}

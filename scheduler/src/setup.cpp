#include "setup.hpp"

const char* FILE_NAME = "data/cs_major_courses.json";

void setup(
        std::vector<Course>& available_courses,
        std::vector<Course::Ref>& course_pointers) {
    extract_courses(available_courses, course_pointers);
    // students = generate_random_students(TOTALSTUDENTS, course_pointers);
}

void extract_courses(
        std::vector<Course>& available_courses,
        std::vector<Course::Ref>& course_pointers) {
    std::cout << "Extracting Needed Course List From " << FILE_NAME
              << std::endl;
    ParseJson parsed_list(FILE_NAME);
    available_courses = parsed_list.get_course_list();

    course_pointers.reserve(available_courses.size());
    for (std::size_t i = 0; i < available_courses.size(); ++i) {
        course_pointers.emplace_back(available_courses.data(), i);
    }
    std::cout << "Done Extracting Course List" << std::endl;
}

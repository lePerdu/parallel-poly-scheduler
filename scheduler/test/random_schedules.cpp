#include "parse_json.hpp"
#include "schedule.hpp"
#include "student.hpp"

static const char* FILE_NAME = "data/cs_major_courses.json";

int main(int argc, char* argv[]) {
    // Get a list of all courses available to cs majors
    ParseJson parsed_list(FILE_NAME);
    auto available_courses = parsed_list.get_course_list();

    // This is kinda annoying. Maybe have generate_random_students take the
    // vector a assume that poitners into it will not change?
    std::vector<const Course*> course_pointers;
    course_pointers.reserve(available_courses.size());
    for (const auto& c : available_courses) {
        course_pointers.push_back(&c);
    }

    // Create a List of students based on the number of students and the list of
    // available courses
    auto students = generate_random_students(500, course_pointers);

    // Schedule from 8AM to 6PM
    Schedule schedule(8 * 2, 18 * 2);
    for (auto& student : students) {
        schedule.add_random_section(student.taken_courses[0]);
    }

    schedule.print();

    return 0;
}

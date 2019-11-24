#include "parse_json.hpp"
#include "schedule.hpp"
#include "student.hpp"

static const char* FILE_NAME = "data/cs_major_courses.json";

constexpr unsigned CLASS_CAPACITY = 30;

int main(int argc, char* argv[]) {
    // Get a list of all courses available to cs majors
    ParseJson parsed_list(FILE_NAME);
    auto available_courses = parsed_list.get_course_list();

    // This is kinda annoying. Maybe have generate_random_students take the
    // vector a assume that poitners into it will not change?
    std::vector<Course::Ref> course_pointers;
    course_pointers.reserve(available_courses.size());
    for (std::size_t i = 0; i < available_courses.size(); ++i) {
        course_pointers.emplace_back(available_courses.data(), i);
    }

    // Create a List of students based on the number of students and the list of
    // available courses
    auto students = generate_random_students(500, course_pointers);

    // Schedule from 8AM to 6PM
    Schedule schedule(
            8 * 2, 18 * 2, required_course_counts(CLASS_CAPACITY, students));

    schedule.print();

    std::cout << "Score: " << schedule.fitness_score(students) << std::endl;

    return 0;
}

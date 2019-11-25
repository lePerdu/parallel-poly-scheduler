#include "parse_json.hpp"
#include "schedule.hpp"
#include "setup.hpp"
#include "student.hpp"

constexpr unsigned CLASS_CAPACITY = 30;

int main(int argc, char* argv[]) {
    // Get a list of all courses available to cs majors
    std::vector<Course> course_list;
    std::vector<Student> student_list;
    setup(course_list, student_list);

    // Schedule from 8AM to 6PM
    Schedule schedule(
            8 * 2,
            18 * 2,
            required_course_counts(CLASS_CAPACITY, student_list));

    schedule.print();

    std::cout << "Score: " << schedule.fitness_score(student_list) << std::endl;

    return 0;
}

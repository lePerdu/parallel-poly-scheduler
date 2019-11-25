#include "parse_json.hpp"

#include "setup.hpp"

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::vector<Course> course_list;
    std::vector<Student> student_list;
    setup(course_list, student_list);

    for (const auto& c : course_list) {
        c.print_course();
    }

    return 0;
}

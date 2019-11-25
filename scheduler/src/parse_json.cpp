#include "parse_json.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Import a JSON file and Parse it to a course vector
std::vector<Course> parse_courses_json(const char* filename) {
    std::ifstream input_file(filename);
    json json_course_map;
    input_file >> json_course_map;

    // Variables that are stored in the json file:
    // id: int
    // name: string
    // department: string
    // number: string (string due to letter chars in some instances)
    // credits: int

    // Iterates through the whole json list and adds each found title/credit
    // into a course. Course gets pushed into course_list vector. Could be
    // extended with all attributes if the Course class needs them
    std::vector<Course> course_list;
    for (auto& course : json_course_map.items()) {
        int id = course.value()["id"];
        std::string name(course.value()["title"]);
        std::uint8_t credits = course.value()["credits"];
        course_list.emplace_back(id, name, credits);
    }

    return course_list;
}

static Course::Ref
localize_course_id(unsigned id, const std::vector<Course>& all_courses) {
    for (size_t i = 0; i < all_courses.size(); ++i) {
        if (all_courses[i].get_id() == id) {
            return {all_courses.data(), i};
        }
    }

    throw std::runtime_error(
            std::string("Could not find course with ID ") + std::to_string(id));
}

std::vector<Student> parse_student_json(
        const char* filename, const std::vector<Course>& all_courses) {
    std::ifstream input_file(filename);
    json json_student_list;
    input_file >> json_student_list;

    // Variables that are stored in the json file:
    // id: int
    // taken_courses: list of int (course IDs)
    // wanted_courses: list of int (course IDs)

    // The IDs are converted into indices in the passed in array since those
    // are easier and faster to work with (after initial parsing)
    std::vector<Student> student_list;
    for (auto& student : json_student_list) {
        unsigned id = student["id"];

        std::vector<Course::Ref> taken_courses;
        for (auto& id : student["taken_courses"]) {
            taken_courses.push_back(localize_course_id(id, all_courses));
        }

        std::vector<Course::Ref> wanted_courses;
        for (auto& id : student["wanted_courses"]) {
            wanted_courses.push_back(localize_course_id(id, all_courses));
        }

        student_list.emplace_back(id, taken_courses, wanted_courses);
    }

    return student_list;
}

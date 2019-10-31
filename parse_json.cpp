#include "parse_json.hpp"

using json = nlohmann::json;

// Import a JSON file and Parse it to a course vector
ParseJson::ParseJson(const std::string& file_name) {
    /* Takes the JSON file and parses it based on attributes we are looking for
     */
    std::ifstream input_file(file_name);
    json json_course_list;
    input_file >> json_course_list;
    // printf("Total number of courses found: [%d]", jsonCourseList.size());

    // Variables that are stored in the json file:
    // name: string
    // department: string
    // number: string (string due to letter chars in some instances)
    // credits: int

    // Iterates through the whole json list and adds each found title/credit
    // into a course. Course gets pushed into course_list vector. Could be
    // extended with all attributes if the Course class needs them
    for (auto& jj : json_course_list) {
        std::string name = (jj["title"]);
        std::uint8_t credits = jj["credits"];
        course_list.emplace_back(name, credits);
    }
}

const std::vector<Course>& ParseJson::get_course_list() const {
    return course_list;
}

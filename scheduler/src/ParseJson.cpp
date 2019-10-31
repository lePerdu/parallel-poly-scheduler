#include "ParseJson.hpp"

using json = nlohmann::json;

// Import a JSON file and Parse it to a course vector
ParseJson::ParseJson(std::string& fileName){
        /* Takes the JSON file and parses it based on attributes we are looking for */
        std::ifstream inputFile(fileName);
        json jsonCourseList;
        inputFile >> jsonCourseList;
        //printf("Total number of courses found: [%d]", jsonCourseList.size());

        // Variables that are stored in the json file
        std::string name, department, number; // number is string due to letter chars added at the end in some instances
        std::uint8_t credits;
        std::uint16_t id;

        // Iterates through the whole json list and adds each found title/credit into a course.. Course gets pushed into courselist vector
        // Could be extended with all attributes if the Course class needs them
        for (auto& jj : jsonCourseList){
            name = (jj["title"]);
            credits = (jj["credits"]);
            Course foundCourse(name, credits);
            courseList.push_back(foundCourse);
        }
}

std::vector<Course>& ParseJson::get_course_list(){
    return courseList;
}
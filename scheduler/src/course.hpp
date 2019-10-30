#ifndef COURSE_HPP_
#define COURSE_HPP_

#include <string>
#include <vector>

class Course {
public:
    Course(std::string name, std::uint8_t credits);

    const std::string& get_name() const;

    void add_prereq(const Course* prereq);

    bool has_prereq(const Course* prereq) const;

private:
    std::string name;
    std::uint8_t credits;
    std::vector<const Course*> prereqs;
};

#endif // COURSE_HPP_

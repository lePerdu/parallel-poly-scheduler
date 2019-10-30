#ifndef SCHEDULE_HPP_
#define SCHEDULE_HPP_

#include "course.hpp"
#include "section.hpp"

#include <map>
#include <vector>

class Schedule {
public:
    /**
     * Add a set of courses for a single student.
     *
     * If possible, the student will be added to existing sections; otherwise,
     * new sections will be created. All of the sections the student is added to
     * will be non-overlapping.
     */
    void merge_student_courses(std::vector<const Course*> courses);

private:
    /**
     * Picks an existing section or adds a new one.
     *
     * The section returned will not overlap with any of the sections in
     * registered.
     *
     * \param course Course to add
     * \param registered The sections which the student is currently
     * registered for. These are to be used for making sure a student is not
     * registered for conflicting sections.
     *
     * \return A pointer to the section that was picked / added.
     */
    const Section* pick_or_add_section(
            const Course* course,
            const std::vector<const Section*>& registered);

    /**
     * Sections in the schedule, keyed by the course.
     */
    std::multimap<const Course*, Section> sections;
};

#endif /* SCHEDULE_HPP_ */

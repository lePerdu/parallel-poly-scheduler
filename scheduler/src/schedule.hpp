#ifndef SCHEDULE_HPP_
#define SCHEDULE_HPP_

#include "class_time.hpp"
#include "course.hpp"
#include "section.hpp"

#include <map>
#include <vector>

class Schedule {
public:
    /**
     * Constructs an empty schedule.
     */
    Schedule(Time start_time, Time end_time);

    /**
     * Cosntructs a schedule with the given sections.
     */
    Schedule(
            Time start_time,
            Time end_time,
            const std::vector<Section>& sections);

    /**
     * An order multimap is used so that multiple ones can be iterated over in
     * "parallel" while pointing to the same courses (assuming they have the
     * same sets of courses and sections).
     */
    using SectionContainer = std::multimap<const Course*, Section>;

    const SectionContainer& get_sections() const;

    /**
     * Adds a section of course at a random time.
     */
    void add_random_section(const Course* course);

    /**
     * Adds count sections of course at random times.
     */
    void add_random_sections(const Course* course, std::size_t count);

    /**
     * Add a set of courses for a single student.
     *
     * If possible, the student will be added to existing sections; otherwise,
     * new sections will be created. All of the sections the student is added to
     * will be non-overlapping.
     *
     * TODO Remove?
     */
    // void merge_student_courses(const std::vector<const Course*>& courses);

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
     *
     * TODO Remove? This logic could be computed elsewhere
     */
    // const Section* pick_or_add_section(
    //         const Course* course,
    //         const std::vector<const Section*>& registered);

    /**
     * Start and end time for all courses in the schedule.
     */
    Time start_time, end_time;

    /**
     * Sections in the schedule, keyed by the course.
     * TODO Do these really need to be indexed by course?
     */
    SectionContainer sections;
};

#endif /* SCHEDULE_HPP_ */

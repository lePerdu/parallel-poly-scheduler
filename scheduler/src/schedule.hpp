#ifndef SCHEDULE_HPP_
#define SCHEDULE_HPP_

#include "class_time.hpp"
#include "course.hpp"
#include "section.hpp"
#include "student.hpp"

#include <map>
#include <vector>

class Schedule {
public:
    /**
     * Constructs an empty schedule.
     */
    Schedule(Time start_time, Time end_time);

    /**
     * Constructs a schedule with random sections based on the number of
     * sections required for each course.
     */
    Schedule(
            Time start_time,
            Time end_time,
            std::unordered_map<Course::Ref, std::uint8_t> course_counts);

    /**
     * Constructs a schedule as the cross-over between 2 parent schedules.
     *
     * The parents must have the same start and end times, as well as the same
     * number of sections of each course.
     *
     * For each section, this randomly choses between the class layout of
     * parent1 and parent2.
     */
    Schedule(const Schedule& parent1, const Schedule& parent2);

    /**
     * An order multimap is used so that multiple ones can be iterated over in
     * "parallel" while pointing to the same courses (assuming they have the
     * same sets of courses and sections).
     */
    using SectionContainer = std::multimap<Course::Ref, Section>;

    Time get_start_time() const {
        return start_time;
    }

    Time get_end_time() const {
        return end_time;
    }

    const SectionContainer& get_sections() const;

    /**
     * Add a section to this sechedule.
     */
    void add_section(Section section);

    /**
     * Adds a section of course at a random time.
     */
    void add_random_section(Course::Ref course);

    /**
     * Adds count sections of course at random times.
     */
    void add_random_sections(Course::Ref course, unsigned count);

    /**
     * Mutates a single section in this schedule.
     *
     * TODO Take a probability parameter and potentially mutate multiple
     * sections, or rely on the caller to decide when to mutate a schedule?
     * The former is more "correct", but could be less performant.
     */
    void mutate();

    /**
     * Add a set of courses for a single student.
     *
     * If possible, the student will be added to existing sections; otherwise,
     * new sections will be created. All of the sections the student is added to
     * will be non-overlapping.
     *
     * TODO Remove?
     */
    // void merge_student_courses(const std::vector<Course::Ref>& courses);

    /**
     * Generate a schedule for a single student based on their wanted courses.
     *
     * This finds a schedule with the most courses which can be taken
     * simultaneously.
     *
     * Note: The schedule returned is only valid while this schedule is not
     * changed (since it holds pointers to sections in this schedule).
     */
    std::vector<const Section*>
    gen_student_schedule(const Student& student) const;

    /**
     * Compute the fitness score given all students.
     *
     * This is the square average of the ratio of wanted courses that students
     * can take.
     */
    float fitness_score(const std::vector<Student>& students) const;

    /**
     * Prints a schedule in a somewhat-human-readable format.
     */
    void print() const;

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
    //         Course::Ref course,
    //         const std::vector<const Section*>& registered);

    Section make_random_section(Course::Ref course) const;

    /**
     * Helper function for gen_student_schedule().
     *
     * All of the sections for the course at wanted_idx are tried in the
     * student's schedule, then this is called recursively for the next indices.
     * This uses backtracking and is meant to be called recursively. It saves
     * the best schedule in best to be accessed across all iterations.
     *
     * @param student Student to build a schedule for.
     * @param wanted_idx Index of the student's wanted course to work with
     * first.
     * @param schedule The schedule currently being built. This may be modified
     * transitively, but should have the same elements when the function returns
     * as when it is passed in.
     * @param best The best schedule found so far.
     */
    void build_student_schedule(
            const Student& student,
            size_t wanted_idx,
            std::vector<const Section*>& schedule,
            std::vector<const Section*>& best) const;

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

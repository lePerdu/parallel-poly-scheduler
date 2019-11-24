#include "schedule.hpp"

#include "class_time.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <random>

Schedule::Schedule(Time start, Time end) : start_time(start), end_time(end) {}

Schedule::Schedule(
        Time start,
        Time end,
        std::unordered_map<Course::Ref, std::uint8_t> course_counts) :
    start_time(start), end_time(end) {
    for (auto& item : course_counts) {
        add_random_sections(item.first, item.second);
    }
}

Schedule::Schedule(const Schedule& parent1, const Schedule& parent2) :
    start_time(parent1.start_time), end_time(parent1.end_time) {
    // These assertions are only run in "debug mode", when NDEBUG is not defined
    // (see the Makefile)
    assert(parent1.start_time == parent2.start_time);
    assert(parent1.end_time == parent2.end_time);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> rand_parent(0, 1);

    // Assume the parents have the same sections.
    // Since a [ordered_]multimap is used, both iterators will have  the same
    // order
    auto end1 = parent1.sections.end();
    auto end2 = parent2.sections.end();
    for (auto it1 = parent1.sections.begin(), it2 = parent2.sections.begin();
         it1 != end1 && it2 != end2;
         ++it1, ++it2) {
        // Make sure the invarient holds
        assert(it1->first == it2->first);

        sections.insert(rand_parent(rng) == 0 ? *it1 : *it2);
    }
}

const Schedule::SectionContainer& Schedule::get_sections() const {
    return sections;
}

Section Schedule::make_random_section(Course::Ref course) const {
    auto layout =
            random_class_layout(course->get_credits(), start_time, end_time);
    return {course, layout};
}

void Schedule::add_random_section(Course::Ref course) {
    auto s = make_random_section(course);
    if (!s.layout.empty()) {
        sections.insert({course, s});
    } else {
        std::cerr << "Empty layout for credits "
                  << (unsigned)course->get_credits() << std::endl;
    }
}

void Schedule::add_random_sections(Course::Ref course, unsigned count) {
    for (unsigned i = 0; i < count; ++i) {
        add_random_section(course);
    }
}

void Schedule::mutate() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> rand_section_idx(0, sections.size() - 1);

    auto it = sections.begin();
    std::advance(it, rand_section_idx(rng));
    it->second = make_random_section(it->second.course);
}

std::vector<const Section*>
Schedule::gen_student_schedule(const Student& student) const {
    const auto wanted_count = student.wanted_courses.size();
    std::vector<const Section*> schedule;
    schedule.reserve(wanted_count);

    std::vector<const Section*> best_schedule;
    best_schedule.reserve(wanted_count);

    build_student_schedule(student, 0, schedule, best_schedule);
    return best_schedule;
}

unsigned Schedule::fitness_score(const std::vector<Student>& students) const {
    unsigned score = 0;
    for (auto& student : students) {
        score += gen_student_schedule(student).size();
    }

    return score;
}

void Schedule::build_student_schedule(
        const Student& student,
        size_t wanted_idx,
        std::vector<const Section*>& schedule,
        std::vector<const Section*>& best) const {
    if (wanted_idx >= student.wanted_courses.size()) {
        if (schedule.size() > best.size()) {
            best = schedule;
        }
        return;
    }

    // Early exit if all wanted courses are in the best one
    if (best.size() >= student.wanted_courses.size()) {
        return;
    }

    // Early exit if the current schedule line could not possibly get better
    // than best.
    if (student.wanted_courses.size() - wanted_idx + schedule.size() <=
        best.size()) {
        return;
    }

    const auto course = student.wanted_courses[wanted_idx];

    const auto course_range = sections.equal_range(course);
    for (auto it = course_range.first; it != course_range.second; ++it) {
        // Only add it if there are no conflicts
        const bool can_add = std::none_of(
                schedule.begin(), schedule.end(), [&](const Section* sect) {
                    return sect->overlaps(it->second);
                });

        if (can_add) {
            schedule.push_back(&it->second);
            build_student_schedule(student, wanted_idx + 1, schedule, best);
            schedule.pop_back();
        }
    }

    // Also try without this course
    build_student_schedule(student, wanted_idx + 1, schedule, best);
}

void Schedule::print() const {
    for (auto& elem : sections) {
        elem.second.print();
    }
}

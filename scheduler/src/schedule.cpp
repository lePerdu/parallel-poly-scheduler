#include "schedule.hpp"

#include "class_time.hpp"

#include <cassert>
#include <iostream>
#include <iterator>
#include <random>

Schedule::Schedule(Time start, Time end) : start_time(start), end_time(end) {}

Schedule::Schedule(Time start, Time end, const std::vector<Section>& sections) :
    Schedule(start, end) {
    for (auto& s : sections) {
        this->sections.insert({s.course, s});
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

void Schedule::add_random_sections(Course::Ref course, std::size_t count) {
    for (std::size_t i = 0; i < count; ++i) {
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

void Schedule::print() const {
    for (auto& elem : sections) {
        elem.second.print();
    }
}

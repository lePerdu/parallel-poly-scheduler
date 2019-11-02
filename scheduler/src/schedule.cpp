#include "schedule.hpp"

#include "class_time.hpp"

Schedule::Schedule(Time start, Time end) : start_time(start), end_time(end) {}

Schedule::Schedule(Time start, Time end, const std::vector<Section>& sections) :
    Schedule(start, end) {
    for (auto& s : sections) {
        this->sections.insert({s.course, s});
    }
}

const Schedule::SectionContainer& Schedule::get_sections() const {
    return sections;
}

void Schedule::add_random_section(const Course* course) {
    auto layout =
            random_class_layout(course->get_credits(), start_time, end_time);
    sections.insert({course, Section(course, layout)});
}

void Schedule::add_random_sections(const Course* course, std::size_t count) {
    for (std::size_t i = 0; i < count; ++i) {
        add_random_section(course);
    }
}

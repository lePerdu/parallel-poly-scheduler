#include "class_time.hpp"

///
/// ClassTime
///

ClassTime::ClassTime(Day day, Time start, Time end) :
    day(day), start(start), end(end) {}

bool ClassTime::overlaps(const ClassTime& other) const {
    return day == other.day && start < other.end && end >= other.start;
}

bool ClassTime::operator==(const ClassTime& other) const {
    return day == other.day && start == other.start && end == other.end;
}

ClassTime ClassTime::next_slot() const {
    return ClassTime(day, end, end + (end - start));
}

ClassTime ClassTime::offset(Time time) const {
    return ClassTime(day, start + time, end + time);
}

///
/// ClassLayout
///

ClassLayout::ClassLayout() {}

ClassLayout::ClassLayout(std::vector<ClassTime> times) : times(times) {}

ClassLayout::ClassLayout(std::initializer_list<ClassTime> times) :
    times(times) {}

const std::vector<ClassTime>& ClassLayout::get_times() const {
    return times;
}

bool ClassLayout::empty() const {
    return times.empty();
}

bool ClassLayout::overlaps(const ClassLayout& other) const {
    for (const auto& time : times) {
        for (const auto& other_time : other.times) {
            if (time.overlaps(other_time)) {
                return true;
            }
        }
    }

    return false;
}

bool ClassLayout::contained_in(Time start, Time end) const {
    for (auto& time : times) {
        if (time.start < start || end < time.end) {
            return false;
        }
    }

    return true;
}

void ClassLayout::offset(Time shift) {
    for (auto& time : times) {
        time = time.offset(shift);
    }
}

bool ClassLayout::operator==(const ClassLayout& other) const {
    return times == other.times;
}

///
/// ClassLayoutGenerator
///

ClassLayoutGenerator::ClassLayoutGenerator(
        std::uint8_t credits, Time start, Time end) :
    credits(credits), start_time(start), end_time(end) {}

// Alias for less typing
using Iterator = ClassLayoutGenerator::Iterator;

Iterator ClassLayoutGenerator::begin() const {
    return Iterator(this);
}

Iterator ClassLayoutGenerator::end() const {
    return Iterator(this, true);
}

// Preset templates for each class layout

//  Just an empty layout
const ClassLayout BASE_LAYOUTS_OTHER[] = {{}};

const ClassLayout BASE_LAYOUTS_1[] = {
        {ClassTime(Day::MONDAY, 0, 2)},
        {ClassTime(Day::TUESDAY, 0, 2)},
        {ClassTime(Day::WEDNESDAY, 0, 2)},
        {ClassTime(Day::THURSDAY, 0, 2)},
        {ClassTime(Day::FRIDAY, 0, 2)},
        {},
};

const ClassLayout BASE_LAYOUTS_3[] = {
        // MWF
        {ClassTime(Day::MONDAY, 0, 2),
         ClassTime(Day::WEDNESDAY, 0, 2),
         ClassTime(Day::FRIDAY, 0, 2)},
        // TR
        {ClassTime(Day::TUESDAY, 0, 3), ClassTime(Day::THURSDAY, 0, 3)},
        // MW
        {ClassTime(Day::MONDAY, 0, 3), ClassTime(Day::WEDNESDAY, 0, 3)},
        {},
};

const ClassLayout BASE_LAYOUTS_4[] = {
        // MWF
        {ClassTime(Day::MONDAY, 0, 3),
         ClassTime(Day::WEDNESDAY, 0, 3),
         ClassTime(Day::FRIDAY, 0, 3)},
        {},
};

static const ClassLayout* credit_base_layouts(std::uint8_t credits) {
    switch (credits) {
    case 1:
        return BASE_LAYOUTS_1;
    case 3:
        return BASE_LAYOUTS_3;
    case 4:
        return BASE_LAYOUTS_4;
    default:
        return BASE_LAYOUTS_OTHER;
    }
}

Iterator::Iterator(const ClassLayoutGenerator* generator, bool end) :
    generator(generator) {
    // Use empty layout if at end
    current_base = credit_base_layouts(end ? 0 : generator->credits);
    current = *current_base;
    current.offset(generator->start_time);
}

const ClassLayout& Iterator::operator*() const {
    return current;
}

const ClassLayout* Iterator::operator->() const {
    return &current;
}

Iterator& Iterator::operator++() {
    if (current.empty()) {
        return *this;
    }

    current.offset(1);
    // Move to next base layout if necessary
    if (!current.contained_in(generator->start_time, generator->end_time)) {
        ++current_base;
        current = *current_base;
        current.offset(generator->start_time);
    }

    return *this;
}

Iterator Iterator::operator++(int) {
    auto orig = *this;
    ++(*this);
    return orig;
}

bool Iterator::operator==(const Iterator& other) const {
    return generator == other.generator && current == other.current;
}

bool Iterator::operator!=(const Iterator& other) const {
    return !(*this == other);
}

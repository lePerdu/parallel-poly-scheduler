#include "class_time.hpp"

#include <random>
#include <unordered_map>
#include <utility>

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

bool ClassLayoutGenerator::operator==(const ClassLayoutGenerator& other) const {
    return credits == other.credits && start_time == other.start_time &&
           end_time == other.end_time;
}

bool ClassLayoutGenerator::operator!=(const ClassLayoutGenerator& other) const {
    return !(*this == other);
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

///
/// ClassLayoutGenerator cache
///

/**
 * Get a vecctor of all layouts for each credit for more efficient lookups.
 */
static std::vector<ClassLayout>
generate_all_layouts(ClassLayoutGenerator generator) {
    return {generator.begin(), generator.end()};
}

static std::unordered_map<ClassLayoutGenerator, std::vector<ClassLayout>>
        class_layout_cache;

const std::vector<ClassLayout>&
all_class_layouts(std::uint8_t credits, Time start_time, Time end_time) {
    ClassLayoutGenerator gen(credits, start_time, end_time);
    auto it = class_layout_cache.find(gen);
    if (it == class_layout_cache.end()) {
        // returns pair of (iterator, bool)
        auto res = class_layout_cache.insert({gen, generate_all_layouts(gen)});
        it = std::get<0>(res);
    }

    return std::get<1>(*it);
}

const ClassLayout&
random_class_layout(std::uint8_t credits, Time start_time, Time end_time) {
    auto all_layouts = all_class_layouts(credits, start_time, end_time);
    if (all_layouts.empty()) {
        // Empty layout
        return BASE_LAYOUTS_OTHER[0];
    }

    // This seems like an awful lot of code just to get a single random integer
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::uniform_int_distribution<int> distribution(0, all_layouts.size() - 1);

    return all_layouts[distribution(rng)];
}

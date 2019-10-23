#ifndef CLASS_TIME_HPP_
#define CLASS_TIME_HPP_

#include <cstdint>
#include <vector>

enum Day { MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY };

/**
 * Time is stored as an index from the start of the day.
 *
 * The time interval between indices is 30 minutes.
 */
using Time = std::uint8_t;

/**
 * Week day and time for a course section.
 */
struct ClassTime {
    Day day;
    Time start;
    Time end;

    ClassTime(Day day, Time start, Time end);

    bool overlaps(const ClassTime &other) const;

    bool operator==(const ClassTime &other) const;

    /**
     * Creates a class time of the same length immediately after this one.
     */
    ClassTime next_slot() const;

    ClassTime offset(Time shift) const;
};

/**
 * Set of class times
 */
class ClassLayout {
public:
    ClassLayout();
    ClassLayout(std::initializer_list<ClassTime> times);
    ClassLayout(std::vector<ClassTime> times);

    const std::vector<ClassTime> &get_times() const;

    bool empty() const;

    bool overlaps(const ClassLayout &other) const;

    bool contained_in(Time start, Time end) const;

    void offset(Time shift);

    bool operator==(const ClassLayout &other) const;

private:
    std::vector<ClassTime> times;
};

/**
 * Generates a set of class layouts for a number of credit hours.
 *
 * The order of the layouts is unspecified.
 * For now, this is only defined for 1- and 3-credit hours courses.
 */
class ClassLayoutGenerator {
public:
    ClassLayoutGenerator(std::uint8_t credits, Time start, Time end);

    class Iterator {
    public:
        Iterator(const ClassLayoutGenerator *generator, bool end = false);

        /**
         * Refrence to the current layout.
         */
        const ClassLayout &operator*() const;
        const ClassLayout *operator->() const;

        Iterator &operator++();
        Iterator operator++(int);

        bool operator==(const Iterator &other) const;
        bool operator!=(const Iterator &other) const;

    private:
        const ClassLayoutGenerator *generator;
        const ClassLayout *current_base;
        ClassLayout current;
    };

    Iterator begin() const;
    Iterator end() const;

private:
    // Configuration parameters

    std::uint8_t credits;
    Time start_time;
    Time end_time;
};

#endif // CLASS_TIME_HPP_

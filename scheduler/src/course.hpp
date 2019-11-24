#ifndef COURSE_HPP_
#define COURSE_HPP_

#include <cassert>
#include <string>
#include <vector>

class Course {
public:
    /**
     * Behaves like const Course*, but holds index information.
     *
     * This exists so that pointers to courses can be sent through MPI messages.
     * The base part should be ignored / not sent when transmitting course
     * references to other nodes (and they should use the base pointer for their
     * own course arrays).
     */
    class Ref {
    public:
        Ref(const Course* base, std::size_t offset) :
            base(base), offset(offset) {}

        const Course* operator->() const {
            return &base[offset];
        }

        const Course* get_base() const {
            return base;
        }

        std::size_t get_offset() const {
            return offset;
        }

        /**
         * Compares Course references for equality.
         *
         * Note: The base values of both references must be the same (this is
         * ensured in debug mode, but not in production as an optimization).
         */
        bool operator==(const Ref& other) const {
            assert(base == other.base);
            return offset == other.offset;
        }

        bool operator!=(const Ref& other) const {
            return !(*this == other);
        }

        /**
         * Compares Course references.
         *
         * Note: The base values of both references must be the same (this is
         * ensured in debug mode, but not in production as an optimization).
         */
        bool operator<(const Ref& other) const {
            assert(base == other.base);
            return offset < other.offset;
        }

        bool operator>(const Ref& other) const {
            return other < *this;
        }

        bool operator<=(const Ref& other) const {
            return !(*this > other);
        }

        bool operator>=(const Ref& other) const {
            return !(*this < other);
        }

    private:
        const Course* base;
        std::size_t offset;
    };

    Course(std::string name, std::uint8_t credits);
    const std::string& get_name() const;
    std::uint8_t get_credits() const;
    void add_prereq(Ref prereq);
    bool has_prereq(Ref prereq) const;
    void print_course() const;

private:
    std::string name;
    std::uint8_t credits;
    std::vector<Ref> prereqs;
};

#endif // COURSE_HPP_

#ifndef MPI_TYPES_HPP_
#define MPI_TYPES_HPP_

#include "class_time.hpp"

#include <cstdint>
#include <mpi.h>

extern MPI_Datatype SIZE_T_MPI;
extern MPI_Datatype DAY_MPI;
extern MPI_Datatype TIME_MPI;
extern MPI_Datatype CLASS_TIME_MPI;
extern MPI_Datatype COURSE_INFO_MPI;
extern MPI_Datatype STUDENT_INFO_MPI;
extern MPI_Datatype CLASS_LAYOUT_INFO_MPI;
extern MPI_Datatype SECTION_INFO_MPI;
extern MPI_Datatype SCHEDULE_INFO_MPI;

// Some intermediate types for sending in MPI messages

struct CourseInfo {
    unsigned id;
    std::uint8_t credits;
    std::size_t name_size;
};

struct StudentInfo {
    unsigned id;
    std::size_t taken_courses_size;
    std::size_t wanted_courses_size;
};

struct ClassLayoutInfo {
    std::size_t times_size;
};

struct SectionInfo {
    std::size_t course_index;
    ClassLayoutInfo layout_info;
};

struct ScheduleInfo {
    Time start_time;
    Time end_time;
    std::size_t sections_size;
};

void init_mpi_types();

void free_mpi_types();

#endif // MPI_TYPES_HPP_


#include "mpi_types.hpp"

#include "class_time.hpp"

#include <cstddef>
#include <utility>

// These are just aliases

MPI_Datatype SIZE_T_MPI = MPI_UNSIGNED_LONG;

MPI_Datatype TIME_MPI = MPI_UNSIGNED_CHAR;
MPI_Datatype DAY_MPI = MPI_CHAR;

// This is just a plain datatype to begin with
MPI_Datatype CLASS_TIME_MPI;

// These are for the intermediate types
MPI_Datatype COURSE_INFO_MPI;
MPI_Datatype STUDENT_INFO_MPI;
MPI_Datatype CLASS_LAYOUT_INFO_MPI;
MPI_Datatype SECTION_INFO_MPI;
MPI_Datatype SCHEDULE_INFO_MPI;

void init_mpi_types() {
    // ClassTime
    {
        const int blocks[] = {1, 1, 1};
        const MPI_Datatype types[] = {
                MPI_CHAR, MPI_UNSIGNED_CHAR, MPI_UNSIGNED_CHAR};
        const MPI_Aint displacements[] = {
                offsetof(ClassTime, day),
                offsetof(ClassTime, start),
                offsetof(ClassTime, end),
        };

        MPI_Type_create_struct(
                3, blocks, displacements, types, &CLASS_TIME_MPI);

        MPI_Type_commit(&CLASS_TIME_MPI);
    }

    // CourseInfo
    {
        const int blocks[] = {1, 1, 1};
        const MPI_Datatype types[] = {
                MPI_UNSIGNED, MPI_UNSIGNED_CHAR, SIZE_T_MPI};
        const MPI_Aint displacements[] = {
                offsetof(CourseInfo, id),
                offsetof(CourseInfo, credits),
                offsetof(CourseInfo, name_size),
        };

        MPI_Type_create_struct(
                3, blocks, displacements, types, &COURSE_INFO_MPI);
        MPI_Type_commit(&COURSE_INFO_MPI);
    }

    // StudentInfo
    {
        const int blocks[] = {1, 1, 1};
        const MPI_Datatype types[] = {MPI_UNSIGNED, SIZE_T_MPI, SIZE_T_MPI};
        const MPI_Aint displacements[] = {
                offsetof(StudentInfo, id),
                offsetof(StudentInfo, taken_courses_size),
                offsetof(StudentInfo, wanted_courses_size),
        };

        MPI_Type_create_struct(
                3, blocks, displacements, types, &STUDENT_INFO_MPI);
        MPI_Type_commit(&STUDENT_INFO_MPI);
    }

    // ClassLayouttInfo
    {
        MPI_Type_contiguous(1, SIZE_T_MPI, &CLASS_LAYOUT_INFO_MPI);
        MPI_Type_commit(&CLASS_LAYOUT_INFO_MPI);
    }

    // SectionInfo
    {
        const int blocks[] = {1, 1};
        const MPI_Datatype types[] = {SIZE_T_MPI, CLASS_LAYOUT_INFO_MPI};
        const MPI_Aint displacements[] = {
                offsetof(SectionInfo, course_index),
                offsetof(SectionInfo, layout_info),
        };

        MPI_Type_create_struct(
                2, blocks, displacements, types, &SECTION_INFO_MPI);
        MPI_Type_commit(&SECTION_INFO_MPI);
    }

    // ScheduleInfo
    {
        const int blocks[] = {1, 1, 1, 1};
        const MPI_Datatype types[] = {
                TIME_MPI, TIME_MPI, MPI_FLOAT, SIZE_T_MPI};
        const MPI_Aint displacements[] = {
                offsetof(ScheduleInfo, start_time),
                offsetof(ScheduleInfo, end_time),
                offsetof(ScheduleInfo, score),
                offsetof(ScheduleInfo, sections_size),
        };

        MPI_Type_create_struct(
                4, blocks, displacements, types, &SCHEDULE_INFO_MPI);
        MPI_Type_commit(&SCHEDULE_INFO_MPI);
    }
}

void free_mpi_types() {
    MPI_Type_free(&CLASS_TIME_MPI);
    MPI_Type_free(&COURSE_INFO_MPI);
    MPI_Type_free(&STUDENT_INFO_MPI);
    MPI_Type_free(&CLASS_LAYOUT_INFO_MPI);
    MPI_Type_free(&SECTION_INFO_MPI);
    MPI_Type_free(&SCHEDULE_INFO_MPI);
}

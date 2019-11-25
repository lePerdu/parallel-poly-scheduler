#include "node.hpp"

#include "mpi_types.hpp"

enum : int {
    SECTION_INFO_TAG,
    SCHEDULE_INFO_TAG,
    CLASS_TIMES_TAG,
};

Node::Node(int rank) {
    this->rank = rank;
}

void Node::start_node_work() {
    // Extract courses from JSON File and save as a vector of course pointers
    if (rank == MASTER)
        setup(available_courses, students);

    broadcast_course_list();
    broadcast_student_list();

    if (rank == 1) {
        for (auto& course : available_courses) {
            course.print();
        }

        print_all_students_courses(students);
    }
}

Course::Ref Node::make_course_ref(std::size_t index) const {
    return {available_courses.data(), index};
}

void Node::broadcast_course_list() {
    // Send how long the course list is
    std::size_t course_list_size;
    if (rank == MASTER)
        course_list_size = available_courses.size();
    MPI_Bcast(&course_list_size, 1, SIZE_T_MPI, MASTER, MPI_COMM_WORLD);

    // Iterate through the course list
    for (std::size_t i = 0; i < course_list_size; i++) {
        // Send the course ID, credits, and the size of the name in a single
        // packed message

        CourseInfo course_info;
        std::string course_name;

        if (rank == MASTER) {
            auto& course = available_courses[i];
            course_info = {
                    course.get_id(),
                    course.get_credits(),
                    course.get_name().size(),
            };
        }

        // Boradcast is the same for both
        MPI_Bcast(&course_info, 1, COURSE_INFO_MPI, MASTER, MPI_COMM_WORLD);

        // Send the name as a separate message

        if (rank == MASTER) {
            // Just send the string's buffer
            auto& name = available_courses[i].get_name();
            MPI_Bcast(
                    // Safe to cast away const since it is just sending here
                    (void*)name.data(),
                    course_info.name_size,
                    MPI_CHAR,
                    MASTER,
                    MPI_COMM_WORLD);
        } else {
            // TODO Allocate this once rather than re-allocating for each course
            char* name_buf = new char[course_info.name_size];
            MPI_Bcast(
                    name_buf,
                    course_info.name_size,
                    MPI_CHAR,
                    MASTER,
                    MPI_COMM_WORLD);
            course_name = std::string(name_buf, course_info.name_size);
            delete[] name_buf;

            available_courses.emplace_back(
                    course_info.id, course_name, course_info.credits);
        }
    }
}

// Receive Each Course -> Could not get it to work with Broadcast_courses()
// Will Try to refactor below once I can get the Student's Courses to be sent
void Node::broadcast_student_list() {
    std::size_t student_list_size;

    if (rank == MASTER) {
        student_list_size = students.size();
    }

    // Send how many students to expect
    MPI_Bcast(&student_list_size, 1, SIZE_T_MPI, MASTER, MPI_COMM_WORLD);

    for (std::size_t i = 0; i < student_list_size; i++) {
        StudentInfo student_info;
        std::vector<Course::Ref> taken_course_list;
        std::vector<Course::Ref> wanted_course_list;

        if (rank == MASTER) {
            student_info.id = students[i].id;
            student_info.taken_courses_size = students[i].taken_courses.size();
            student_info.wanted_courses_size =
                    students[i].wanted_courses.size();
        }

        // Broadcast Student ID and how many courses to expect
        MPI_Bcast(&student_info, 1, STUDENT_INFO_MPI, MASTER, MPI_COMM_WORLD);

        // Send taken courses
        for (std::size_t j = 0; j < student_info.taken_courses_size; j++) {
            // Only send the index of the course
            std::size_t offset;
            if (rank == MASTER) {
                offset = students[i].taken_courses[j].get_offset();
            }

            // Broadcast offset of taken course
            MPI_Bcast(&offset, 1, SIZE_T_MPI, MASTER, MPI_COMM_WORLD);

            if (rank != MASTER) {
                taken_course_list.push_back(make_course_ref(offset));
            }
        }

        // Send wanted courses
        for (std::size_t j = 0; j < student_info.wanted_courses_size; j++) {
            // Only send the index of the course
            std::size_t offset;
            if (rank == MASTER) {
                offset = students[i].wanted_courses[j].get_offset();
            }

            // Broadcast Credits, Size of Course Name, and the Name of Course
            MPI_Bcast(&offset, 1, SIZE_T_MPI, MASTER, MPI_COMM_WORLD);

            if (rank != MASTER) {
                wanted_course_list.push_back(make_course_ref(offset));
            }
        }

        if (rank != MASTER) {
            students.emplace_back(
                    student_info.id, taken_course_list, wanted_course_list);
        }
    }

    if (rank == 1) {
        print_all_students_courses(students);
    }
}

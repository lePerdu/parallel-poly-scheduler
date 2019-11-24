#include "node.hpp"

/*
 *===================== TODO ========================
 * 1. broadcast_student_list();
 * Correctly save the courses to the student
 * Possible corruption / overwritten from
 * pointer in std::vector<Course::Ref> taken_courses) ?
 *
 * 2. Refactor broadcast_student_list() to
 * incorporate broadcasst_course_list() when sending
 * student's courses,
 *
 * Possible fixes:
 * 1. Change taken_courses from vector<Course::Ref>
 * to vector<const Course>
 *
 * 2. --
 * ===================================================
 */

Node::Node(std::uint8_t rank) {
    this->rank = rank;
}

void Node::start_node_work() {
    // Extract courses from JSON File and save as a vector of course pointers
    if (rank == MASTER)
        setup(available_courses, course_pointers);

    broadcast_course_list();

    broadcast_student_list();
}

void Node::broadcast_course_list() {
    // Send how long the course list is
    std::size_t course_list_size;
    if (rank == MASTER)
        course_list_size = course_pointers.size();
    MPI_Bcast(&course_list_size, 1, MPI_UNSIGNED_LONG, MASTER, MPI_COMM_WORLD);

    // Iterate through the course list
    for (std::size_t i = 0; i < course_list_size; i++) {
        std::string course_name = "";
        std::uint8_t credits;
        std::size_t size_of_course_name;

        if (rank == MASTER) {
            course_name = course_pointers[i]->get_name();
            credits = course_pointers[i]->get_credits();
            size_of_course_name = course_name.length() + 1;
            printf("[MASTER] ---- Sending Course: %s\tCredits: %d\n",
                   course_name.c_str(),
                   credits);
        }

        // Broadcast to all nodes..
        //      Credits, Size of Course Name, and the Name of Course
        MPI_Bcast(&credits, 1, MPI_UNSIGNED_CHAR, MASTER, MPI_COMM_WORLD);
        MPI_Bcast(
                &size_of_course_name,
                1,
                MPI_UNSIGNED_LONG,
                MASTER,
                MPI_COMM_WORLD);
        for (std::size_t j = 0; j < size_of_course_name; j++) {
            char char_name;
            if (rank == MASTER)
                char_name = course_name[j];

            MPI_Bcast(&char_name, 1, MPI_CHAR, MASTER, MPI_COMM_WORLD);

            if (rank != MASTER)
                course_name += char_name;
        }

        if (rank == MASTER) {
            MPI_Bcast(
                    &course_name[0],
                    size_of_course_name,
                    MPI_CHAR,
                    MASTER,
                    MPI_COMM_WORLD);
        } else {
            // Allocate a temporary buffer to read the name into
            char* buffer = new char[size_of_course_name];
            MPI_Bcast(
                    buffer,
                    size_of_course_name,
                    MPI_CHAR,
                    MASTER,
                    MPI_COMM_WORLD);
            course_name = buffer;
            delete[] buffer;
        }

        if (rank != MASTER) {
            available_courses.emplace_back(course_name, credits);
            // Address of the last one pushed
            course_pointers.emplace_back(
                    available_courses.data(), available_courses.size() - 1);
        }
    }

    if (rank == 1) {
        for (std::size_t i = 0; i < course_pointers.size(); i++) {
            // Will output strange chars at the front of the course_pointers but
            // correct at the end... Not sure what is up with this? -> non
            // pointers work
            // course_pointers[i]->print_course();
            available_courses[i].print_course();
        }
    }
}

// Receive Each Course -> Could not get it to work with Broadcast_courses()
// Will Try to refactor below once I can get the Student's Courses to be sent
void Node::broadcast_student_list() {
    std::size_t student_list_size;

    if (rank == MASTER) {
        students = generate_random_students(TOTALSTUDENTS, course_pointers);
        student_list_size = students.size();
        printf("Student list size: %ld\n", student_list_size);
    }
    // Send how many students to expect
    MPI_Bcast(&student_list_size, 1, MPI_UNSIGNED_LONG, MASTER, MPI_COMM_WORLD);

    for (std::size_t i = 0; i < student_list_size; i++) {
        std::uint16_t student_id;
        std::size_t taken_courses_list_size;
        std::vector<Course::Ref> taken_course_list;
        std::vector<Course::Ref>* taken_course_list_ptr = &taken_course_list;

        if (rank == MASTER) {
            student_id = students[i].student_id;
            taken_course_list_ptr = &students[i].taken_courses;
            taken_courses_list_size = taken_course_list_ptr->size();
        }

        // Broadcast Student ID and how many Courses to expect
        MPI_Bcast(&student_id, 1, MPI_UNSIGNED_SHORT, MASTER, MPI_COMM_WORLD);
        MPI_Bcast(
                &taken_courses_list_size,
                1,
                MPI_UNSIGNED_LONG,
                MASTER,
                MPI_COMM_WORLD);

        // Part to be refactored later -> using broadcast_course();
        for (std::size_t j = 0; j < taken_courses_list_size; j++) {
            // Only send the index of the course
            unsigned long offset; // Should be equivalent to size_t
            if (rank == MASTER) {
                const auto& course = students[i].taken_courses[j];
                auto& course_name = course->get_name();
                auto credits = course->get_credits();
                printf("[MASTER] ---- Sending Course From Student: "
                       "%d\t%s\tCredits: %d\n",
                       student_id,
                       course_name.c_str(),
                       credits);

                offset = course.get_offset();
            }

            // Broadcast Credits, Size of Course Name, and the Name of Course
            MPI_Bcast(&offset, 1, MPI_UNSIGNED_LONG, MASTER, MPI_COMM_WORLD);

            if (rank != MASTER) {
                taken_course_list.emplace_back(
                        available_courses.data(), offset);

                if (rank == 1) {
                    const auto& course = taken_course_list.back();
                    printf("[WORKER %d] ---- Adding Course To Student: "
                           "%d\t%s\tCredits: %d\n",
                           rank,
                           student_id,
                           course->get_name().c_str(),
                           course->get_credits());
                }
            }
        }

        // problem here? Probably storing
        if (rank == 1)
            print_all_students_courses(students);
    }
}

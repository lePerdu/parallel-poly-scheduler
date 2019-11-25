#include "node.hpp"

#include "mpi_types.hpp"

#include <algorithm>

constexpr unsigned CLASS_CAPACITY = 30;

enum : int {
    SECTION_INFO_TAG,
    SCHEDULE_INFO_TAG,
    CLASS_TIMES_TAG,
};

Node::Node(int rank) {
    this->rank = rank;
}

std::ostream& Node::logstream() const {
    if (rank == MASTER) {
        std::cout << "[MASTER] ";
    } else {
        std::cout << "[WORKER " << rank << "] ";
    }

    return std::cout;
}

void Node::start_node_work() {
    // Extract courses from JSON File and save as a vector of course pointers
    if (rank == MASTER)
        setup(available_courses, students);

    broadcast_course_list();
    broadcast_student_list();

    if (rank == MASTER) {
        logstream() << "Done broadasting courses and students" << std::endl;
    }

    /* Have each node generate a random schedule and send it to master */
    if (rank != MASTER) {
        Schedule schedule(
                8 * 2,
                18 * 2,
                required_course_counts(CLASS_CAPACITY, students));

        const float score = schedule.fitness_score(students);
        send_schedule(schedule, score, MASTER);
    } else {
        int node_count;
        MPI_Comm_size(MPI_COMM_WORLD, &node_count);

        std::vector<std::pair<Schedule, float>> population;
        for (int node = 1; node < node_count; ++node) {
            population.push_back(recv_schedule(node));
        }

        for (std::size_t i = 0; i < population.size(); ++i) {
            logstream() << "Node: " << (i + 1)
                        << "\tScore: " << population[i].second << std::endl;
        }

        // Sort with the best at the front
        std::sort(
                population.begin(),
                population.end(),
                [](std::pair<Schedule, float> a, std::pair<Schedule, float> b) {
                    return a.second > b.second;
                });

        population[0].first.print();
        std::cout << "Score: " << population[0].second << std::endl;
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
}

void Node::send_section(const Section& section, int dest) const {
    auto& layout_times = section.layout.get_times();
    const SectionInfo info{section.course.get_offset(), {layout_times.size()}};

    MPI_Ssend(
            &info, 1, SECTION_INFO_MPI, dest, SECTION_INFO_TAG, MPI_COMM_WORLD);

    MPI_Ssend(
            layout_times.data(),
            layout_times.size(),
            CLASS_TIME_MPI,
            dest,
            CLASS_TIMES_TAG,
            MPI_COMM_WORLD);
}

Section Node::recv_section(int source) const {
    // TODO Actually check this
    MPI_Status status;
    SectionInfo info;

    MPI_Recv(
            &info,
            1,
            SECTION_INFO_MPI,
            source,
            SECTION_INFO_TAG,
            MPI_COMM_WORLD,
            &status);

    std::vector<ClassTime> times(info.layout_info.times_size);
    MPI_Recv(
            times.data(),
            times.size(),
            CLASS_TIME_MPI,
            source,
            CLASS_TIMES_TAG,
            MPI_COMM_WORLD,
            &status);

    return {make_course_ref(info.course_index), times};
}

void Node::send_schedule(
        const Schedule& schedule, float score, int dest) const {
    auto& sections = schedule.get_sections();
    const ScheduleInfo info{schedule.get_start_time(),
                            schedule.get_end_time(),
                            score,
                            sections.size()};

    MPI_Ssend(
            &info,
            1,
            SCHEDULE_INFO_MPI,
            dest,
            SCHEDULE_INFO_TAG,
            MPI_COMM_WORLD);

    for (auto& sect : sections) {
        // Each section stores which course it is for, so we don't need to send
        // it
        send_section(sect.second, dest);
    }
}

std::pair<Schedule, float> Node::recv_schedule(int source) const {
    MPI_Status status; // TODO Actually check the status
    ScheduleInfo info;

    MPI_Recv(
            &info,
            1,
            SCHEDULE_INFO_MPI,
            source,
            SCHEDULE_INFO_TAG,
            MPI_COMM_WORLD,
            &status);

    Schedule schedule(info.start_time, info.end_time);

    for (std::size_t i = 0; i < info.sections_size; ++i) {
        auto sect = recv_section(source);
        schedule.add_section(sect);
    }

    return {schedule, info.score};
}

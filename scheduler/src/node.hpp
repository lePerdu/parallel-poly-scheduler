#ifndef NODES_HPP_
#define NODES_HPP_

#include "course.hpp"
#include "schedule.hpp"
#include "setup.hpp"
#include "student.hpp"

#include <iostream>
#include <mpi.h>
#include <string>
#include <vector>

/*
 * First Setups the master node by running the Setup function in
 * Setup.hpp. This will fill the course_pointers list and the
 * students lists, which will both be broadcasted to all other nodes
 * in the MPI::COMM_WORLD
 */

#define MASTER 0
#define TOTALSTUDENTS 10

class Node {
private:
    int rank;
    std::vector<Course> available_courses;
    std::vector<Student> students;

    Course::Ref make_course_ref(std::size_t index) const;

    void broadcast_course_list();
    void broadcast_student_list();

    void send_section(const Section& section, int dest) const;
    Section recv_section(int source) const;

    void send_schedule(const Schedule& schedule, float score, int dest) const;
    std::pair<Schedule, float> recv_schedule(int source) const;

    std::ostream& logstream() const;

public:
    Node(int rank);
    void start_node_work();
};

#endif // NODE_HPP_

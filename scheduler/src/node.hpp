#ifndef NODES_HPP_
#define NODES_HPP_

#include "course.hpp"
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
    std::uint8_t rank;
    std::vector<Student> students;
    std::vector<Course::Ref> course_pointers;
    std::vector<Course> available_courses;

    void broadcast_course_list();
    void broadcast_student_list();

public:
    Node(std::uint8_t rank);
    void start_node_work();
};

#endif // NODE_HPP_

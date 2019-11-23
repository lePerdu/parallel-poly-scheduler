#ifndef NODES_HPP_
#define NODES_HPP_

#include <iostream>
#include <mpi.h>
#include "course.hpp"
#include "student.hpp"
#include "setup.hpp"
#include <vector>
#include <string>

/*
 * First Setups the master node by running the Setup function in
 * Setup.hpp. This will fill the course_pointers list and the 
 * students lists, which will both be broadcasted to all other nodes
 * in the MPI::COMM_WORLD
 */

#define MASTER 0
#define TOTALSTUDENTS 10

class Node{
private:
    std::uint8_t rank;
    std::vector<Student> students;
    std::vector<const Course*> course_pointers;
    std::vector<Course> available_courses;

    void broadcast_course_list(std::vector<const Course*>& course_ptr);
    void broadcast_student_list();

public:
    Node(std::uint8_t rank);
    void start_node_work();
};

#endif  // NODE_HPP_
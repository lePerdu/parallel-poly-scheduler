#include "node.hpp"

/*
 *===================== TODO ========================
 * 1. broadcast_student_list();
 * Correctly save the courses to the student
 * Possible corruption / overwritten from 
 * pointer in std::vector<const Course*> taken_courses) ? 
 * 
 * 2. Refactor broadcast_student_list() to 
 * incorporate broadcasst_course_list() when sending
 * student's courses,
 * 
 * Possible fixes:
 * 1. Change taken_courses from vector<const Course*>
 * to vector<const Course>
 * 
 * 2. -- 
 * ===================================================
 */

Node::Node(std::uint8_t rank){
    this->rank = rank;
}

void Node::start_node_work(){
    // Extract Courses from JSON File and save as a vector of course pointers
    if (rank == MASTER)
        Setup(available_courses, course_pointers);
    
    broadcast_course_list(course_pointers);

    MPI_Barrier(MPI::COMM_WORLD);
    broadcast_student_list();
}

void Node::broadcast_course_list(std::vector<const Course*>& course_ptr){
    // Send how long the course list is
    int course_list_size;
    if (rank == MASTER)
         course_list_size = course_ptr.size();
    MPI_Bcast(&course_list_size, 1, MPI_INT, MASTER, MPI::COMM_WORLD);
    
    // Iterate through the course list
    for (int i = 0; i < course_list_size; i++){
        std::string course_name = "";
        std::uint8_t credits, size_of_course_name;

        if (rank == MASTER){
            course_name = course_ptr.at(i)->get_name();
            credits = course_ptr.at(i)->get_credits();
            size_of_course_name = course_name.length();
            printf("[MASTER] ---- Sending Course: %s\tCredits: %d\n", course_name.c_str(), credits);
        }

        // Broadcast to all nodes..
        //      Credits, Size of Course Name, and the Name of Course
        MPI_Bcast(&credits, 1, MPI_INT, MASTER, MPI::COMM_WORLD);
        MPI_Barrier(MPI::COMM_WORLD);
        MPI_Bcast(&size_of_course_name, 1, MPI_INT, MASTER, MPI::COMM_WORLD);
        MPI_Barrier(MPI::COMM_WORLD);
        for (int j = 0; j < size_of_course_name; j++){
            char char_name;
            if (rank == MASTER)
                char_name = course_name[j];

            MPI_Bcast(&char_name, 1, MPI_CHAR, MASTER, MPI::COMM_WORLD);

            if (rank != MASTER)
                course_name += char_name;

            MPI_Barrier(MPI::COMM_WORLD);
        }

        if (rank != MASTER){
            const Course recv_course(course_name, credits);
            course_ptr.push_back(&recv_course);
            available_courses.push_back(recv_course);
        }
    }

    if (rank == 1){
        for (int i = 0; i < course_ptr.size(); i++){
            // Will output strange chars at the front of the course_ptr but correct at the end... 
            // Not sure what is up with this? -> non pointers work 
            //course_ptr.at(i)->print_course();
            available_courses.at(i).print_course();
        }
    }
}

// Receive Each Course -> Could not get it to work with Broadcast_courses()
// Will Try to refactor below once I can get the Student's Courses to be sent
void Node::broadcast_student_list(){
    std::uint16_t student_list_size;

    if (rank == MASTER){
        students = generate_random_students(TOTALSTUDENTS, course_pointers);
        student_list_size = students.size();
        printf("Student list size: %d\n", student_list_size);
    }
    // Send how many students to expect
    MPI_Bcast(&student_list_size, 1, MPI_INT, MASTER, MPI::COMM_WORLD);
    MPI_Barrier(MPI::COMM_WORLD);

    for (int i = 0; i < student_list_size; i++){
        std::uint16_t student_id, taken_courses_list_size;
        std::vector<const Course*> taken_course_list;
        std::vector<const Course*>* taken_course_list_ptr = &taken_course_list;

        if (rank == MASTER){
            student_id = students.at(i).student_id;
            taken_course_list_ptr = &students.at(i).taken_courses;
            taken_courses_list_size = taken_course_list_ptr->size();
        }

        // Broadcast Student ID and how many Courses to expect
        MPI_Bcast(&student_id, 10, MPI_INT, MASTER, MPI::COMM_WORLD);
        MPI_Bcast(&taken_courses_list_size, 1, MPI_INT, MASTER, MPI::COMM_WORLD);

        // Part to be refactored later -> using broadcast_course();
        for (int j = 0; j < taken_courses_list_size; j++){
            std::string course_name = "";
            std::uint8_t credits = 0;
            std::uint8_t size_of_course_name = 0;

            if (rank == MASTER){
                course_name = students.at(i).taken_courses.at(j)->get_name();
                credits = students.at(i).taken_courses.at(j)->get_credits();
                size_of_course_name = course_name.length();
                printf("[MASTER] ---- Sending Course From Student: %d\t%s\tCredits: %d\n", student_id, course_name.c_str(), credits);
            }

            // Broadcast Credits, Size of Course Name, and the Name of Course
            MPI_Bcast(&credits, 1, MPI_INT, MASTER, MPI::COMM_WORLD);
            MPI_Bcast(&size_of_course_name, 1, MPI_INT, MASTER, MPI::COMM_WORLD);

            for (int k = 0; k < size_of_course_name; k++){
                char char_name;
                if (rank == MASTER)
                    char_name = course_name[k];

                MPI_Bcast(&char_name, 1, MPI_CHAR, MASTER, MPI::COMM_WORLD);

                if (rank != MASTER)
                    course_name += char_name;

                MPI_Barrier(MPI::COMM_WORLD);
            }

            if (rank != MASTER){
                const Course recv_course(course_name, credits);
                taken_course_list.push_back(&recv_course);

                if (rank == 1){
                    printf("[WORKER %d] ---- Adding Course To Student: %d\t%s\tCredits: %d\n", rank, student_id, course_name.c_str(), credits);
                    //recv_course.print_course();
                }
            }
        }

        if (rank != MASTER){
            Student recv_student(student_id, taken_course_list);
            students.push_back(recv_student);
            //printf("[WORKER %d] Adding Student -- Student ID: %d\n", rank, recv_student.student_id);
            //for (int j = 0; j < recv_student.taken_courses.size(); j++)
            //   recv_student.taken_courses.at(j)->print_course();
        }

        // problem here? Probably storing 
        if (rank == 1)
            print_all_students_courses(students);
        MPI_Barrier(MPI::COMM_WORLD);
    }
}
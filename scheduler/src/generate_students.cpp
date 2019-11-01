#include "generate_students.hpp"

GenerateStudents::GenerateStudents(const std::uint16_t total_students, std::vector<Course>& available_courses){
    GenerateStudents::total_students = total_students;

    // Create a list of all students based on the inputted number of students
    for (std::uint16_t i = 0; i < total_students; i++){
        Student created_student;
        created_student.student_id = i;
        generate_taken_courses(available_courses, created_student);
        students_list.emplace_back(created_student);
    }
}

// Randomly assigns taken courses for a student given a list of avilable courses
void GenerateStudents::generate_taken_courses(std::vector<Course>& available_courses, Student& created_student){
    created_student.taken_credits = 0;
    
    /* Random Number Generator that generates a better distribution than srand */
    std::random_device dev;
    std::mt19937 rng(dev());

    // Pick a random value between (0 and the total number of credits possible to achieve a degree)
    std::uniform_int_distribution<std::mt19937::result_type> randomly_assigned_max_credits(0, TOTAL_CREDITS_POSSIBLE);
    uint16_t max_assigned_credits = randomly_assigned_max_credits(rng);
    
    // Pick a random value between (0 and the total number of available courses) -- the picked number will correlate to the chosen course
    std::uniform_int_distribution<std::mt19937::result_type> randomly_assigned_course(0, available_courses.size()-1);

    // Keep generating courses until the number of taken credits is <= max that was assigned
    // Does not currently look at if the prereqs were met before assigning
    while (created_student.taken_credits <= max_assigned_credits){
        Course chosen_course = available_courses.at(randomly_assigned_course(rng));

        // Check to  see if the newly picked course was already assigned to the student
        bool already_assigned = false;
        for (int i = 0; i < created_student.taken_courses.size(); i++){
            if (created_student.taken_courses.at(i).get_name() == chosen_course.get_name()){
                already_assigned = true;
                break;
            }
        }

        // The class was not previously assigned .. add it to the student's courses
        if (!already_assigned){
            created_student.taken_credits += chosen_course.get_credits();
            created_student.taken_courses.emplace_back(chosen_course);
        }
    }
}

std::vector<Student>& GenerateStudents::get_student_list(){
    return students_list;
}

// Prints all students along with their generated courses
void GenerateStudents::print_all_students_courses(){
    for (std::uint16_t i = 0; i < students_list.size(); i++){
        Student printing_student = students_list.at(i);
        printf("Student Id: [%u] -- Total Credits: [%u]\n", printing_student.student_id, printing_student.taken_credits);
        for (int j = 0; j < printing_student.taken_courses.size(); j++)
            printing_student.taken_courses.at(j).print_course();
        printf("\n");
    }
}

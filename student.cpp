#include "student.hpp"

Student::Student(const std::uint16_t student_id):
    student_id(student_id){}

// Adds a course for the student
void Student::add_taken_course(const Course& course){
    taken_student_courses.emplace_back(course);
}

// Gets all the courses for the student
std::vector<Course>& Student::get_taken_courses(){
    return taken_student_courses;
}

// Randomly assigns taken courses for a student given a list of avilable courses
void Student::generate_taken_courses(std::vector<Course>& available_courses){
    std::uint8_t total_credits_possible = 120;
    taken_credits = 0;
    
    /* Random Number Generator that generates a better distribution than srand */
    std::random_device dev;
    std::mt19937 rng(dev());

    // Pick a random value between (0 and the total number of credits possible to achieve a degree)
    std::uniform_int_distribution<std::mt19937::result_type> randomly_assigned_max_credits(0, total_credits_possible);
    uint16_t max_assigned_credits = randomly_assigned_max_credits(rng);
    
    // Pick a random value between (0 and the total number of available courses) -- the picked number will correlate to the chosen course
    std::uniform_int_distribution<std::mt19937::result_type> randomly_assigned_course(0, available_courses.size()-1);

    std::vector<Course> already_assigned_courses;

    // Keep generating courses until the number of taken credits is <= max that was assigned
    // Does not currently look at if the prereqs were met before assigning
    while (taken_credits <= max_assigned_credits){
        Course chosen_course = available_courses.at(randomly_assigned_course(rng));
        
        // Check to  see if the newly picked course was already assigned to the student
        bool already_assigned = false;
        for (int i = 0; i < already_assigned_courses.size(); i++){
            if (already_assigned_courses.at(i).get_name() == chosen_course.get_name()){
                already_assigned = true;
                break;
            }
        }

        // The class was not previously assigned .. add it to the student's courses
        if (!already_assigned){
            add_taken_course(chosen_course);
            taken_credits += chosen_course.get_credits();
            //printf("Taken credits: [%u]\n", taken_credits);
        }
    }
}

// Prints all taken courses for the student
void Student::print_taken_courses(){
    printf("Student Id: [%u] -- Total Credits: [%u]\n", student_id, taken_credits);
    for (int i = 0; i < taken_student_courses.size(); i++)
        taken_student_courses.at(i).print_course();
    printf("\n");
}

#ifndef SETUP_HPP_
#define SETUP_HPP_

#include <iostream>
#include <vector>
#include <string>
#include <mpi.h>

#include "parse_json.cpp"
#include "student.cpp"
#include "course.cpp"

#define TOTALSTUDENTS 10

void Setup(std::vector<Course>& available_courses, std::vector<const Course*>& course_pointers);
void Extract_Courses(std::vector<Course>& available_courses, std::vector<const Course*>& course_pointers);

//std::vector<Student> students;

/*
 * Parses the courses from the list of courses that a
 * CS major student must take && Generates a list of
 * students with the courses that they have already taken
 */

void Setup(std::vector<Course>& available_courses, std::vector<const Course*>& course_pointers){
  Extract_Courses(available_courses, course_pointers);
  //students = generate_random_students(TOTALSTUDENTS, course_pointers);
}

void Extract_Courses(std::vector<Course>& available_courses, std::vector<const Course*>& course_pointers){
  std::string FILE_NAME = "cs_major_courses.json";
  std::cout << "Extracting Needed Course List From " << FILE_NAME << std::endl;
  ParseJson parsed_list(FILE_NAME);
  available_courses = parsed_list.get_course_list();
  for (const auto& c : available_courses)
      course_pointers.push_back(&c);
  std::cout << "Done Extracting Course List" << std::endl;
}


#endif // SETUP_HPP_
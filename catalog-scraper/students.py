"""Generate random student data for testing purposes."""

import json
import random
import sys
import pprint

SEMESTER_COURSE_BOUNDS = (4, 6)
DEGREE_CREDITS = 120


class Student:
    def __init__(self, student_id):
        self.student_id = student_id
        self.taken_courses = set()
        self.wanted_courses = set()

        self.taken_credits = 0

    def take_courses(self, ids, all_courses):
        for cid in ids:
            self.taken_courses.add(cid)
            self.taken_credits += all_courses[cid]['credits']

    def choose_semester_courses(self, all_courses):
        possible = [
            cid for cid, course in all_courses.items() \
                if self.can_take(cid, course)
        ]

        course_count = random.randint(*SEMESTER_COURSE_BOUNDS)
        course_count = min(course_count, len(possible))
        return random.sample(possible, course_count)

    def can_take(self, cid, course):
        return (cid not in self.taken_courses) and self.has_prereqs(course)

    def has_prereqs(self, course):
        return all((p in self.taken_courses) for p in course['prereqs'])

    def as_dict(self):
        # Don't output taken_credits
        return {
            'id': self.student_id,
            'taken_courses': list(self.taken_courses),
            'wanted_courses': list(self.wanted_courses),
        }


def generate_student(student_id, all_courses):
    student = Student(student_id)

    target_credits = random.randint(0, DEGREE_CREDITS)
    while student.taken_credits < target_credits:
        courses = student.choose_semester_courses(all_courses)
        if len(courses) > 0:
            student.take_courses(courses, all_courses)
        else:
            break

    student.wanted_courses = student.choose_semester_courses(all_courses)

    return student


def generate_students(n, all_courses):
    return [generate_student(i, all_courses) for i in range(1, n+1)]


def main():
    student_count = int(sys.argv[1])
    course_file = sys.argv[2]

    with open(course_file) as f:
        all_courses = json.load(f)

    # Convert indices from strings to ints
    all_courses = {int(k): v for k, v in all_courses.items()}

    students = generate_students(student_count, all_courses)
    students_output = [s.as_dict() for s in students]
    pprint.pprint(students_output)

    if len(sys.argv) > 3:
        with open(sys.argv[3], 'w') as f:
            json.dump(students_output, f)


if __name__ == '__main__':
    main()

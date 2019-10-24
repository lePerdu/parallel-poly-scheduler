# parallel-poly-scheduler
Course scheduler for Fall 2019 Intro to Parallel and Distributed Computing,
Group "High" 5

## Catalog Scraper

This is a web scraper written in Python which pulls course data from the
school's catalog. See the README in the subdirectory for more information.

## Parallel Scheduler

This is a parallel schedule generator using the MPI library. The plan in to use
a generic algorithm with a fitness function based on how many classes each
student can take at once in a given schedule.

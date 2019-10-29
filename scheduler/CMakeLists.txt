# CMakeLists.txt

cmake_minimum_required(VERSION 3.15)

project(parallel_scheduler VERSION 0.0.0 LANGUAGES CXX)

file(GLOB SOURCES "src/*.cpp")

include_directories(src)
add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# MPI

find_package(MPI REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE MPI::MPI_CXX)

# target_include_directories(${PROJECT_NAME} PRIVATE ${MPI_CXX_INCLUDE_DIRS})
# target_compile_options(${PROJECT_NAME} PRIVATE
#     ${MPI_CXX_COMPILE_OPTIONS})
# target_compile_definitions(${PROJECT_NAME} PRIVATE
#     ${MPI_CXX_COMPILE_DEFINITIONS})
# target_link_libraries(${PROJECT_NAME} PRIVATE
#     ${MPI_CXX_LIBRARIES})
# target_link_options(${PROJECT_NAME} PRIVATE
#     ${MPI_CXX_LINK_FLAGS})

# nlohmann_json ilbrary

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

cmake_minimum_required(VERSION 3.9.0)
set(CMAKE_CXX_STANDARD 17)
project (tests)
find_package(Boost 1.67.0 COMPONENTS system REQUIRED)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
add_executable(tests
    ../src/data/list/linked_list.cpp
    ../src/data/queue/functional_queue.cpp
    ../src/data/data.cpp
    test.cpp)
target_compile_options(tests PUBLIC -fext-numeric-literals)

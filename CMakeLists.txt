cmake_minimum_required(VERSION 3.10)
project(computorv2)

# set C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# set flags
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

# find Boost
find_package(Boost)

add_subdirectory(source)
add_subdirectory(test)
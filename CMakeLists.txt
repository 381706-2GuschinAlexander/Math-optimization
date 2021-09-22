cmake_minimum_required( VERSION 3.9 )

project(Math-optimization)

# include_directories("${CMAKE_CURRENT_SOURCE_DIR}/gtest")
# add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/gtest")

#file(GLOB SUBLIBS "${CMAKE_CURRENT_SOURCE_DIR}/src/*lib")

#set(MATPLOTCPP D:/Git/matplotlib-cpp)
include_directories($ENV{MATPLOTCPP})

file(GLOB SUBDIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/*")

foreach(subdir ${SUBDIRS})
  add_subdirectory(${subdir})
endforeach()
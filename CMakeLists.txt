cmake_minimum_required(VERSION 3.15)
project("Math-optimization" LANGUAGES CXX)

# guard against in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_CXX_FLAGS_DISTRIBUTION "-O3")
SET(CMAKE_C_FLAGS_DISTRIBUTION "-O3")


# find python libraries
find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
find_package(PythonLibs 3.0 REQUIRED)
include_directories(${PYTHON3_INCLUDE_DIRS} ${NumPy_INCLUDE_DIRS})

include(FetchContent)
FetchContent_Declare(
    matplotlib
    GIT_REPOSITORY https://github.com/lava/matplotlib-cpp.git
    GIT_TAG        f23347fca25219d1c42cbb91608b5556814bf572
)
FetchContent_Declare(
    parser
    URL http://warp.povusers.org/FunctionParser/fparser4.5.2.zip
)

FetchContent_GetProperties(matplotlib)
if(NOT matplotlib_POPULATED)
    FetchContent_Populate(matplotlib)
endif()
include_directories(SYSTEM ${matplotlib_SOURCE_DIR})

FetchContent_GetProperties(parser)
if(NOT parser_POPULATED)
    FetchContent_Populate(parser)
endif()
include_directories(${parser_SOURCE_DIR})
add_library(parserlib ${parser_SOURCE_DIR}/fparser.cc ${parser_SOURCE_DIR}/fpoptimizer.cc)
set_target_properties(parserlib PROPERTIES LINKER_LANGUAGE CXX)


find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)


# add executable
add_library(mathlib STATIC ${CMAKE_CURENT_SOURCE_DIR}src/mathlib/mathlib.cpp)
target_link_libraries(mathlib
    PUBLIC
        ${PYTHON_LIBRARIES}
        Python3::NumPy
        parserlib
)

include_directories(${CMAKE_CURENT_SOURCE_DIR}src/mathlib)
add_executable(main ${CMAKE_CURENT_SOURCE_DIR}src/main/main.cpp)
target_link_libraries(main
    PRIVATE
        mathlib
        ${GTKMM_LIBRARIES}
)

target_include_directories(main PRIVATE ${GTKMM_INCLUDE_DIRS})
target_link_directories(main PRIVATE ${GTKMM_LIBRARY_DIRS})

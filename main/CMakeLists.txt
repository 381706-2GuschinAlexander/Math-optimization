find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

find_package(OpenMP REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main
    PRIVATE
        mathlib
        beam
        ${GTKMM_LIBRARIES}
)

target_include_directories(main PRIVATE 
    ${GTKMM_INCLUDE_DIRS} 
    ${CMAKE_SOURCE_DIR}/lib/mathlib/include/
    ${CMAKE_SOURCE_DIR}/lib/beam/include/ )

target_link_directories(main PRIVATE ${GTKMM_LIBRARY_DIRS})
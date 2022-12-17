add_subdirectory(extra)
add_subdirectory(mathlib)

target_include_directories(mathlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/extra/include)
target_link_libraries(mathlib PUBLIC extra)
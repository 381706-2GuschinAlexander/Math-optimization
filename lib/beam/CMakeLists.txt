add_library(beam STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/beam.cpp)
target_include_directories(beam PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
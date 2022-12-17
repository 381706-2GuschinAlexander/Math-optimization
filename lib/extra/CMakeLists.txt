add_library(extra STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/iterator.cpp)
target_include_directories(extra PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)



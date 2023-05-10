add_library(mathlib STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/mathlib.cpp)
target_include_directories(mathlib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_compile_options(mathlib PUBLIC -O3)
target_compile_features(mathlib PUBLIC cxx_std_17)
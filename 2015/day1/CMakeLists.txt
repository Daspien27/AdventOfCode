cmake_minimum_required(VERSION 3.0.0)
project(2015d03p1 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(2015d01p1 2015d01p1.cpp)
target_compile_features(2015d01p1 PRIVATE cxx_std_17)

add_executable(2015d01p2 2015d01p2.cpp)
target_compile_features(2015d01p2 PRIVATE cxx_std_17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

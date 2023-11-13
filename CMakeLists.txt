cmake_minimum_required(VERSION 3.5.0)
project(worldCounter VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory("tests")

include(CTest)
enable_testing()

add_executable(wordCounter main.cpp)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERPSION})
include(CPack)

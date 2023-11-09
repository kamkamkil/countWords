cmake_minimum_required(VERSION 3.0.0)
project(worldCounter VERSION 0.1.0 LANGUAGES C CXX)
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)

include(CTest)
enable_testing()

add_executable(worldCounter main.cpp)

add_executable(tests tests/test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERPSION})
include(CPack)

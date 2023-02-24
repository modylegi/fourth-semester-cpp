cmake_minimum_required(VERSION 3.0.0)
project(cpp VERSION 0.1.0)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_executable(cpp hw01.cpp)
find_package(Catch2)
target_link_libraries(${PROJECT_NAME} Catch2::Catch2WithMain)

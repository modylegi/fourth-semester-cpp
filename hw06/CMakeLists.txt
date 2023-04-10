cmake_minimum_required(VERSION 3.5)
project(hw06 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)


add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} Threads::Threads)


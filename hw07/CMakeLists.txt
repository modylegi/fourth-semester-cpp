cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
project(hw07)

find_package(Boost REQUIRED COMPONENTS filesystem )
add_executable(hw07 hw07.cpp)


target_link_libraries(hw07 ${Boost_SYSTEM_LIBRARY} Boost::filesystem)
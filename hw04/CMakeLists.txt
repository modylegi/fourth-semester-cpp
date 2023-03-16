cmake_minimum_required(VERSION 3.5)
project(boost_fs_tutorials LANGUAGES CXX)
find_package(Boost REQUIRED COMPONENTS filesystem program_options)

add_executable(hw04 hw04.cpp)
target_link_libraries(hw04 Boost::filesystem)
target_link_libraries(hw04 Boost::program_options)
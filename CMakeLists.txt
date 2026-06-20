cmake_minimum_required(VERSION 3.16)

project(own_htop)

set(CMAKE_CXX_STANDARD 17)

add_executable(mini_htop
src/main.cpp
src/data_manipulation.cpp)

target_include_directories(mini_htop
PRIVATE
header)
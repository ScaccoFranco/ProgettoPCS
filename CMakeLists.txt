cmake_minimum_required(VERSION 3.20)
project(progetto)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla -Wno-sign-compare")

find_package(Eigen3 REQUIRED)

# eseguibile principale
set(SRC_main main.cpp)
add_executable(progetto ${SRC_main})
target_link_libraries(progetto Eigen3::Eigen)

# eseguibile eigen
set(SRC_main main_eigen.cpp)
add_executable(progetto_eigen ${SRC_main})
target_link_libraries(progetto_eigen Eigen3::Eigen)


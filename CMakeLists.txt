cmake_minimum_required(VERSION 3.20)
project(progetto)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla -Wno-sign-compare")

find_package(Eigen3 REQUIRED)

# eseguibile principale
#set(SRC_main main.cpp)
#add_executable(progetto ${SRC_main})
#target_link_libraries(progetto Eigen3::Eigen)


# eseguibile test
set(SRC_test test/test_cicli_dfs.cpp include/algoritmi_grafi.hpp)
add_executable(test_cicli_dfs ${SRC_test})
add_test(NAME cicli_dfs COMMAND test_cicli_dfs)

enable_testing()


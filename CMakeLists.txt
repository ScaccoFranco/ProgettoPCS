cmake_minimum_required(VERSION 3.20)
project(progetto)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla -Wno-sign-compare")

find_package(Eigen3 REQUIRED)

set(SRC_main main.cpp)
add_executable(progetto ${SRC_main})
target_link_libraries(progetto Eigen3::Eigen)

enable_testing()

# 1. Test Grafo
add_executable(test_grafo tests/test_grafo.cpp)
target_link_libraries(test_grafo Eigen3::Eigen)
add_test(NAME TestGrafo COMMAND test_grafo)

# 2. Test Matrici
add_executable(test_matrici tests/test_matrici.cpp)
target_link_libraries(test_matrici Eigen3::Eigen)
add_test(NAME TestMatrici COMMAND test_matrici)

# 3. Test Parser
add_executable(test_parser tests/test_parser.cpp)
target_link_libraries(test_parser Eigen3::Eigen)
add_test(NAME TestParser COMMAND test_parser)

# 4. Test Dominio Elettrotecnico
add_executable(test_dominio tests/test_dominio.cpp)
target_link_libraries(test_dominio Eigen3::Eigen)
add_test(NAME TestSegniDominio COMMAND test_dominio)


# 4. Test COnfronto De Pina find_cycles
add_executable(test_confronto tests/test_confronto.cpp)
target_link_libraries(test_confronto Eigen3::Eigen)
add_test(NAME TestConfrontoRisultati COMMAND test_confronto)
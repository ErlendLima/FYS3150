project("Project1")
cmake_minimum_required(VERSION 3.9)
include_directories("src")
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-g -O3 -Wall  -std=c++14 -D_GLIBCXX_PARALLEL -Wextra -Wshadow -Wfloat-equal -Wundef -Wstrict-overflow=5 -Wwrite-strings -Wcast-qual -Wswitch-default -march=native -Wswitch-enum -Wconversion -Wunreachable-code -Wformat=2")
message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})
# find_package(PkgConfig)
# pkg_check_modules(GTEST REQUIRED gtest>=1.7.0)
# # pkg_check_modules(GMOCK REQUIRED gmock>=1.7.0)

# include_directories(
#     ${GTEST_INCLUDE_DIRS}
#     # ${GMOCK_INCLUDE_DIRS}
# )
add_executable(solve src/main.cpp
  src/solve.cpp
  src/solver.cpp)
target_link_libraries(solve
  blas lapack armadillo)

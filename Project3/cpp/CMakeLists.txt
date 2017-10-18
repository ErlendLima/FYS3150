cmake_minimum_required(VERSION 3.1)

# Options. Turn on with 'cmake -Dmyvarname=ON'.
option(TEST "Build all tests." OFF) # Makes boolean 'test' available.

# Make PROJECT_SOURCE_DIR, PROJECT_BINARY_DIR, and PROJECT_NAME available.
set(PROJECT_NAME Project3)
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS_DEBUG "-g -O3 -Wall -fopenmp  -std=c++14 -D_GLIBCXX_PARALLEL -Wextra -Wshadow -Wfloat-equal -Wundef -Wstrict-overflow=5 -Wwrite-strings -Wcast-qual -Wswitch-default -march=native -Wswitch-enum -Wconversion -Wunreachable-code -Wformat=2")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -fopenmp -std=c++14 -D_GLIBCXX_PARALLEL -Wfloat-equal -march=native -Wunreachable-code -Wshadow")

set(include_files src/main.cpp src/planet.cpp src/solarSys.cpp src/solver.cpp src/Vec3/vec3.cpp)
set(executable_name solve)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RELEASE")
endif(NOT CMAKE_BUILD_TYPE)
message("Build type is ${CMAKE_BUILD_TYPE}")

# force static runtime libraries for msvc builds
if(MSVC)
  set(variables
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
	CMAKE_CXX_FLAGS_RELWITHDEBINFO
	CMAKE_CXX_FLAGS_MINSIZEREL
  )
  foreach(variable ${variables})
	if(${variable} MATCHES "/MD")
	  string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
	endif()
  endforeach()
endif()

# If you want your own include/ directory, set this, and then you can do
# include_directories(${COMMON_INCLUDES}) in other CMakeLists.txt files.
# set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)

################################
# Normal Libraries & Executables
################################
find_package(Armadillo REQUIRED)
include_directories("src")
add_library(project_lib ${include_files})
message("${executable_name} ${include_files}")
add_executable(${executable_name} ${include_files})
# Key idea: SEPARATE OUT your main() function into its own file so it can be its
# own executable. Separating out main() means you can add this library to be
# used elsewhere.

target_link_libraries(solve project_lib blas lapack -O1 armadillo )

################################
# Testing
################################
if (test)
  if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
  endif (APPLE)

  # This adds another subdirectory, which has 'project(googletest)'.
  add_subdirectory(../../googletest [binary-dir])

  enable_testing()

  # Include the gtest library. gtest_SOURCE_DIR is available due to
  # 'project(gtest)' above.
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

  ##############
  # Unit Tests
  ##############
  add_executable(runUnitTests test/test_solve.cpp)

  # Standard linking to gtest stuff.
  target_link_libraries(runUnitTests gtest gtest_main)

  # # Extra linking for the project.
  target_link_libraries(runUnitTests project_lib blas lapack -O1 armadillo)

  # This is so you can do 'make test' to see all your tests run, instead of
  # manually running the executable runUnitTests to see those specific tests.
  # add_test(NAME that-test-I-made COMMAND runUnitTests)

  # You can also omit NAME and COMMAND. The second argument could be some other
  # test executable.
  # add_test(that-other-test-I-made runUnitTests)
  add_test(jacobi runUnitTests)
  add_test(find_max_offidag runUnitTests)
endif()

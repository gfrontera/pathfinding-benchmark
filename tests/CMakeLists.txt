# Set the name of the test executable
set(PROJECT_NAME all_tests)

# Include the source files
file(GLOB_RECURSE TEST_HEADERS *.h)
file(GLOB_RECURSE TEST_SOURCES *.cpp)

# Create executable
add_executable(${PROJECT_NAME} main.cpp ${TEST_HEADERS} ${TEST_SOURCES})

# Include and link from other projects
include_directories(${CMAKE_SOURCE_DIR})

set(PROJECTS_REQUIRED env geom pf sim)

foreach(REQUIRED_PROJECT ${PROJECTS_REQUIRED})
target_link_libraries(${PROJECT_NAME} ${REQUIRED_PROJECT})
endforeach(REQUIRED_PROJECT)

# Include and link from external projects

# GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})

# Add the test
GTEST_ADD_TESTS(${PROJECT_NAME} "" ${TEST_HEADERS})

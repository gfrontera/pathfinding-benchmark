# Set the name of the project
set(PROJECT_NAME ser)

# Include the source files
file(GLOB_RECURSE PROJECT_HEADERS *.h)
file(GLOB_RECURSE PROJECT_SOURCES *.cpp)

# Create executable
add_library(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES})

# Include and link from other projects
include_directories(${CMAKE_SOURCE_DIR})

set(PROJECTS_REQUIRED log)

foreach(REQUIRED_PROJECT ${PROJECTS_REQUIRED})
  target_link_libraries(${PROJECT_NAME} ${REQUIRED_PROJECT})
endforeach(REQUIRED_PROJECT)

# Include and link from external projects

# Qt 5
find_package(Qt5 COMPONENTS Core REQUIRED)
target_link_libraries(${PROJECT_NAME} Qt5::Core)

# Boost (includes only)
find_package( Boost REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

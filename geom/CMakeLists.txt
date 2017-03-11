# Set the name of the project
set(PROJECT_NAME geom)

# Include the source files
file(GLOB_RECURSE PROJECT_HEADERS *.h)
file(GLOB_RECURSE PROJECT_SOURCES *.cpp)

# Create library
add_library(${PROJECT_NAME} STATIC ${PROJECT_HEADERS} ${PROJECT_SOURCES})

# Include and link from other projects
include_directories(${CMAKE_SOURCE_DIR})

set(PROJECTS_REQUIRED log math)

foreach(REQUIRED_PROJECT ${PROJECTS_REQUIRED})
  target_link_libraries(${PROJECT_NAME} ${REQUIRED_PROJECT})
endforeach(REQUIRED_PROJECT)

# Include and link from external projects

# Armadillo
find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${ARMADILLO_LIBRARIES})

# Boost (includes only)
find_package( Boost REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

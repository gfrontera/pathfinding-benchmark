# Set the name of the project
set(PROJECT_NAME log)

# Include the source files
file(GLOB_RECURSE PROJECT_HEADERS *.h)
file(GLOB_RECURSE PROJECT_SOURCES *.cpp)

# Create library
add_library(${PROJECT_NAME} STATIC ${PROJECT_HEADERS} ${PROJECT_SOURCES})

# Include and link from other projects
include_directories(${CMAKE_SOURCE_DIR})

set(PROJECTS_REQUIRED)

foreach(REQUIRED_PROJECT ${PROJECTS_REQUIRED})
  target_link_libraries(${PROJECT_NAME} ${REQUIRED_PROJECT})
endforeach(REQUIRED_PROJECT)

# Include and link from external projects

# Boost
find_package( Boost COMPONENTS date_time REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} )

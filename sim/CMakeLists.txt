# Set the name of the project
set(PROJECT_NAME sim)

# Include the source files
file(GLOB_RECURSE PROJECT_HEADERS *.h)
file(GLOB_RECURSE PROJECT_SOURCES *.cpp)

# Create library
add_library(${PROJECT_NAME} STATIC ${PROJECT_HEADERS} ${PROJECT_SOURCES})

# Include and link from other projects
include_directories(${CMAKE_SOURCE_DIR})

set(PROJECTS_REQUIRED log pf)

foreach(REQUIRED_PROJECT ${PROJECTS_REQUIRED})
  target_link_libraries(${PROJECT_NAME} ${REQUIRED_PROJECT})
endforeach(REQUIRED_PROJECT)

# Include and link from external projects

# Boost (includes only)
find_package( Boost REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

# Posix Threads
find_package( Threads REQUIRED )
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

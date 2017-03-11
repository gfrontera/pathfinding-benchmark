# Set the name of the project
set(PROJECT_NAME ues_interface)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

# Include the source files
file(GLOB_RECURSE PROJECT_HEADERS *.h)
file(GLOB_RECURSE PROJECT_SOURCES *.cpp)
file(GLOB_RECURSE PROJECT_INTERFACES *.ui)
file(GLOB_RECURSE PROJECT_RESOURCES *.qrc)

# Use the Widgets module from Qt5 (required before calling qt5_wrap_ui)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
# Generate Qt5 interface files
qt5_wrap_ui(PROJECT_INTERFACE_HEADERS ${PROJECT_INTERFACES})
# Generate Qt5 resource files
qt5_add_resources(PROJECT_RESOURCES_SOURCES ${PROJECT_RESOURCES})

# Create executable
add_executable(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES} ${PROJECT_INTERFACE_HEADERS} ${PROJECT_RESOURCES_SOURCES})

# Include and link from other projects
include_directories(${CMAKE_SOURCE_DIR})

set(PROJECTS_REQUIRED env exc geom log ser sim)

foreach(REQUIRED_PROJECT ${PROJECTS_REQUIRED})
  target_link_libraries(${PROJECT_NAME} ${REQUIRED_PROJECT})
endforeach(REQUIRED_PROJECT)

# Include and link from external projects

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

# Qt 5
find_package(Qt5 COMPONENTS OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::OpenGL)

# Boost (includes only)
find_package( Boost REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

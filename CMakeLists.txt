cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enables testing
enable_testing()

add_compile_options(-pedantic-errors -Wall)

# Add all sub-projects
add_subdirectory(env)
add_subdirectory(exc)
add_subdirectory(geom)
add_subdirectory(gui)
add_subdirectory(log)
add_subdirectory(math)
add_subdirectory(misc)
add_subdirectory(pf)
add_subdirectory(ser)
add_subdirectory(sim)

# Add tests
add_subdirectory(tests)

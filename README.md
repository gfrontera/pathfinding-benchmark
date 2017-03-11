# Pathfinder Benchmark
This project performs a comparison of several pathfinding algorithms, including a novel algorithm described in [this journal paper](http://dx.doi.org/10.1007/s10846-016-0409-1).

## Structure
The source files are grouped into several sub-projects:

- **env**: Provides classes that model the environment. This includes the obstacles present.

- **exc**: Provides an exception class that is used in many of the other sub-projects.

- **geom**: Provides a number of geometric-related functionality, such as representations of points, segments and polygons; and algorithms that operate on these elements.
       
- **gui**: Provides a graphical user interface to run and view the results of the simulations.
       
- **log**: Provides logging tools.

- **math**: Basic mathematical types and operations.

- **misc**: Provides miscellaneous tools, such as a cache.

- **pf**: Provides several pathfinding algorithms, for 2D and 3D environments.

- **ser**: Provides serialization of some objects to/from Json.

- **sim**: Provides an environment to perform simulations and to process the results.
       
- **tests**: Provides unitary tests for some functionality.

## Dependencies
In order to compile these sources, additional libraries are required:

- Armadillo (versions 3.920.2 to 7.600.2)
- Boost (versions 1.55 to 1.63)
- Qt 5 (versions 5.2.1 to 5.8.0)
- GTest (versions 1.7.0 to 1.8)
- OMPL (versions 1.1.0 to 1.3.0)

Version numbers shown in the list above are the ones that have been tested. It does not mean that the project will not work for other versions of these libraries.

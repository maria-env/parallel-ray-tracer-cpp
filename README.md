# Parallel Ray Tracer in C++ with TBB
 
Image renderer based on ray tracing, written in C++23 and parallelized with Intel oneTBB. Project for the Computer Architecture course (Universidad Carlos III de Madrid), parallel version (`render-par`).
 
## What's included
 
- **Geometry and scene:** spheres and cylinders with materials, configurable camera, scene loading from file and a Mersenne Twister random number generator.
- **Ray tracer** with per-pixel sampling, maximum bounce depth and gamma correction.
- **Parallelization with TBB:** `parallel_for` over 2D ranges, with configurable thread count, partitioner choice (simple, static or auto) and grain size.
- **Tests:** unit tests (`utcommon/`) and functional tests (`ftest.py`, scenes in `test_files/`).
- **Code quality:** strict compiler flags (`-Wall -Wextra -Werror -pedantic`), `clang-tidy` and `clang-format`, and a reproducible environment with Dev Container.
## Structure
 
| Folder | Contents |
|---|---|
| `common/` | Library with geometry, materials, scene and ray tracer |
| `par/` | Parallel executable `render-par` |
| `utcommon/` | Unit tests |
| `test_files/` | Test configurations and scenes |
 
## Build
 
Requires CMake 3.28 or later, `g++-14` and TBB.
 
```bash
cmake --preset default
cmake --build --preset gcc-release
```
 
## Usage
 
```bash
render-par <config.txt> <scene.txt> <output.ppm> [num_threads] [partitioner] [grain_size]
```
 
Example with one of the test scenes:
 
```bash
render-par test_files/config_test1.txt test_files/scene_test1.txt output.ppm
```
 
The output is an image in PPM format. The destination folder must already exist.
 
## Technologies
 
C++23 · oneTBB · CMake · Docker / Dev Container · clang-tidy · clang-format
 
## Authors
 
Team project by María Arias Rodríguez, Jorge Ignacio Castañeda Vallenilla, Ana Díaz Jiménez and Jaime Sánchez Sánchez.

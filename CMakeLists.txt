cmake_minimum_required(VERSION 3.28)
project(render_project LANGUAGES CXX)

# Set C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find TBB package (required for parallel execution)
find_package(TBB REQUIRED)

# Enable testing
enable_testing()

if(ENABLE_CLANG_TIDY)
  find_program(CLANG_TIDY_EXE NAMES clang-tidy-20 clang-tidy)
  if(CLANG_TIDY_EXE)
    message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXE}")
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};--header-filter=.*")
  else()
    message(STATUS "clang-tidy not found; skipping CMAKE_CXX_CLANG_TIDY")
  endif()
endif()

add_subdirectory(common)
add_subdirectory(par)

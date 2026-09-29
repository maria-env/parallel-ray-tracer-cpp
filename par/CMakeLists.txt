add_executable(render-par)
target_sources(render-par 
    PRIVATE 
      src/main.cpp
      src/imagen_par.cpp
)
target_include_directories(render-par PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link with common (which transitively provides TBB::tbb)
target_link_libraries(render-par PRIVATE common)

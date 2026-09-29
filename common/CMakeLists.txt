add_library(common STATIC)

target_sources(common 
    PRIVATE 
        src/vector.cpp
        src/rayo.cpp
        src/esfera.cpp
        src/cilindro.cpp
        src/escena.cpp
        src/geometria.cpp
        src/mersenne_twister.cpp
        src/tamaño_ventana.cpp
        src/ventana.cpp
        src/configuracion.cpp
        src/lector_archivo_escena.cpp
        src/material.cpp
        src/trazador_rayos.cpp
)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(common PUBLIC TBB::tbb)

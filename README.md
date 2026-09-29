# Ray tracer paralelo en C++ con TBB

Renderizador de imágenes por trazado de rayos (ray tracing) escrito en C++23 y paralelizado con Intel oneTBB. Práctica de Arquitectura de Computadores (Universidad Carlos III de Madrid), versión paralela (`render-par`).

## Qué incluye

- **Geometría y escena:** esferas y cilindros con materiales, cámara configurable, lectura de escenas desde fichero y generador de números aleatorios Mersenne Twister.
- **Trazador de rayos** con muestreo por píxel, profundidad máxima de rebotes y corrección gamma.
- **Paralelización con TBB:** `parallel_for` sobre rangos 2D, con número de hilos configurable, elección de partitioner (simple, static o auto) y tamaño de grano.
- **Pruebas:** tests unitarios (`utcommon/`) y funcionales (`ftest.py`, escenas en `test_files/`).
- **Calidad de código:** flags estrictos de compilación (`-Wall -Wextra -Werror -pedantic`), `clang-tidy` y `clang-format`, y un entorno reproducible con Dev Container.

## Estructura

| Carpeta | Contenido |
|---|---|
| `common/` | Biblioteca con geometría, materiales, escena y trazador de rayos |
| `par/` | Ejecutable paralelo `render-par` |
| `utcommon/` | Tests unitarios |
| `test_files/` | Configuraciones y escenas de prueba |

## Cómo compilarlo

Requiere CMake 3.28 o superior, `g++-14` y TBB.

```bash
cmake --preset default
cmake --build --preset gcc-release
```

## Cómo ejecutarlo

```bash
render-par <config.txt> <escena.txt> <salida.ppm> [num_hilos] [partitioner] [tamaño_grano]
```

Ejemplo con una de las escenas de prueba:

```bash
render-par test_files/config_test1.txt test_files/scene_test1.txt salida.ppm
```

La salida es una imagen en formato PPM. La carpeta de destino debe existir.

## Tecnologías

C++23 · oneTBB · CMake · Docker / Dev Container · clang-tidy · clang-format

## Autoría

Trabajo en equipo de 4 personas: María Arias Rodríguez, Jorge Ignacio Castañeda Vallenilla, Ana Díaz Jiménez y Jaime Sánchez Sánchez.

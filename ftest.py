#!/usr/bin/env python3
"""
Script de pruebas funcionales para render-aos
Uso: python test_render.py
"""

import os
import subprocess
import sys
from pathlib import Path
from typing import Tuple, List

# Configuración
RENDER_BIN = "/workspace/out/build/clang-tidy/aos/Debug/render-aos"
TEST_DIR = "/workspace/test_files"
OUTPUT_DIR = "/workspace/test_output"

# Colores para terminal
class Colors:
    GREEN = '\033[0;32m'
    RED = '\033[0;31m'
    YELLOW = '\033[1;33m'
    BLUE = '\033[0;34m'
    NC = '\033[0m'  # No Color

class TestStats:
    def __init__(self):
        self.passed = 0
        self.failed = 0
        self.total = 0

def create_directories():
    """Crea los directorios necesarios para las pruebas"""
    Path(TEST_DIR).mkdir(parents=True, exist_ok=True)
    Path(OUTPUT_DIR).mkdir(parents=True, exist_ok=True)

def write_file(filepath: str, content: str):
    """Escribe contenido en un archivo"""
    with open(filepath, 'w') as f:
        f.write(content)

def is_valid_ppm(filepath: str) -> bool:
    """Verifica si el archivo es un PPM válido"""
    try:
        with open(filepath, 'r') as f:
            first_line = f.readline().strip()
            return first_line == "P3"
    except:
        return False

def run_test(test_name: str, config_file: str, scene_file: str, 
             output_file: str, expected_result: str, stats: TestStats) -> bool:
    """
    Ejecuta una prueba individual
    
    Args:
        test_name: Nombre descriptivo de la prueba
        config_file: Ruta al archivo de configuración
        scene_file: Ruta al archivo de escena
        output_file: Ruta al archivo de salida
        expected_result: "success" o "fail"
        stats: Objeto para tracking de estadísticas
    
    Returns:
        True si la prueba pasó, False en caso contrario
    """
    stats.total += 1
    
    print(f"\n{Colors.YELLOW}[TEST {stats.total}]{Colors.NC} {test_name}")
    
    # Ejecutar el comando
    try:
        result = subprocess.run(
            [RENDER_BIN, config_file, scene_file, output_file],
            capture_output=True,
            text=True,
            timeout=300  # 5 minutos timeout
        )
        exit_code = result.returncode
    except subprocess.TimeoutExpired:
        print(f"{Colors.RED}✗ FAILED{Colors.NC} (Timeout - excedió 5 minutos)")
        stats.failed += 1
        return False
    except Exception as e:
        print(f"{Colors.RED}✗ FAILED{Colors.NC} (Error: {e})")
        stats.failed += 1
        return False
    
    # Evaluar resultado
    if expected_result == "success":
        if exit_code == 0 and os.path.exists(output_file):
            print(f"{Colors.GREEN}✓ PASSED{Colors.NC}")
            stats.passed += 1
            
            # Verificar validez del PPM
            if is_valid_ppm(output_file):
                print(f"  {Colors.BLUE}→{Colors.NC} Archivo PPM válido generado")
                file_size = os.path.getsize(output_file)
                print(f"  {Colors.BLUE}→{Colors.NC} Tamaño: {file_size:,} bytes")
            else:
                print(f"  {Colors.RED}⚠{Colors.NC} Advertencia: El archivo no parece un PPM válido")
            
            return True
        else:
            print(f"{Colors.RED}✗ FAILED{Colors.NC} (Exit code: {exit_code})")
            if result.stderr:
                print(f"  Error: {result.stderr[:200]}")
            stats.failed += 1
            return False
    else:  # expected_result == "fail"
        if exit_code != 0:
            print(f"{Colors.GREEN}✓ PASSED{Colors.NC} (Falló como se esperaba)")
            stats.passed += 1
            return True
        else:
            print(f"{Colors.RED}✗ FAILED{Colors.NC} (Debería haber fallado)")
            stats.failed += 1
            return False

def test_1_basic_sphere(stats: TestStats):
    """TEST 1: Escena simple con una esfera"""
    config = """image_width: 400
gamma: 2.2

camera_position: 0 0 10
camera_target: 0 0 0
camera_north: 0 1 0
field_of_view: 45

samples_per_pixel: 10
max_depth: 10

material_rng_seed: 1
ray_rng_seed: 1

background_dark_color: 0.5 0.7 1.0
background_light_color: 1.0 1.0 1.0
"""
    
    scene = """matte: red_matte 0.8 0.2 0.2
sphere: 0 0 0 1 red_matte
"""
    
    config_file = f"{TEST_DIR}/config_test1.txt"
    scene_file = f"{TEST_DIR}/scene_test1.txt"
    output_file = f"{OUTPUT_DIR}/test1.ppm"
    
    write_file(config_file, config)
    write_file(scene_file, scene)
    
    run_test("Escena básica: una esfera matte",
             config_file, scene_file, output_file, "success", stats)

def test_2_multiple_materials(stats: TestStats):
    """TEST 2: Múltiples materiales y geometrías"""
    config = """image_width: 600
gamma: 2.4

camera_position: 5 3 5
camera_target: 0 0 0
camera_north: 0 1 0
field_of_view: 60

samples_per_pixel: 50
max_depth: 50

material_rng_seed: 42
ray_rng_seed: 100

background_dark_color: 0.2 0.2 0.2
background_light_color: 0.6 0.7 1.0
"""
    
    scene = """matte: ground 0.5 0.5 0.5
metal: gold 0.8 0.6 0.2 0.3
metal: silver 0.9 0.9 0.9 0.1
refractive: glass 1.5

sphere: 0 -100.5 0 100 ground
sphere: -1.5 0 0 0.5 gold
sphere: 0 0 0 0.5 glass
sphere: 1.5 0 0 0.5 silver
"""
    
    config_file = f"{TEST_DIR}/config_test2.txt"
    scene_file = f"{TEST_DIR}/scene_test2.txt"
    output_file = f"{OUTPUT_DIR}/test2.ppm"
    
    write_file(config_file, config)
    write_file(scene_file, scene)
    
    run_test("Múltiples materiales (matte, metal, refractive)",
             config_file, scene_file, output_file, "success", stats)

def test_3_cylinders(stats: TestStats):
    """TEST 3: Cilindros"""
    config = """image_width: 500
gamma: 2.2

camera_position: 8 4 8
camera_target: 0 1 0
camera_north: 0 1 0
field_of_view: 50

samples_per_pixel: 30
max_depth: 30

material_rng_seed: 10
ray_rng_seed: 20

background_dark_color: 0.3 0.3 0.3
background_light_color: 0.5 0.6 0.8
"""
    
    scene = """metal: blue_metal 0.2 0.4 0.8 0.5
matte: orange 0.9 0.5 0.1

cylinder: 0 0 0 0.5 0 3 0 blue_metal
cylinder: 2 0 0 0.3 0 2 0 orange
"""
    
    config_file = f"{TEST_DIR}/config_test3.txt"
    scene_file = f"{TEST_DIR}/scene_test3.txt"
    output_file = f"{OUTPUT_DIR}/test3.ppm"
    
    write_file(config_file, config)
    write_file(scene_file, scene)
    
    run_test("Geometría: cilindros",
             config_file, scene_file, output_file, "success", stats)

def test_4_complex_scene(stats: TestStats):
    """TEST 4: Escena compleja (similar a config3)"""
    config = """image_width: 800
gamma: 2.6

camera_position: 10 2 3
camera_target: 5 1 0
camera_north: 0 1 0
field_of_view: 50

samples_per_pixel: 100
max_depth: 50

material_rng_seed: 45
ray_rng_seed: 133

background_dark_color: 0.25 0.25 0.25
background_light_color: 0 0.5 1
"""
    
    scene = """matte: mat1 0.75 0.25 0
metal: metal1 0.9 0 0 2.0
metal: metal2 0.8 0.8 0 1.5
metal: metal3 0 0 0.8 2.0
refractive: ref1 1.3
refractive: ref2 0.5

sphere: 0 0 0 2 ref1
sphere: 0 2 0 1 metal2
sphere: 0 -2 0 1 metal3
sphere: 3 2 0 0.25 mat1
cylinder: 5 -3 0 0.5 4 2.5 -1.25 ref1
cylinder: 3 0 -3 0.5 -4 2.5 1.25 metal1
"""
    
    config_file = f"{TEST_DIR}/config_test4.txt"
    scene_file = f"{TEST_DIR}/scene_test4.txt"
    output_file = f"{OUTPUT_DIR}/test4.ppm"
    
    write_file(config_file, config)
    write_file(scene_file, scene)
    
    run_test("Escena compleja con múltiples objetos",
             config_file, scene_file, output_file, "success", stats)

def test_5_minimal_values(stats: TestStats):
    """TEST 5: Valores extremos - imagen pequeña"""
    config = """image_width: 100
gamma: 2.2

camera_position: 0 0 5
camera_target: 0 0 0
camera_north: 0 1 0
field_of_view: 45

samples_per_pixel: 5
max_depth: 5

material_rng_seed: 1
ray_rng_seed: 1

background_dark_color: 0.1 0.1 0.1
background_light_color: 0.9 0.9 0.9
"""
    
    scene = """matte: simple 0.5 0.5 0.5
sphere: 0 0 0 1 simple
"""
    
    config_file = f"{TEST_DIR}/config_test5.txt"
    scene_file = f"{TEST_DIR}/scene_test5.txt"
    output_file = f"{OUTPUT_DIR}/test5.ppm"
    
    write_file(config_file, config)
    write_file(scene_file, scene)
    
    run_test("Valores mínimos (imagen pequeña, pocos samples)",
             config_file, scene_file, output_file, "success", stats)

def test_6_high_quality(stats: TestStats):
    """TEST 6: Alta calidad"""
    config = """image_width: 1920
gamma: 2.2

camera_position: 0 2 10
camera_target: 0 0 0
camera_north: 0 1 0
field_of_view: 45

samples_per_pixel: 500
max_depth: 100

material_rng_seed: 999
ray_rng_seed: 888

background_dark_color: 0.05 0.05 0.1
background_light_color: 0.5 0.7 1.0
"""
    
    scene = """matte: ground 0.8 0.8 0.8
refractive: crystal 1.5
metal: chrome 0.95 0.95 0.95 0.05

sphere: 0 -1000.5 0 1000 ground
sphere: 0 0 0 1 crystal
sphere: -2.5 0 0 1 chrome
"""
    
    config_file = f"{TEST_DIR}/config_test6.txt"
    scene_file = f"{TEST_DIR}/scene_test6.txt"
    output_file = f"{OUTPUT_DIR}/test6.ppm"
    
    write_file(config_file, config)
    write_file(scene_file, scene)
    
    print(f"{Colors.YELLOW}⚠ Advertencia: Este test puede tardar varios minutos{Colors.NC}")
    run_test("Alta calidad (1920px, 500 samples)",
             config_file, scene_file, output_file, "success", stats)

def test_7_empty_scene(stats: TestStats):
    """TEST 7: Sin objetos (solo fondo)"""
    config = """image_width: 400
gamma: 2.2

camera_position: 0 0 0
camera_target: 0 0 -1
camera_north: 0 1 0
field_of_view: 90

samples_per_pixel: 10
max_depth: 10

material_rng_seed: 1
ray_rng_seed: 1

background_dark_color: 0.1 0.2 0.3
background_light_color: 0.7 0.8 0.9
"""
    
    scene = ""  # Escena vacía
    
    config_file = f"{TEST_DIR}/config_test7.txt"
    scene_file = f"{TEST_DIR}/scene_test7.txt"
    output_file = f"{OUTPUT_DIR}/test7.ppm"
    
    write_file(config_file, config)
    write_file(scene_file, scene)
    
    run_test("Escena vacía (solo fondo)",
             config_file, scene_file, output_file, "success", stats)

def test_8_refraction_indices(stats: TestStats):
    """TEST 8: Diferentes índices de refracción"""
    config = """image_width: 600
gamma: 2.2

camera_position: 0 0 8
camera_target: 0 0 0
camera_north: 0 1 0
field_of_view: 45

samples_per_pixel: 100
max_depth: 50

material_rng_seed: 50
ray_rng_seed: 60

background_dark_color: 0.3 0.3 0.3
background_light_color: 0.8 0.9 1.0
"""
    
    scene = """refractive: air 1.0
refractive: water 1.33
refractive: glass 1.5
refractive: diamond 2.42

sphere: -3 0 0 0.8 air
sphere: -1 0 0 0.8 water
sphere: 1 0 0 0.8 glass
sphere: 3 0 0 0.8 diamond
"""
    
    config_file = f"{TEST_DIR}/config_test8.txt"
    scene_file = f"{TEST_DIR}/scene_test8.txt"
    output_file = f"{OUTPUT_DIR}/test8.ppm"
    
    write_file(config_file, config)
    write_file(scene_file, scene)
    
    run_test("Diferentes índices de refracción",
             config_file, scene_file, output_file, "success", stats)

def test_9_missing_config(stats: TestStats):
    """TEST 9: Archivo de configuración inexistente"""
    scene_file = f"{TEST_DIR}/scene_test1.txt"
    output_file = f"{OUTPUT_DIR}/test9.ppm"
    
    run_test("Error: config inexistente",
             f"{TEST_DIR}/noexiste.txt", scene_file, output_file, "fail", stats)

def test_10_missing_scene(stats: TestStats):
    """TEST 10: Archivo de escena inexistente"""
    config_file = f"{TEST_DIR}/config_test1.txt"
    output_file = f"{OUTPUT_DIR}/test10.ppm"
    
    run_test("Error: scene inexistente",
             config_file, f"{TEST_DIR}/noexiste.txt", output_file, "fail", stats)

def test_11_undefined_material(stats: TestStats):
    """TEST 11: Material no definido"""
    config = """image_width: 400
gamma: 2.2
camera_position: 0 0 5
camera_target: 0 0 0
camera_north: 0 1 0
field_of_view: 45
samples_per_pixel: 10
max_depth: 10
material_rng_seed: 1
ray_rng_seed: 1
background_dark_color: 0.5 0.5 0.5
background_light_color: 1.0 1.0 1.0
"""
    
    scene = """sphere: 0 0 0 1 material_no_definido
"""
    
    config_file = f"{TEST_DIR}/config_test11.txt"
    scene_file = f"{TEST_DIR}/scene_test11.txt"
    output_file = f"{OUTPUT_DIR}/test11.ppm"
    
    write_file(config_file, config)
    write_file(scene_file, scene)
    
    run_test("Error: material no definido",
             config_file, scene_file, output_file, "fail", stats)

def print_summary(stats: TestStats):
    """Imprime el resumen de las pruebas"""
    print("\n" + "=" * 50)
    print("  RESUMEN DE PRUEBAS")
    print("=" * 50)
    print(f"Total:   {stats.total}")
    print(f"{Colors.GREEN}Pasadas: {stats.passed}{Colors.NC}")
    print(f"{Colors.RED}Fallidas: {stats.failed}{Colors.NC}")
    print()
    
    if stats.failed == 0:
        print(f"{Colors.GREEN}¡Todas las pruebas pasaron exitosamente!{Colors.NC}")
        return 0
    else:
        print(f"{Colors.RED}Algunas pruebas fallaron. Revisa los resultados arriba.{Colors.NC}")
        return 1

def main():
    """Función principal"""
    print("=" * 50)
    print("  PRUEBAS FUNCIONALES - RENDER-AOS")
    print("=" * 50)
    
    # Verificar que el binario existe
    if not os.path.exists(RENDER_BIN):
        print(f"{Colors.RED}Error: No se encuentra el binario en {RENDER_BIN}{Colors.NC}")
        return 1
    
    # Crear directorios
    create_directories()
    
    # Inicializar estadísticas
    stats = TestStats()
    
    # Ejecutar todas las pruebas
    test_1_basic_sphere(stats)
    test_2_multiple_materials(stats)
    test_3_cylinders(stats)
    test_4_complex_scene(stats)
    test_5_minimal_values(stats)
    test_6_high_quality(stats)
    test_7_empty_scene(stats)
    test_8_refraction_indices(stats)
    
    # Pruebas de error
    print(f"\n{Colors.BLUE}=== PRUEBAS DE MANEJO DE ERRORES ==={Colors.NC}")
    test_9_missing_config(stats)
    test_10_missing_scene(stats)
    test_11_undefined_material(stats)
    
    # Imprimir resumen
    return print_summary(stats)

if __name__ == "__main__":
    sys.exit(main())
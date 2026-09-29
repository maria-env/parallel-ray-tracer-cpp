#!/usr/bin/env python3
"""
Script de Python 3.12 para ejecutar todas las pruebas unitarias (GoogleTest) 
del proyecto render.

Este script cumple con el requisito del enunciado "utest.sh/utest.py".

Asume que los ejecutables de las pruebas se han compilado en modo 'Release'
y se encuentran en el directorio de compilación por defecto:
- out/build/default/utcommon/Release/utcommon
- out/build/default/utaos/Release/utaos
- out/build/default/utsoa/Release/utsoa

Ejecutar desde el directorio raíz del proyecto (render-2025-a-m81-01).
"""

import subprocess
import sys
from pathlib import Path

# --- Colores para la salida (funciona en terminales modernas) ---
class Colors:
    HEADER = '\033[95m'
    OKGREEN = '\033[92m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    INFO = '\033[94m'

def print_header(message: str):
    """Imprime un encabezado resaltado."""
    print(f"\n{Colors.HEADER}{Colors.BOLD}>>> {message} <<<{Colors.ENDC}")

def print_pass(message: str):
    """Imprime un mensaje de éxito."""
    print(f"{Colors.OKGREEN}✔ {message} PASSED{Colors.ENDC}")

def print_fail(message: str):
    """Imprime un mensaje de fallo."""
    print(f"{Colors.FAIL}✖ {message} FAILED{Colors.ENDC}")

def print_error(message: str):
    """Imprime un mensaje de error crítico."""
    print(f"{Colors.FAIL}{Colors.BOLD}ERROR: {message}{Colors.ENDC}")

def main():
    """Función principal del script de pruebas."""
    
    # Obtenemos la ruta al directorio donde se encuentra este script
    # Asumimos que es la raíz del proyecto.
    try:
        project_root = Path(__file__).resolve().parent
    except NameError:
        # Fallback por si se ejecuta de forma interactiva
        project_root = Path.cwd()

    build_dir = project_root / "out/build/default"

    # Definimos los ejecutables de las suites de pruebas
    test_suites = {
        "common": build_dir / "utcommon/Release/utcommon",
        "aos": build_dir / "utaos/Release/utaos",
        "soa": build_dir / "utsoa/Release/utsoa",
    }

    failed_suites = []

    print_header("Iniciando Pruebas Unitarias del Proyecto Render")

    # 1. Verificar que todos los ejecutables existen
    all_found = True
    for name, path in test_suites.items():
        if not path.exists():
            print_error(f"El ejecutable de prueba '{name}' no se encontró en:")
            print_error(f" {path}")
            all_found = False
            
    if not all_found:
        print_info("\nAsegúrate de haber compilado el proyecto en modo 'Release'.")
        print_info("Ejemplo: cmake --build out/build/default --config Release")
        sys.exit(1)

    # 2. Ejecutar cada suite de pruebas
    for name, path in test_suites.items():
        print_header(f"Ejecutando suite de pruebas: {name}")
        
        try:
            # Ejecutamos la prueba.
            # GTest imprime directamente a stdout/stderr, así que no capturamos la salida.
            result = subprocess.run(
                [str(path)], 
                text=True, 
                check=False  # No lanzamos excepción si falla, lo manejamos nosotros
            )
            
            # GTest devuelve 0 si pasa, y un número != 0 si falla
            if result.returncode == 0:
                print_pass(f"Suite '{name}'")
            else:
                print_fail(f"Suite '{name}' (código de salida: {result.returncode})")
                failed_suites.append(name)

        except Exception as e:
            print_fail(f"No se pudo ejecutar la suite '{name}': {e}")
            failed_suites.append(name)

    # --- Resumen Final ---
    print_header("Resumen de Pruebas Unitarias")
    if not failed_suites:
        print(f"{Colors.OKGREEN}{Colors.BOLD}¡Todas las pruebas unitarias han pasado! ✅{Colors.ENDC}")
        sys.exit(0)
    else:
        print_fail(f"Fallaron {len(failed_suites)} suite(s): {', '.join(failed_suites)}")
        sys.exit(1)

if __name__ == "__main__":
    main()
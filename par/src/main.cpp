#include "color.hpp"
#include "configuracion.hpp"
#include "geometria.hpp"
#include "imagen_par.hpp"
#include "mersenne_twister.hpp"
#include "tamaño_ventana.hpp"
#include "trazador_rayos.hpp"
#include "ventana.hpp"

#include <oneapi/tbb/blocked_range2d.h>
#include <oneapi/tbb/enumerable_thread_specific.h>
#include <oneapi/tbb/global_control.h>
#include <oneapi/tbb/parallel_for.h>
#include <oneapi/tbb/task_arena.h>

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <span>
#include <string>
#include <utility>

namespace {

  // CONFIGURACIÓN DE PARALELIZACIÓN
  // Valores por defecto
  constexpr int MAX_THREADS                = 128;  // Hilos por defecto
  constexpr int DEFAULT_PARTITIONER        = 1;    // Simple partitioner
  constexpr std::size_t DEFAULT_GRAIN_SIZE = 2;    // Grain size

  struct generadores_hilo {
    render::mersenne_twister mt_rayos;
    render::mersenne_twister mt_materiales;

    generadores_hilo(std::uint64_t semilla_rayos, std::uint64_t semilla_materiales)
        : mt_rayos(static_cast<unsigned>(semilla_rayos)),
          mt_materiales(static_cast<unsigned>(semilla_materiales)) { }
  };

  struct parametros_renderizado {
    std::string const * archivo_escena;
    std::string const * archivo_salida;
    int hilos_solicitados;
    int partitioner_type;
    int grain_size;
  };

  struct argumentos_programa {
    std::string const * archivo_config;
    std::string const * archivo_escena;
    std::string const * archivo_salida;
    std::string const * numero_hilos;
    std::string const * tipo_partitioner;
    std::string const * tam_grano;
  };

  int calcular_num_threads(int hilos_solicitados) {
    if (hilos_solicitados > 0) {
      return hilos_solicitados;
    }
    if (MAX_THREADS > 0) {
      return MAX_THREADS;
    }
    return tbb::this_task_arena::max_concurrency();
  }

  void ejecutar_parallel_for(tbb::blocked_range2d<int> const & rango, auto const & procesar_pixeles,
                             int partitioner_type) {
    std::cout << "  Partitioner: ";
    switch (partitioner_type) {
      case 1:
        std::cout << "Simple" << '\n';
        tbb::parallel_for(rango, procesar_pixeles, tbb::simple_partitioner{});
        break;
      case 2:
        std::cout << "Static" << '\n';
        tbb::parallel_for(rango, procesar_pixeles, tbb::static_partitioner{});
        break;
      default:  // 0 y otros
        std::cout << "Auto" << '\n';
        tbb::parallel_for(rango, procesar_pixeles, tbb::auto_partitioner{});
        break;
    }
  }

  std::pair<std::size_t, std::size_t> configurar_grain_sizes(int grain_size) {
    std::size_t const gs =
        (grain_size > 0) ? static_cast<std::size_t>(grain_size) : DEFAULT_GRAIN_SIZE;
    std::cout << "  Grain Size: " << gs << "x" << gs << ((grain_size == 0) ? " (default)" : "")
              << '\n';
    return {gs, gs};
  }

  tbb::enumerable_thread_specific<generadores_hilo> crear_generadores_locales(
      Configuracion const & config, int num_threads) {
    render::generador_semillas const gen_rayos(static_cast<std::uint64_t>(config.ray_rng_seed),
                                               static_cast<std::size_t>(num_threads));
    render::generador_semillas const gen_materiales(
        static_cast<std::uint64_t>(config.material_rng_seed),
        static_cast<std::size_t>(num_threads));
    std::atomic<std::size_t> thread_counter{0};
    return tbb::enumerable_thread_specific<generadores_hilo>{[=, &thread_counter]() mutable {
      std::size_t const id = thread_counter++;
      return generadores_hilo(
          gen_rayos.obtener_semilla(id % static_cast<std::size_t>(num_threads)),
          gen_materiales.obtener_semilla(id % static_cast<std::size_t>(num_threads)));
    }};
  }

  void renderizar_paralelo(Configuracion const & config, int alto_imagen,
                           parametros_renderizado const & params) {
    render::imagen_par imagen(config.image_width, alto_imagen);
    render::geometria const geo(config.camera_position, config.camera_target, config.camera_north,
                                config.field_of_view);
    render::mersenne_twister const mt_temp(static_cast<unsigned>(config.ray_rng_seed));
    render::ventana const vent(geo, render::tamaño_imagen(config.image_width, alto_imagen),
                               mt_temp);
    render::mersenne_twister mt_materiales_temp(static_cast<unsigned>(config.material_rng_seed));
    render::trazador_rayos trazador(config, mt_materiales_temp);
    trazador.cargar_escena(*params.archivo_escena);
    int const num_threads = calcular_num_threads(params.hilos_solicitados);
    std::cout << "Configuracion de renderizado:" << '\n' << "  Hilos: " << num_threads << '\n';
    auto generadores_locales = crear_generadores_locales(config, num_threads);
    auto const [gs_y, gs_x]  = configurar_grain_sizes(params.grain_size);
    tbb::blocked_range2d<int> const rango(0, alto_imagen, gs_y, 0, config.image_width, gs_x);
    auto procesar_pixeles = [&](tbb::blocked_range2d<int> const & r) {
      generadores_hilo & gen = generadores_locales.local();
      for (int y = r.rows().begin(); y < r.rows().end(); ++y) {
        for (int x = r.cols().begin(); x < r.cols().end(); ++x) {
          render::color pixel_color(0.0, 0.0, 0.0);
          for (int s = 0; s < config.samples_per_pixel; ++s) {
            pixel_color = pixel_color.suma(
                trazador.trazar_rayo_con_mt(vent.generar_rayos_pixel_con_mt(x, y, gen.mt_rayos),
                                            config.max_depth, gen.mt_materiales));
          }
          imagen.establecer_pixel(
              x, y,
              pixel_color.escalar(1.0 / static_cast<double>(config.samples_per_pixel))
                  .aplicar_gamma(config.gamma));
        }
      }
    };
    ejecutar_parallel_for(rango, procesar_pixeles, params.partitioner_type);
    imagen.guardar_ppm(*params.archivo_salida);
    std::cout << "Imagen generada: " << *params.archivo_salida << '\n';
  }

  parametros_renderizado parsear_argumentos(argumentos_programa const & args) {
    int hilos = 0;
    if (!args.numero_hilos->empty()) {
      hilos = std::stoi(*args.numero_hilos);
    }

    int partitioner = DEFAULT_PARTITIONER;
    if (!args.tipo_partitioner->empty()) {
      partitioner = std::stoi(*args.tipo_partitioner);
    }

    int grano = 0;
    if (!args.tam_grano->empty()) {
      grano = std::stoi(*args.tam_grano);
    }

    return {args.archivo_escena, args.archivo_salida, hilos, partitioner, grano};
  }

  void ejecutar_renderizado(argumentos_programa const & args) {
    std::cout << "Iniciando renderizado paralelo con TBB" << '\n';

    Configuracion const config = leer_configuracion(*args.archivo_config);

    int const alto_imagen =
        static_cast<int>(static_cast<double>(config.image_width * config.aspect_height) /
                         static_cast<double>(config.aspect_width));

    std::cout << "Dimensiones de imagen: " << config.image_width << "x" << alto_imagen << '\n';
    std::cout << "Muestras por pixel: " << config.samples_per_pixel << '\n';
    std::cout << "Profundidad maxima: " << config.max_depth << '\n';

    parametros_renderizado const params = parsear_argumentos(args);

    if (params.hilos_solicitados > 0) {
      std::cout << "Limitando global_control a " << params.hilos_solicitados << " hilos" << '\n';
      tbb::global_control const gc(tbb::global_control::max_allowed_parallelism,
                                   static_cast<size_t>(params.hilos_solicitados));
      renderizar_paralelo(config, alto_imagen, params);
    } else {
      renderizar_paralelo(config, alto_imagen, params);
    }
  }

}  // namespace

int main(int argc, char * argv[]) noexcept try
{
  std::span<char *> const args(argv, static_cast<size_t>(argc));

  if (argc < 4 or argc > 7) {
    std::cerr << "Error: Numero de argumentos invalido: " << (argc - 1) << '\n';
    std::cerr << "Uso: render-par <config.txt> <escena.txt> <salida.ppm> [num_hilos] [partitioner] "
                 "[grain_size]"
              << '\n';
    std::cerr << "  partitioner: 0=auto, 1=simple, 2=static" << '\n';
    return 1;
  }

  std::string const archivo_config   = args[1];
  std::string const archivo_escena   = args[2];
  std::string const archivo_salida   = args[3];
  std::string const numero_hilos     = (argc >= 5) ? args[4] : "";
  std::string const tipo_partitioner = (argc >= 6) ? args[5] : "";
  std::string const tam_grano        = (argc >= 7) ? args[6] : "";

  argumentos_programa const prog_args{&archivo_config, &archivo_escena,   &archivo_salida,
                                      &numero_hilos,   &tipo_partitioner, &tam_grano};
  ejecutar_renderizado(prog_args);
  return 0;

} catch (std::exception const & e) {
  std::cerr << "Error: " << e.what() << '\n';
  return 1;
} catch (...) {
  std::cerr << "Error: Excepcion desconocida" << '\n';
  return 1;
}

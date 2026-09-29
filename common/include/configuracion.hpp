#ifndef LECTOR_CONFIG_HPP
#define LECTOR_CONFIG_HPP

#include "vector.hpp"
#include <string>

/*Estructura para almacenar los parámetros de configuración*/
struct Configuracion {
  int aspect_width  = 16;
  int aspect_height = 9;
  int image_width   = 1'920;
  double gamma      = 2.2;

  render::vector camera_position = {0.0, 0.0, -10.0};
  render::vector camera_target   = {0.0, 0.0, 0.0};
  render::vector camera_north    = {0.0, 1.0, 0.0};
  double field_of_view           = 90.0;

  int samples_per_pixel = 20;
  int max_depth         = 5;

  int material_rng_seed = 13;
  int ray_rng_seed      = 19;

  render::vector background_dark_color  = {0.25, 0.5, 1.0};
  render::vector background_light_color = {1.0, 1.0, 1.0};
};

Configuracion leer_configuracion(std::string const & ruta);

#endif

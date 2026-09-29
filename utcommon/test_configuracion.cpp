#include "configuracion.hpp"
#include "vector.hpp"
#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

namespace {

  // Crea un archivo temporal con contenido dado y devuelve la ruta
  std::string crear_archivo_temp(std::string const & contenido) {
    std::string const ruta = "tmp_config_test.txt";
    std::ofstream archivo(ruta);
    archivo << contenido;
    archivo.close();
    return ruta;
  }

}  // namespace

// -------------------------------------------------------------

TEST(ConfigTest, ValoresPorDefecto) {
  auto const ruta         = crear_archivo_temp("");
  Configuracion const cfg = leer_configuracion(ruta);

  EXPECT_EQ(cfg.aspect_width, 16);
  EXPECT_EQ(cfg.aspect_height, 9);
  EXPECT_EQ(cfg.image_width, 1'920);
  EXPECT_DOUBLE_EQ(cfg.gamma, 2.2);
  EXPECT_DOUBLE_EQ(cfg.camera_position.getZ(), -10.0);
  EXPECT_DOUBLE_EQ(cfg.field_of_view, 90.0);
  EXPECT_EQ(cfg.samples_per_pixel, 20);
  EXPECT_EQ(cfg.max_depth, 5);
  EXPECT_EQ(cfg.material_rng_seed, 13);
  EXPECT_EQ(cfg.ray_rng_seed, 19);

  (void) std::remove(ruta.c_str());
}

TEST(ConfigTest, CargaBasicaCorrecta) {
  std::string const contenido = R"(
aspect_ratio: 4 3
image_width: 800
gamma: 1.8
camera_position: 1 2 3
camera_target: 0 0 0
camera_north: 0 1 0
field_of_view: 45
samples_per_pixel: 50
max_depth: 10
material_rng_seed: 99
ray_rng_seed: 42
background_dark_color: 0.1 0.2 0.3
background_light_color: 0.9 0.8 0.7
)";
  auto const ruta             = crear_archivo_temp(contenido);
  Configuracion const cfg     = leer_configuracion(ruta);

  EXPECT_EQ(cfg.aspect_width, 4);
  EXPECT_EQ(cfg.aspect_height, 3);
  EXPECT_EQ(cfg.image_width, 800);
  EXPECT_DOUBLE_EQ(cfg.gamma, 1.8);
  EXPECT_DOUBLE_EQ(cfg.camera_position.getX(), 1.0);
  EXPECT_DOUBLE_EQ(cfg.field_of_view, 45.0);
  EXPECT_EQ(cfg.samples_per_pixel, 50);
  EXPECT_EQ(cfg.max_depth, 10);
  EXPECT_EQ(cfg.material_rng_seed, 99);
  EXPECT_EQ(cfg.ray_rng_seed, 42);
  EXPECT_DOUBLE_EQ(cfg.background_dark_color.getX(), 0.1);
  EXPECT_DOUBLE_EQ(cfg.background_light_color.getZ(), 0.7);

  (void) std::remove(ruta.c_str());
}

TEST(ConfigTest, LineasVaciasYEspacios) {
  std::string const contenido = R"(
   
   image_width:   1024   

   gamma: 2.0 
   
)";
  auto const ruta             = crear_archivo_temp(contenido);
  Configuracion const cfg     = leer_configuracion(ruta);

  EXPECT_EQ(cfg.image_width, 1'024);
  EXPECT_DOUBLE_EQ(cfg.gamma, 2.0);

  (void) std::remove(ruta.c_str());
}

TEST(ConfigTest, EtiquetaDesconocidaLanzaError) {
  std::string const contenido = "image_xwidth: 900\n";
  auto const ruta             = crear_archivo_temp(contenido);

  EXPECT_THROW(
      {
        try {
          leer_configuracion(ruta);
        } catch (std::runtime_error const & e) {
          EXPECT_NE(std::string(e.what()).find("Unknown configuration key"), std::string::npos);
          throw;
        }
      },
      std::runtime_error);

  (void) std::remove(ruta.c_str());
}

TEST(ConfigTest, ValorInvalidoLanzaError) {
  std::string const contenido = "aspect_ratio: -4 3\n";
  auto const ruta             = crear_archivo_temp(contenido);

  EXPECT_THROW(
      {
        try {
          leer_configuracion(ruta);
        } catch (std::runtime_error const & e) {
          EXPECT_NE(std::string(e.what()).find("Invalid value for key"), std::string::npos);
          throw;
        }
      },
      std::runtime_error);

  (void) std::remove(ruta.c_str());
}

TEST(ConfigTest, DatosExtraLanzanError) {
  std::string const contenido = "gamma: 2.1 2.2 99\n";
  auto const ruta             = crear_archivo_temp(contenido);

  EXPECT_THROW(
      {
        try {
          leer_configuracion(ruta);
        } catch (std::runtime_error const & e) {
          EXPECT_NE(std::string(e.what()).find("Extra data after configuration value for key"),
                    std::string::npos);
          throw;
        }
      },
      std::runtime_error);

  (void) std::remove(ruta.c_str());
}

TEST(ConfigTest, CampoDeVisionFueraDeRango) {
  std::string const contenido = "field_of_view: 190\n";
  auto const ruta             = crear_archivo_temp(contenido);

  EXPECT_THROW(leer_configuracion(ruta), std::runtime_error);

  (void) std::remove(ruta.c_str());
}

TEST(ConfigTest, ArchivoNoExisteLanzaError) {
  EXPECT_THROW(leer_configuracion("archivo_inexistente.txt"), std::runtime_error);
}

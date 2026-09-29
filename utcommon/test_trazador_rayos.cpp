// test_trazador_rayos_google.cpp
#include <gtest/gtest.h>

#include "color.hpp"
#include "configuracion.hpp"
#include "mersenne_twister.hpp"
#include "rayo.hpp"
#include "trazador_rayos.hpp"
#include "vector.hpp"

using namespace render;

// Helper: compara colores con tolerancia
namespace {

  void EXPECT_COLOR_NEAR(color const & expected, color const & actual, double eps = 1e-6) {
    EXPECT_NEAR(expected.obtener_r(), actual.obtener_r(), eps);
    EXPECT_NEAR(expected.obtener_g(), actual.obtener_g(), eps);
    EXPECT_NEAR(expected.obtener_b(), actual.obtener_b(), eps);
  }

}  // namespace

// Caso 1: si la profundidad <= 0 debe devolver negro
TEST(TrazadorRayos_Basico, ProfundidadCeroDevuelveNegro) {
  Configuracion cfg;

  render::color const claro(1.0, 1.0, 1.0);
  render::color const oscuro(0.0, 0.0, 0.0);

  cfg.background_light_color =
      render::vector(claro.obtener_r(), claro.obtener_g(), claro.obtener_b());
  cfg.background_dark_color =
      render::vector(oscuro.obtener_r(), oscuro.obtener_g(), oscuro.obtener_b());

  mersenne_twister mt(13);
  trazador_rayos trazador(cfg, mt);

  vector const dir(0.0, 1.0, 0.0);
  rayo const rayo_origen(vector(0.0, 0.0, 0.0), dir);

  color const resultado = trazador.trazar_rayo(rayo_origen, 0);
  EXPECT_COLOR_NEAR(color(0.0, 0.0, 0.0), resultado);
}

// Caso 2: escena vacía -> color de fondo (tres direcciones)
TEST(TrazadorRayos_Basico, ColorFondoArribaAbajoLateral) {
  Configuracion cfg;

  render::color const claro(1.0, 1.0, 1.0);
  render::color const oscuro(0.0, 0.0, 0.0);

  cfg.background_light_color =
      render::vector(claro.obtener_r(), claro.obtener_g(), claro.obtener_b());
  cfg.background_dark_color =
      render::vector(oscuro.obtener_r(), oscuro.obtener_g(), oscuro.obtener_b());

  mersenne_twister mt(13);
  trazador_rayos trazador(cfg, mt);

  // Dirección hacia arriba: Y = 1 -> t = (1 + 1)/2 = 1 -> oscuro (negro)
  {
    rayo const r_up(vector(0.0, 0.0, 0.0), vector(0.0, 1.0, 0.0));
    color const res_up = trazador.trazar_rayo(r_up, 10);
    EXPECT_COLOR_NEAR(color(0.0, 0.0, 0.0), res_up);
  }

  // Dirección hacia abajo: Y = -1 -> t = 0 -> claro (blanco)
  {
    rayo const r_down(vector(0.0, 0.0, 0.0), vector(0.0, -1.0, 0.0));
    color const res_down = trazador.trazar_rayo(r_down, 10);
    EXPECT_COLOR_NEAR(color(1.0, 1.0, 1.0), res_down);
  }

  // Dirección lateral: Y = 0 -> t = 0.5 -> gris medio (0.5)
  {
    rayo const r_side(vector(0.0, 0.0, 0.0), vector(1.0, 0.0, 0.0));
    color const res_side = trazador.trazar_rayo(r_side, 10);
    EXPECT_COLOR_NEAR(color(0.5, 0.5, 0.5), res_side);
  }
}

// Caso 3: interpolación de fondo arbitraria
TEST(TrazadorRayos_Basico, ColorFondoInterpolacionArbitraria) {
  Configuracion cfg;

  render::color const claro(0.0, 0.0, 1.0);   // azul claro
  render::color const oscuro(1.0, 0.0, 0.0);  // rojo oscuro

  cfg.background_light_color =
      render::vector(claro.obtener_r(), claro.obtener_g(), claro.obtener_b());
  cfg.background_dark_color =
      render::vector(oscuro.obtener_r(), oscuro.obtener_g(), oscuro.obtener_b());

  mersenne_twister mt(13);
  trazador_rayos trazador(cfg, mt);

  // Dirección con Y = 0.5 -> t = 0.75
  // Resultado esperado: (0.75, 0.0, 0.25)
  {
    vector const dir(0.0, 0.5, 0.0);
    rayo const r(vector(0.0, 0.0, 0.0), dir);
    color const res = trazador.trazar_rayo(r, 5);
    EXPECT_COLOR_NEAR(color(0.75, 0.0, 0.25), res, 1e-6);
  }
}

int main(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

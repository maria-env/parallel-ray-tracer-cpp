#include "geometria.hpp"
#include "mersenne_twister.hpp"
#include "rayo.hpp"
#include "tamaño_ventana.hpp"
#include "vector.hpp"
#include "ventana.hpp"
#include <cmath>
#include <gtest/gtest.h>
#include <numbers>

using namespace render;

constexpr double EPS = 1e-6;

// -------------------------------
// TEST 1: Cálculo geométrico base
// -------------------------------
TEST(VentanaModeloMatematico, CalculoVectorFocalYDistanciaFocal) {
  vector const POV(0.0, 0.0, 0.0);
  vector const DEST(0.0, 0.0, -10.0);
  vector const NORTE(0.0, 1.0, 0.0);
  geometria const geo(POV, DEST, NORTE, 60.0);

  // Vector focal: P - D = (0,0,10)
  vector const vf = POV.resta(DEST);
  EXPECT_NEAR(vf.getX(), 0.0, EPS);
  EXPECT_NEAR(vf.getY(), 0.0, EPS);
  EXPECT_NEAR(vf.getZ(), 10.0, EPS);

  // Distancia focal
  double const df = vf.magnitude();
  EXPECT_NEAR(df, 10.0, EPS);

  // Altura de la ventana (hp)
  double const hp = 2.0 * std::tan((60.0 * std::numbers::pi / 180.0) / 2.0) * df;
  EXPECT_NEAR(hp, 11.547, 1e-3);
}

// -------------------------------
// TEST 2: Cálculo de vectores directores u, v
// -------------------------------
TEST(VentanaModeloMatematico, CalculoVectoresU_V) {
  vector const POV(0.0, 0.0, 0.0);
  vector const DEST(0.0, 0.0, -10.0);
  vector const NORTE(0.0, 1.0, 0.0);
  geometria const geo(POV, DEST, NORTE, 60.0);

  vector const vf = POV.resta(DEST).normalizar();
  vector const u  = NORTE.producto_vectorial(vf).normalizar();
  vector const v  = vf.producto_vectorial(u);

  // vf apunta hacia +Z (porque P está detrás de D)
  EXPECT_NEAR(vf.getZ(), 1.0, EPS);

  // u debe apuntar hacia +X
  EXPECT_NEAR(u.getX(), 1.0, EPS);

  // v debe apuntar hacia +Y
  EXPECT_NEAR(v.getY(), 1.0, EPS);
}

// -------------------------------
// TEST 3: Vectores de ventana (p_h y p_v) y su orientación
// -------------------------------
TEST(VentanaModeloMatematico, CalculoVectoresPH_PV) {
  vector const POV(0.0, 0.0, 0.0);
  vector const DEST(0.0, 0.0, -10.0);
  vector const NORTE(0.0, 1.0, 0.0);
  geometria const geo(POV, DEST, NORTE, 90.0);

  tamaño_imagen const tam(800, 600);
  mersenne_twister const mt(0);

  double const df = 10.0;
  double const hp = 2.0 * std::tan((90.0 * std::numbers::pi / 180.0) / 2.0) * df;  // = 20
  double const wp = hp * (800.0 / 600.0);                                          // = 26.6667

  vector const vf = POV.resta(DEST).normalizar();               // (0,0,1)
  vector const u  = NORTE.producto_vectorial(vf).normalizar();  // (1,0,0)
  vector const v  = vf.producto_vectorial(u);                   // (0,1,0)

  vector const p_h = u.producto_constante(wp);
  vector const p_v = v.producto_constante(-hp);

  EXPECT_NEAR(p_h.getX(), 26.6667, 1e-3);
  EXPECT_NEAR(p_v.getY(), -20.0, 1e-3);
}

// -------------------------------
// TEST 4: Cálculo del origen de la ventana (O)
// -------------------------------
TEST(VentanaModeloMatematico, CalculoOrigenVentana) {
  vector const POV(0.0, 0.0, 0.0);
  vector const DEST(0.0, 0.0, -10.0);
  vector const NORTE(0.0, 1.0, 0.0);
  geometria const geo(POV, DEST, NORTE, 90.0);

  tamaño_imagen const tam(800, 600);
  mersenne_twister const mt(0);

  double const df = 10.0;
  double const hp = 2.0 * std::tan((90.0 * std::numbers::pi / 180.0) / 2.0) * df;  // = 20
  double const wp = hp * (800.0 / 600.0);                                          // = 26.6667

  vector const vf     = POV.resta(DEST);
  vector const vf_hat = vf.normalizar();
  vector const u      = NORTE.producto_vectorial(vf_hat).normalizar();
  vector const v      = vf_hat.producto_vectorial(u);

  vector const p_h = u.producto_constante(wp);
  vector const p_v = v.producto_constante(-hp);
  vector const dx  = p_h.producto_constante(1.0 / 800.0);
  vector const dy  = p_v.producto_constante(1.0 / 600.0);

  vector const O = POV.resta(vf)
                       .resta(p_h.producto_constante(0.5))
                       .resta(p_v.producto_constante(0.5))
                       .suma(dx.producto_constante(0.5))
                       .suma(dy.producto_constante(0.5));

  // El origen debe estar centrado respecto al POV pero desplazado hacia (-x,+y,-z)
  EXPECT_LT(O.getX(), 0.0);
  EXPECT_GT(O.getY(), 0.0);
  EXPECT_LT(O.getZ(), 0.0);
}

// -------------------------------
// TEST 5: Generación coherente de rayos
// -------------------------------
TEST(VentanaModeloMatematico, GeneracionRayoDesdePixel) {
  vector const POV(0.0, 0.0, 0.0);
  vector const DEST(0.0, 0.0, -10.0);
  vector const NORTE(0.0, 1.0, 0.0);
  geometria const geo(POV, DEST, NORTE, 60.0);

  tamaño_imagen const tam(400, 300);
  mersenne_twister const mt(42);
  ventana v(geo, tam, mt);

  // Rayo en el centro de la ventana
  rayo const r     = v.generar_rayos_pixel(200, 150);
  vector const dir = r.obtener_direccion();

  // El rayo debe estar normalizado
  EXPECT_NEAR(dir.magnitude(), 1.0, EPS);

  // Debe apuntar aproximadamente hacia -Z (porque POV mira hacia el eje -Z)
  EXPECT_LT(dir.getZ(), 0.0);
}

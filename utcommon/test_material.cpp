#include "material.hpp"
#include "mersenne_twister.hpp"
#include "vector.hpp"
#include <cmath>
#include <gtest/gtest.h>
#include <numbers>

using namespace render;

namespace {

  // Utilidad para comparar vectores con tolerancia
  bool approx_equal(vector const & a, vector const & b, double eps = 1e-6) {
    return std::fabs(a.getX() - b.getX()) < eps and
           std::fabs(a.getY() - b.getY()) < eps and
           std::fabs(a.getZ() - b.getZ()) < eps;
  }

}  // namespace

// -----------------------------------------------------------------------------
// material_mate
// -----------------------------------------------------------------------------

TEST(MaterialMateTest, DireccionSiempreNormalizada) {
  mersenne_twister mt(42);
  material_mate const mat(vector(1.0, 1.0, 1.0));
  vector const normal(0, 1, 0);

  for (int i = 0; i < 50; ++i) {
    vector const dir = mat.calcular_direccion_reflexion(vector(1, 0, 0), normal, true, mt);
    EXPECT_NEAR(dir.magnitude(), 1.0, 1e-9);
  }
}

TEST(MaterialMateTest, DireccionNoCasiNula) {
  mersenne_twister mt(13);
  material_mate const mat(vector(0.8, 0.8, 0.8));
  vector const normal(0, 1, 0);
  vector const dir = mat.calcular_direccion_reflexion(vector(0, 0, 0), normal, true, mt);
  EXPECT_GT(std::fabs(dir.getY()), 1e-3);
}

// -----------------------------------------------------------------------------
// material_metal
// -----------------------------------------------------------------------------

TEST(MaterialMetalTest, ReflexionCorrectaSinDifusion) {
  mersenne_twister mt(7);
  material_metal const mat(vector(1.0, 1.0, 1.0), 0.0);

  vector const incidente(0, -1, 0);  // rayo entra desde arriba
  vector const normal(0, 1, 0);      // plano hacia arriba
  vector const reflejado = mat.calcular_direccion_reflexion(incidente, normal, true, mt);

  EXPECT_TRUE(approx_equal(reflejado, vector(0, 1, 0)));
}

TEST(MaterialMetalTest, ReflexionNormalizadaYDeterminista) {
  mersenne_twister mt1(999);
  mersenne_twister mt2(999);
  material_metal const mat(vector(1.0, 1.0, 1.0), 0.2);

  vector const incidente(0, -1, 0);
  vector const normal(0, 1, 0);

  vector const r1 = mat.calcular_direccion_reflexion(incidente, normal, true, mt1);
  vector const r2 = mat.calcular_direccion_reflexion(incidente, normal, true, mt2);

  EXPECT_NEAR(r1.magnitude(), 1.0, 1e-9);
  EXPECT_TRUE(approx_equal(r1, r2));
}

// -----------------------------------------------------------------------------
// material_refractivo
// -----------------------------------------------------------------------------

TEST(MaterialRefractivoTest, RefraccionPerpendicularNoDesvia) {
  mersenne_twister mt(10);
  material_refractivo const mat(1.5);

  vector const incidente(0, -1, 0);
  vector const normal(0, 1, 0);

  // frente_externo = true porque el rayo entra de aire a material
  vector const dir = mat.calcular_direccion_reflexion(incidente, normal, true, mt);
  EXPECT_TRUE(approx_equal(dir, vector(0, -1, 0), 1e-6));
}

TEST(MaterialRefractivoTest, ReflexionTotalInternaSeCumple) {
  mersenne_twister mt(1);
  material_refractivo const mat(1.5);

  vector const incidente(std::sin(std::numbers::pi / 3), std::cos(std::numbers::pi / 3), 0);

  vector const normal(0, -1, 0);

  vector const dir = mat.calcular_direccion_reflexion(incidente, normal, false, mt);

  EXPECT_LT(dir.getY(), 0.0);  // refleja hacia abajo
  EXPECT_GT(dir.getX(), 0.0);
  EXPECT_NEAR(dir.magnitude(), 1.0, 1e-9);
}

TEST(MaterialRefractivoTest, DireccionSiempreNormalizada) {
  mersenne_twister mt(77);
  material_refractivo const mat(1.3);
  vector const incidente(0.3, -0.95, 0.0);
  vector const normal(0, 1, 0);

  vector const dir = mat.calcular_direccion_reflexion(incidente, normal, true, mt);
  EXPECT_NEAR(dir.magnitude(), 1.0, 1e-9);
}

TEST(MaterialRefractivoTest, CalcularRefraccionAuxiliarFunciona) {
  material_refractivo const mat(1.3);
  vector const incidente(0, -1, 0);
  vector const normal(0, 1, 0);
  vector refractada(0, 0, 0);
  bool const ok = mat.calcular_refraccion(incidente, normal, true, refractada);
  EXPECT_TRUE(ok);
  EXPECT_NEAR(refractada.magnitude(), 1.0, 1e-9);
}

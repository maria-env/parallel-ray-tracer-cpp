#include "cilindro.hpp"
#include "rayo.hpp"
#include "vector.hpp"
#include <gtest/gtest.h>

using namespace render;

TEST(CilindroTest, InterseccionCurva) {
  vector const origen(0.0, 0.0, 0.0);
  vector const direccion(1.0, 0.0, 0.0);
  rayo const r(origen, direccion);

  vector const base(5.0, -1.0, -1.0);
  vector const eje(0.0, 2.0, 2.0);
  double const radio(1.0);
  cilindro const c(base, eje, radio);

  double t             = -1.0;  // Valor por defecto, si existe solución se modifica después
  bool const resultado = c.interseccion(r, t);

  EXPECT_TRUE(resultado);
  EXPECT_NEAR(t, 4.0, 1e-6);
}

TEST(CilindroTest, SinInterseccion) {
  vector const origen(0.0, 0.0, 0.0);
  vector const direccion(0.0, 1.0, 0.0);
  rayo const r(origen, direccion);

  vector const base(5.0, -1.0, -1.0);
  vector const eje(0.0, 2.0, 2.0);
  double const radio(1.0);
  cilindro const c(base, eje, radio);

  double t             = -1.0;  // Valor por defecto, si existe solución se modifica después
  bool const resultado = c.interseccion(r, t);

  EXPECT_FALSE(resultado);
}

TEST(CilindroTest, InterseccionBaseSuperior) {
  vector const origen(0.0, 3.0, 0.0);
  vector const direccion(0.0, -1.0, 0.0);
  rayo const r(origen, direccion);

  vector const centro(0.0, 0.0, 0.0);
  vector const eje(0.0, 2.0, 0.0);
  double const radio = 1.0;
  cilindro const c(centro, eje, radio);

  double t             = -1.0;
  bool const resultado = c.interseccion(r, t);

  EXPECT_TRUE(resultado);
  EXPECT_NEAR(t, 2.0, 1e-6);
}

TEST(CilindroTest, InterseccionBaseInferior) {
  vector const origen(0.0, -3.0, 0.0);
  vector const direccion(0.0, 1.0, 0.0);
  rayo const r(origen, direccion);

  vector const centro(0.0, 0.0, 0.0);
  vector const eje(0.0, 2.0, 0.0);
  double const radio = 1.0;
  cilindro const c(centro, eje, radio);

  double t             = -1.0;
  bool const resultado = c.interseccion(r, t);

  EXPECT_TRUE(resultado);
  EXPECT_NEAR(t, 2.0, 1e-6);
}

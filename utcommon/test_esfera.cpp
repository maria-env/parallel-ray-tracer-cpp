#include "esfera.hpp"
#include "rayo.hpp"
#include "vector.hpp"
#include <gtest/gtest.h>

using namespace render;

TEST(EsferaTest, InterseccionFrontal) {
  vector const origen(0.0, 0.0, 0.0);
  vector const direccion(1.0, 0.0, 0.0);
  rayo const r(origen, direccion);

  vector const centro(5.0, 0.0, 0.0);
  double const radio(1.0);
  esfera const e(centro, radio);

  double t             = -1.0;  // Valor por defecto, si existe solución se modifica después
  bool const resultado = e.interseccion(r, t);

  EXPECT_TRUE(resultado);
  EXPECT_NEAR(t, 4.0, 1e-6);
}

TEST(EsferaTest, SinInterseccion) {
  vector const origen(0.0, 0.0, 0.0);
  vector const direccion(0.0, 1.0, 0.0);
  rayo const r(origen, direccion);

  vector const centro(5.0, 0.0, 0.0);
  double const radio(1.0);
  esfera const e(centro, radio);

  double t             = -1.0;  // Valor por defecto, si existe solución se modifica después
  bool const resultado = e.interseccion(r, t);

  EXPECT_FALSE(resultado);
}

TEST(EsferaTest, InterseccionTangente) {
  vector const origen(0.0, 1.0, 0.0);
  vector const direccion(1.0, 0.0, 0.0);
  rayo const r(origen, direccion);

  vector const centro(5.0, 0.0, 0.0);
  double const radio(1.0);
  esfera const e(centro, radio);

  double t             = -1.0;
  bool const resultado = e.interseccion(r, t);

  EXPECT_TRUE(resultado);
  EXPECT_NEAR(t, 5.0, 1e-6);
}

TEST(EsferaTest, RayoDentroDeLaEsfera) {
  vector const origen(0.0, 0.0, 0.0);
  vector const direccion(1.0, 0.0, 0.0);
  rayo const r(origen, direccion);

  vector const centro(0.0, 0.0, 0.0);
  double const radio(1.0);
  esfera const e(centro, radio);

  double t             = -1.0;
  bool const resultado = e.interseccion(r, t);

  EXPECT_TRUE(resultado);
  EXPECT_NEAR(t, 1.0, 1e-6);
}

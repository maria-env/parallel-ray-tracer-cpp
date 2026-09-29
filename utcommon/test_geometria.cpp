#include "geometria.hpp"
#include "vector.hpp"
#include <cmath>
#include <gtest/gtest.h>
#include <numbers>

using namespace render;

TEST(GeometriaTest, InicializacionBasica) {
  vector const pov(0.0, 0.0, 0.0);
  vector const destino(0.0, 0.0, -1.0);
  vector const norte(0.0, 1.0, 0.0);
  double const fov_grados = 90.0;

  geometria const g(pov, destino, norte, fov_grados);

  auto gPOV   = g.obtener_POV();
  auto gDEST  = g.obtener_DESTINO();
  auto gNORTE = g.obtener_NORTE();

  EXPECT_DOUBLE_EQ(gPOV.getX(), 0.0);
  EXPECT_DOUBLE_EQ(gPOV.getY(), 0.0);
  EXPECT_DOUBLE_EQ(gPOV.getZ(), 0.0);

  EXPECT_DOUBLE_EQ(gDEST.getX(), 0.0);
  EXPECT_DOUBLE_EQ(gDEST.getY(), 0.0);
  EXPECT_DOUBLE_EQ(gDEST.getZ(), -1.0);

  EXPECT_DOUBLE_EQ(gNORTE.getX(), 0.0);
  EXPECT_DOUBLE_EQ(gNORTE.getY(), 1.0);
  EXPECT_DOUBLE_EQ(gNORTE.getZ(), 0.0);

  EXPECT_NEAR(g.obtener_FOV(), std::numbers::pi / 2.0, 1e-9);
}

TEST(GeometriaTest, FOVConversionRadianes) {
  vector const pov(0.0, 0.0, 0.0);
  vector const destino(0.0, 0.0, -1.0);
  vector const norte(0.0, 1.0, 0.0);

  geometria const g1(pov, destino, norte, 0.0);
  EXPECT_NEAR(g1.obtener_FOV(), 0.0, 1e-9);

  geometria const g2(pov, destino, norte, 180.0);
  EXPECT_NEAR(g2.obtener_FOV(), std::numbers::pi, 1e-9);

  geometria const g3(pov, destino, norte, 45.0);
  EXPECT_NEAR(g3.obtener_FOV(), std::numbers::pi / 4.0, 1e-9);
}

TEST(GeometriaTest, AsignacionVectores) {
  vector const pov(1.0, 2.0, 3.0);
  vector const destino(4.0, 5.0, 6.0);
  vector const norte(0.0, 1.0, 0.0);
  double const fov_grados = 60.0;

  geometria const g(pov, destino, norte, fov_grados);

  auto gPOV   = g.obtener_POV();
  auto gDEST  = g.obtener_DESTINO();
  auto gNORTE = g.obtener_NORTE();

  EXPECT_DOUBLE_EQ(gPOV.getX(), 1.0);
  EXPECT_DOUBLE_EQ(gPOV.getY(), 2.0);
  EXPECT_DOUBLE_EQ(gPOV.getZ(), 3.0);

  EXPECT_DOUBLE_EQ(gDEST.getX(), 4.0);
  EXPECT_DOUBLE_EQ(gDEST.getY(), 5.0);
  EXPECT_DOUBLE_EQ(gDEST.getZ(), 6.0);

  EXPECT_DOUBLE_EQ(gNORTE.getX(), 0.0);
  EXPECT_DOUBLE_EQ(gNORTE.getY(), 1.0);
  EXPECT_DOUBLE_EQ(gNORTE.getZ(), 0.0);

  EXPECT_NEAR(g.obtener_FOV(), std::numbers::pi / 3.0, 1e-9);
}

TEST(GeometriaTest, CamposIndependientes) {
  vector pov(0.0, 0.0, 0.0);
  vector destino(1.0, 0.0, 0.0);
  vector norte(0.0, 1.0, 0.0);
  double const fov = 90.0;

  geometria const g(pov, destino, norte, fov);

  // Modificamos los vectores originales
  pov     = vector(99.0, 0.0, 0.0);
  destino = vector(0.0, 99.0, 0.0);
  norte   = vector(0.0, 0.0, 99.0);

  auto gPOV   = g.obtener_POV();
  auto gDEST  = g.obtener_DESTINO();
  auto gNORTE = g.obtener_NORTE();

  EXPECT_DOUBLE_EQ(gPOV.getX(), 0.0);
  EXPECT_DOUBLE_EQ(gDEST.getY(), 0.0);
  EXPECT_DOUBLE_EQ(gNORTE.getZ(), 0.0);
}

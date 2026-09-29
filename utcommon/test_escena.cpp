#include "cilindro.hpp"
#include "escena.hpp"
#include "esfera.hpp"
#include "interseccion.hpp"
#include "rayo.hpp"
#include "vector.hpp"
#include <gtest/gtest.h>

using namespace render;

TEST(EscenaTest, InterseccionConEsfera) {
  escena scn;
  esfera const e(vector(0.0, 0.0, -5.0), 1.0);
  scn.agregar_esfera(e);

  rayo const r(vector(0.0, 0.0, 0.0), vector(0.0, 0.0, -1.0));

  interseccion const inter = scn.interseccion_mas_cercana(r);

  EXPECT_TRUE(inter.obtener_existe());
  EXPECT_NEAR(inter.obtener_t(), 4.0, 1e-6);  // el rayo impacta a z = -4
}

TEST(EscenaTest, InterseccionConCilindro) {
  escena scn;
  cilindro const c(vector(0.0, 0.0, -5.0), vector(0.0, 1.0, 0.0), 1.0);
  scn.agregar_cilindro(c);

  rayo const r(vector(0.0, 0.0, 0.0), vector(0.0, 0.0, -1.0));

  interseccion const inter = scn.interseccion_mas_cercana(r);

  EXPECT_TRUE(inter.obtener_existe());
}

TEST(EscenaTest, SinInterseccion) {
  escena scn;
  esfera const e(vector(10.0, 0.0, 0.0), 1.0);
  scn.agregar_esfera(e);

  rayo const r(vector(0.0, 0.0, 0.0), vector(0.0, 1.0, 0.0));

  interseccion const inter = scn.interseccion_mas_cercana(r);

  EXPECT_FALSE(inter.obtener_existe());
}

TEST(EscenaTest, EligeEsferaMasCercana) {
  escena scn;

  esfera const esfera1(vector(0.0, 0.0, -3.0), 1.0);

  esfera const esfera2(vector(0.0, 0.0, -8.0), 1.0);

  scn.agregar_esfera(esfera1);
  scn.agregar_esfera(esfera2);

  rayo const r(vector(0.0, 0.0, 0.0), vector(0.0, 0.0, -1.0));

  interseccion const inter = scn.interseccion_mas_cercana(r);

  EXPECT_TRUE(inter.obtener_existe());

  EXPECT_NEAR(inter.obtener_t(), 2.0, 1e-6);

  vector const p = inter.obtener_punto();
  EXPECT_NEAR(p.getZ(), -2.0, 1e-6);
}

TEST(EscenaTest, EligeCilindroMasCercano) {
  escena scn;

  cilindro const cilindro1(vector(0.0, 0.0, -4.0), vector(0.0, 1.0, 0.0), 1.0);

  cilindro const cilindro2(vector(0.0, 0.0, -10.0), vector(0.0, 1.0, 0.0), 1.0);

  scn.agregar_cilindro(cilindro1);
  scn.agregar_cilindro(cilindro2);

  rayo const r(vector(0.0, 0.0, 0.0), vector(0.0, 0.0, -1.0));

  interseccion const inter = scn.interseccion_mas_cercana(r);

  EXPECT_TRUE(inter.obtener_existe());

  EXPECT_NEAR(inter.obtener_t(), 3.0, 1e-6);
}

TEST(EscenaTest, EligeEsferaSobreCilindro) {
  escena scn;

  esfera const e(vector(0.0, 0.0, -3.0), 1.0);

  cilindro const c(vector(0.0, 0.0, -8.0), vector(0.0, 1.0, 0.0), 1.0);

  scn.agregar_esfera(e);
  scn.agregar_cilindro(c);

  rayo const r(vector(0.0, 0.0, 0.0), vector(0.0, 0.0, -1.0));

  interseccion const inter = scn.interseccion_mas_cercana(r);

  EXPECT_TRUE(inter.obtener_existe());
  EXPECT_NEAR(inter.obtener_t(), 2.0, 1e-6);

  vector const p = inter.obtener_punto();
  EXPECT_NEAR(p.getZ(), -2.0, 1e-6);
}

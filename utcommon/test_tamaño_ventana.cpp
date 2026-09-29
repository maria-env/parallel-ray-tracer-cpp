#include "tamaño_ventana.hpp"
#include <gtest/gtest.h>

using namespace render;

TEST(TamanoVentanaTest, InicializacionCorrecta) {
  tamaño_imagen const img(800, 600);

  EXPECT_EQ(img.ancho_pixeles, 800);
  EXPECT_EQ(img.alto_pixeles, 600);
}

TEST(TamanoVentanaTest, ValoresCero) {
  tamaño_imagen const img(0, 0);

  EXPECT_EQ(img.ancho_pixeles, 0);
  EXPECT_EQ(img.alto_pixeles, 0);
}

TEST(TamanoVentanaTest, ValoresNegativos) {
  tamaño_imagen const img(-1'920, -1'080);

  EXPECT_EQ(img.ancho_pixeles, -1'920);
  EXPECT_EQ(img.alto_pixeles, -1'080);
}

TEST(TamanoVentanaTest, DiferentesValores) {
  tamaño_imagen const img1(1'024, 768);
  tamaño_imagen const img2(1'920, 1'080);

  EXPECT_EQ(img1.ancho_pixeles, 1'024);
  EXPECT_EQ(img1.alto_pixeles, 768);

  EXPECT_EQ(img2.ancho_pixeles, 1'920);
  EXPECT_EQ(img2.alto_pixeles, 1'080);
}

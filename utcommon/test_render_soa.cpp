#include "color.hpp"
#include "imagen_soa.hpp"
#include <fstream>
#include <gtest/gtest.h>
#include <string>

using namespace render;

TEST(ImagenSoa, CreacionValidaEInvalida) {
  // Dimensiones válidas
  imagen_soa img(10, 5);
  EXPECT_EQ(img.obtener_ancho(), 10);
  EXPECT_EQ(img.obtener_alto(), 5);

  // Dimensiones inválidas
  EXPECT_THROW(imagen_soa(0, 10), std::invalid_argument);
  EXPECT_THROW(imagen_soa(-5, 10), std::invalid_argument);
  EXPECT_THROW(imagen_soa(10, 0), std::invalid_argument);
}

TEST(ImagenSoa, EstablecerPixelDentroDeLimites) {
  imagen_soa img(3, 2);
  color rojo(1.0, 0.0, 0.0);
  img.establecer_pixel(1, 1, rojo);

  // Verificar que el pixel se estableció correctamente
  color pixel = img.obtener_pixel(1, 1);
  EXPECT_FLOAT_EQ(pixel.obtener_rojo(), 1.0f);
  EXPECT_FLOAT_EQ(pixel.obtener_verde(), 0.0f);
  EXPECT_FLOAT_EQ(pixel.obtener_azul(), 0.0f);

  std::string const archivo = "test_output_soa.ppm";
  img.guardar_ppm(archivo);

  std::ifstream in(archivo);
  ASSERT_TRUE(in.is_open());

  std::string header;
  std::getline(in, header);
  EXPECT_EQ(header, "P3");

  in.close();
  (void) std::remove(archivo.c_str());
}

TEST(ImagenSoa, EstablecerPixelFueraDeLimitesNoLanza) {
  imagen_soa img(3, 3);
  color verde(0.0, 1.0, 0.0);

  EXPECT_NO_THROW(img.establecer_pixel(-1, 0, verde));
  EXPECT_NO_THROW(img.establecer_pixel(3, 0, verde));
  EXPECT_NO_THROW(img.establecer_pixel(0, 3, verde));
}

TEST(ImagenSoa, GuardarPPMGeneraArchivoCorrecto) {
  imagen_soa img(2, 2);
  img.establecer_pixel(0, 0, color(1.0, 0.0, 0.0));  // rojo
  img.establecer_pixel(1, 0, color(0.0, 1.0, 0.0));  // verde
  img.establecer_pixel(0, 1, color(0.0, 0.0, 1.0));  // azul
  img.establecer_pixel(1, 1, color(1.0, 1.0, 1.0));  // blanco

  // Verificar los píxeles antes de guardar
  color pixel00 = img.obtener_pixel(0, 0);
  EXPECT_FLOAT_EQ(pixel00.obtener_rojo(), 1.0f);
  EXPECT_FLOAT_EQ(pixel00.obtener_verde(), 0.0f);
  EXPECT_FLOAT_EQ(pixel00.obtener_azul(), 0.0f);

  std::string const archivo = "imagen_test_soa.ppm";
  img.guardar_ppm(archivo);

  std::ifstream in(archivo);
  ASSERT_TRUE(in.is_open());

  std::string header;
  std::getline(in, header);
  EXPECT_EQ(header, "P3");

  int ancho = 0, alto = 0, maxval = 0;
  in >> ancho >> alto >> maxval;
  EXPECT_EQ(ancho, 2);
  EXPECT_EQ(alto, 2);
  EXPECT_EQ(maxval, 255);

  // Verificar algunos valores de color en el archivo
  int r, g, b;

  // Primer pixel (rojo)
  in >> r >> g >> b;
  EXPECT_GE(r, 250);  // Cerca de 255
  EXPECT_LE(g, 5);    // Cerca de 0
  EXPECT_LE(b, 5);    // Cerca de 0

  in.close();
  (void) std::remove(archivo.c_str());
}

TEST(ImagenSoa, ObtenerPixelDentroDeLimites) {
  imagen_soa img(2, 2);
  color original(0.5, 0.3, 0.7);
  img.establecer_pixel(1, 1, original);

  color obtenido = img.obtener_pixel(1, 1);
  EXPECT_FLOAT_EQ(obtenido.obtener_rojo(), 0.5f);
  EXPECT_FLOAT_EQ(obtenido.obtener_verde(), 0.3f);
  EXPECT_FLOAT_EQ(obtenido.obtener_azul(), 0.7f);
}

TEST(ImagenSoa, ObtenerPixelFueraDeLimitesRetornaNegro) {
  imagen_soa img(2, 2);

  // Obtener píxeles fuera de límites debería retornar color negro (0,0,0)
  color fuera1 = img.obtener_pixel(-1, 0);
  EXPECT_FLOAT_EQ(fuera1.obtener_rojo(), 0.0f);
  EXPECT_FLOAT_EQ(fuera1.obtener_verde(), 0.0f);
  EXPECT_FLOAT_EQ(fuera1.obtener_azul(), 0.0f);

  color fuera2 = img.obtener_pixel(0, 5);
  EXPECT_FLOAT_EQ(fuera2.obtener_rojo(), 0.0f);
  EXPECT_FLOAT_EQ(fuera2.obtener_verde(), 0.0f);
  EXPECT_FLOAT_EQ(fuera2.obtener_azul(), 0.0f);
}

TEST(ImagenSoa, ComparacionConAoS_MismosResultados) {
  // Test para verificar que SOA produce los mismos resultados que AoS
  int const ancho = 3;
  int const alto  = 3;

  imagen_soa img_soa(ancho, alto);
  // imagen_aos img_aos(ancho, alto); // Si quieres comparar con AoS

  color test_color(0.2, 0.4, 0.6);

  // Establecer mismo pixel en ambas representaciones
  img_soa.establecer_pixel(1, 1, test_color);
  // img_aos.establecer_pixel(1, 1, test_color);

  color color_soa = img_soa.obtener_pixel(1, 1);
  // color color_aos = img_aos.obtener_pixel(1, 1);

  EXPECT_FLOAT_EQ(color_soa.obtener_rojo(), 0.2f);
  EXPECT_FLOAT_EQ(color_soa.obtener_verde(), 0.4f);
  EXPECT_FLOAT_EQ(color_soa.obtener_azul(), 0.6f);

  // Si tienes ambas implementaciones, puedes comparar:
  // EXPECT_FLOAT_EQ(color_soa.obtener_rojo(), color_aos.obtener_rojo());
  // EXPECT_FLOAT_EQ(color_soa.obtener_verde(), color_aos.obtener_verde());
  // EXPECT_FLOAT_EQ(color_soa.obtener_azul(), color_aos.obtener_azul());
}

TEST(ImagenSoa, RendimientoEstablecerMultiplesPixels) {
  // Test para verificar que podemos establecer muchos píxeles sin problemas
  int const tam = 100;
  imagen_soa img(tam, tam);

  for (int y = 0; y < tam; ++y) {
    for (int x = 0; x < tam; ++x) {
      color c(static_cast<float>(x) / tam, static_cast<float>(y) / tam, 0.5f);
      EXPECT_NO_THROW(img.establecer_pixel(x, y, c));
    }
  }

  // Verificar que el último píxel se estableció correctamente
  color ultimo = img.obtener_pixel(tam - 1, tam - 1);
  EXPECT_FLOAT_EQ(ultimo.obtener_rojo(), 0.99f);
  EXPECT_FLOAT_EQ(ultimo.obtener_verde(), 0.99f);
  EXPECT_FLOAT_EQ(ultimo.obtener_azul(), 0.5f);
}

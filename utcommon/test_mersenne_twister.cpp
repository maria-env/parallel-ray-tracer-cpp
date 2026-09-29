#include "mersenne_twister.hpp"
#include <cmath>
#include <gtest/gtest.h>

using namespace render;

// Test 1: Comprobar que el constructor con semilla produce siempre la misma secuencia
TEST(MersenneTwisterTest, ReproducibilidadConSemilla) {
  unsigned int const seed = 1'234;

  mersenne_twister mt1(seed);
  mersenne_twister mt2(seed);

  // Deben generar exactamente la misma secuencia
  for (int i = 0; i < 5; ++i) {
    double const n1 = mt1.siguiente_numero();
    double const n2 = mt2.siguiente_numero();
    EXPECT_DOUBLE_EQ(n1, n2);
  }
}

// Test 2: Asegurarse de que los valores generados están dentro del rango [-0.5, 0.5]
TEST(MersenneTwisterTest, ValoresEnRango) {
  mersenne_twister mt(42);

  for (int i = 0; i < 1'000; ++i) {
    double const val = mt.siguiente_numero();
    EXPECT_GE(val, -0.5);
    EXPECT_LE(val, 0.5);
  }
}

// Test 3: Verificar que diferentes semillas producen secuencias distintas (alta probabilidad)
TEST(MersenneTwisterTest, SemillasDiferentesGeneranResultadosDiferentes) {
  mersenne_twister mt1(1);
  mersenne_twister mt2(2);

  bool hay_diferencia = false;

  for (int i = 0; i < 10; ++i) {
    if (std::fabs(mt1.siguiente_numero() - mt2.siguiente_numero()) > 1e-9) {
      hay_diferencia = true;
      break;
    }
  }

  EXPECT_TRUE(hay_diferencia);
}

// Test 4: Comprobar que la media aproximada de muchos números se acerca a 0
TEST(MersenneTwisterTest, MediaCercanaACero) {
  mersenne_twister mt(999);
  int const N = 100'000;

  double suma = 0.0;
  for (int i = 0; i < N; ++i) {
    suma += mt.siguiente_numero();
  }

  double const media = suma / N;

  // La media debería estar cerca de 0 (dentro de ±0.01)
  EXPECT_NEAR(media, 0.0, 0.01);
}

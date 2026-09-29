#include <gtest/gtest.h>

#include "vector.hpp"

TEST(test_vector, magnitude_zero) {
  render::vector const vec{0.0, 0.0, 0.0};
  EXPECT_EQ(vec.magnitude(), 0.0);
}

TEST(test_vector, magnitude_positive) {
  render::vector const vec{3.0, 4.0, 0.0};
  EXPECT_EQ(vec.magnitude(), 5.0);
}

TEST(test_vector, suma) {
  render::vector const vec1{1.0, 2.0, 3.0};
  render::vector const vec2{4.0, 5.0, 6.0};
  render::vector const result = vec1.suma(vec2);
  EXPECT_EQ(result.getX(), 5.0);
  EXPECT_EQ(result.getY(), 7.0);
  EXPECT_EQ(result.getZ(), 9.0);
}

TEST(test_vector, resta) {
  render::vector const vec1{4.0, 5.0, 6.0};
  render::vector const vec2{1.0, 2.0, 3.0};
  render::vector const result = vec1.resta(vec2);
  EXPECT_EQ(result.getX(), 3.0);
  EXPECT_EQ(result.getY(), 3.0);
  EXPECT_EQ(result.getZ(), 3.0);
}

TEST(test_vector, producto) {
  render::vector const vec1{1.0, 2.0, 3.0};
  render::vector const vec2{4.0, 5.0, 6.0};
  double const result = vec1.producto_escalar(vec2);
  EXPECT_EQ(result, 32.0);
}

TEST(test_vector, producto_vectorial) {
  render::vector const vec1{1.0, 2.0, 3.0};
  render::vector const vec2{4.0, 5.0, 6.0};
  render::vector const result = vec1.producto_vectorial(vec2);
  EXPECT_EQ(result.getX(), -3.0);
  EXPECT_EQ(result.getY(), 6.0);
  EXPECT_EQ(result.getZ(), -3.0);
}

TEST(test_vector, normalizar) {
  render::vector const vec{3.0, 4.0, 0.0};
  render::vector const result = vec.normalizar();
  EXPECT_NEAR(result.getX(), 0.6, 1e-9);
  EXPECT_NEAR(result.getY(), 0.8, 1e-9);
  EXPECT_NEAR(result.getZ(), 0.0, 1e-9);
}

TEST(test_vector, normalizar_cero) {
  render::vector const vec{0.0, 0.0, 0.0};
  render::vector const result = vec.normalizar();
  EXPECT_EQ(result.getX(), 0.0);
  EXPECT_EQ(result.getY(), 0.0);
  EXPECT_EQ(result.getZ(), 0.0);
}

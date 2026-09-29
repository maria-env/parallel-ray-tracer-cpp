#ifndef RENDER_VECTOR_HPP
#define RENDER_VECTOR_HPP

#include <cmath>

namespace render {

  class vector {
  public:
    vector(double cx, double cy, double cz) : x{cx}, y{cy}, z{cz} { }

    [[nodiscard]] double magnitude() const;

    [[nodiscard]] vector suma(
        vector const & otro_vector) const;  // Para sumar 2 vectores (x11+x21, x12+x22, x13+x23)

    [[nodiscard]] vector resta(
        vector const & otro_vector) const;  // Para sumar 2 vectores (x11-x21, x12-x22, x13-x23)

    [[nodiscard]] double producto_escalar(vector const & otro_vector) const;  // Producto

    [[nodiscard]] vector producto_vectorial(
        vector const & otro_vector) const;  // Obtener vector perpendicular por ej

    [[nodiscard]] vector normalizar() const;  // Normalizar el vector

    [[nodiscard]] vector producto_constante(double escalar) const;

    [[nodiscard]] double getX() const { return x; }

    [[nodiscard]] double getY() const { return y; }

    [[nodiscard]] double getZ() const { return z; }

    //...
  private:
    double x, y, z;
  };

}  // namespace render

#endif

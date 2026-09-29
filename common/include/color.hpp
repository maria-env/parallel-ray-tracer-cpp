#ifndef RENDER_COLOR_HPP
#define RENDER_COLOR_HPP

#include "vector.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>

namespace render {

  class color {
  private:
    double r;
    double g;
    double b;

  public:
    color(double red = 0.0, double green = 0.0, double blue = 0.0) : r{red}, g{green}, b{blue} { }

    explicit color(vector const & v) : r{v.getX()}, g{v.getY()}, b{v.getZ()} { }

    // Regla de los cinco - usar default ya que no hay recursos especiales
    color(color const &)             = default;
    color & operator=(color const &) = default;
    color(color &&)                  = default;
    color & operator=(color &&)      = default;
    ~color()                         = default;

    [[nodiscard]] double obtener_r() const { return r; }

    [[nodiscard]] double obtener_g() const { return g; }

    [[nodiscard]] double obtener_b() const { return b; }

    [[nodiscard]] color suma(color const & otro) const {
      return {r + otro.r, g + otro.g, b + otro.b};
    }

    [[nodiscard]] color multiplicar(color const & otro) const {
      return {r * otro.r, g * otro.g, b * otro.b};
    }

    [[nodiscard]] color escalar(double s) const { return {r * s, g * s, b * s}; }

    [[nodiscard]] color aplicar_gamma(double gamma) const {
      double const inv_gamma = 1.0 / gamma;
      return {std::pow(r, inv_gamma), std::pow(g, inv_gamma), std::pow(b, inv_gamma)};
    }

    [[nodiscard]] uint8_t mapear_r() const {
      return static_cast<uint8_t>(std::clamp(r * 255.0, 0.0, 255.0));
    }

    [[nodiscard]] uint8_t mapear_g() const {
      return static_cast<uint8_t>(std::clamp(g * 255.0, 0.0, 255.0));
    }

    [[nodiscard]] uint8_t mapear_b() const {
      return static_cast<uint8_t>(std::clamp(b * 255.0, 0.0, 255.0));
    }
  };

}  // namespace render

#endif

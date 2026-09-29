#ifndef RENDER_IMAGEN_PAR_HPP
#define RENDER_IMAGEN_PAR_HPP

#include "color.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace render {

  class imagen_par {
  private:
    std::vector<uint8_t> canal_r;
    std::vector<uint8_t> canal_g;
    std::vector<uint8_t> canal_b;
    int ancho;
    int alto;

  public:
    imagen_par(int w, int h);

    // Regla de los cinco
    imagen_par(imagen_par const &)             = default;
    imagen_par & operator=(imagen_par const &) = default;
    imagen_par(imagen_par &&)                  = default;
    imagen_par & operator=(imagen_par &&)      = default;
    ~imagen_par()                              = default;

    void establecer_pixel(int x, int y, color const & c);

    void guardar_ppm(std::string const & archivo) const;

    [[nodiscard]] int obtener_ancho() const { return ancho; }

    [[nodiscard]] int obtener_alto() const { return alto; }
  };

}  // namespace render

#endif

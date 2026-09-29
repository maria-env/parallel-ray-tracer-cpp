#include "imagen_par.hpp"
#include "color.hpp"
#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <string>

namespace render {

  imagen_par::imagen_par(int w, int h) : ancho{w}, alto{h} {
    if (w <= 0 or h <= 0) {
      throw std::invalid_argument("Dimensiones de imagen inválidas");
    }
    auto tam = static_cast<size_t>(w) * static_cast<size_t>(h);
    canal_r.resize(tam, 0);
    canal_g.resize(tam, 0);
    canal_b.resize(tam, 0);
  }

  void imagen_par::establecer_pixel(int x, int y, color const & c) {
    if (x < 0 or x >= ancho or y < 0 or y >= alto) {
      return;
    }
    auto indice     = static_cast<size_t>(y) * static_cast<size_t>(ancho) + static_cast<size_t>(x);
    canal_r[indice] = c.mapear_r();
    canal_g[indice] = c.mapear_g();
    canal_b[indice] = c.mapear_b();
  }

  void imagen_par::guardar_ppm(std::string const & archivo) const {
    std::ofstream salida(archivo);
    if (!salida.is_open()) {
      throw std::runtime_error("No se pudo crear el archivo: " + archivo);
    }

    salida << "P3\n";
    salida << ancho << " " << alto << "\n";
    salida << "255\n";

    auto total = static_cast<size_t>(ancho) * static_cast<size_t>(alto);
    for (size_t i = 0; i < total; ++i) {
      salida << static_cast<int>(canal_r[i]) << " " << static_cast<int>(canal_g[i]) << " "
             << static_cast<int>(canal_b[i]) << "\n";
    }
  }

}  // namespace render

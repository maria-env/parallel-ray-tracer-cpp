#ifndef RENDER_GEOMETRIA_HPP
#define RENDER_GEOMETRIA_HPP

#include "vector.hpp"

namespace render {

  class geometria {
  public:
    vector POV, DESTINO, NORTE;
    double FOV;

    geometria(vector vector_pov, vector vector_destino, vector vector_norte, double fov);

    [[nodiscard]] vector obtener_POV() const { return POV; }

    [[nodiscard]] vector obtener_DESTINO() const { return DESTINO; }

    [[nodiscard]] vector obtener_NORTE() const { return NORTE; }

    [[nodiscard]] double obtener_FOV() const { return FOV; }
  };

}  // namespace render
#endif

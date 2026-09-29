#ifndef RENDER_RAYO_HPP
#define RENDER_RAYO_HPP

#include "vector.hpp"

namespace render {

  class rayo {
  private:
    vector origen, direccion;

  public:
    rayo(vector v_origen, vector v_direccion) : origen{v_origen}, direccion{v_direccion} { }

    [[nodiscard]] vector obtener_origen() const { return origen; }

    [[nodiscard]] vector obtener_direccion() const { return direccion; }

    [[nodiscard]] vector posicion(double distancia) const;
  };

}  // namespace render

#endif

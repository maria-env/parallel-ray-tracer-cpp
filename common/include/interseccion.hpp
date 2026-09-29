#ifndef RENDER_INTERSECCION_HPP
#define RENDER_INTERSECCION_HPP

#include "vector.hpp"

namespace render {

  class interseccion {
  private:
    bool existe         = false;                  // indica si hay o no colisión
    double t            = 0.0;                    // distancia desde el origen del rayo
    vector punto        = vector(0.0, 0.0, 0.0);  // coordenadas del punto de impacto
    vector normal       = vector(0.0, 0.0, 0.0);  // vector normal a la superficie en ese punto
    bool frente_externo = true;  // necesario para luego hacer cálculos en el material refractivo

  public:
    interseccion() = default;

    /* Si le vamos a dar valores desde el principio, usamos este constructor */
    interseccion(bool b_existe, double d_t, vector const & v_punto, vector const & v_normal,
                 bool b_frente_externo = true)
        : existe{b_existe}, t{d_t}, punto{v_punto}, normal{v_normal},
          frente_externo{b_frente_externo} { }

    [[nodiscard]] bool obtener_existe() const { return existe; }

    [[nodiscard]] double obtener_t() const { return t; }

    [[nodiscard]] vector obtener_punto() const { return punto; }

    [[nodiscard]] vector obtener_normal() const { return normal; }

    [[nodiscard]] bool obtener_frente_externo() const { return frente_externo; }
  };

}  // namespace render

#endif

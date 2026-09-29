#ifndef RENDER_ESFERA_HPP
#define RENDER_ESFERA_HPP

#include "rayo.hpp"
#include "vector.hpp"

namespace render {

  class esfera {
  private:
    vector centro;
    double radio;

  public:
    esfera(vector v_centro, double d_radio) : centro{v_centro}, radio{d_radio} { }

    [[nodiscard]] vector obtener_centro() const { return centro; }

    [[nodiscard]] double obtener_radio() const { return radio; }

    [[nodiscard]] bool interseccion(rayo const & r, double & t) const;

    [[nodiscard]] static bool resultado(double lambda1, double lambda2, double & t);
  };

}  // namespace render

#endif

#ifndef RENDER_CILINDRO_HPP
#define RENDER_CILINDRO_HPP

#include "rayo.hpp"
#include "vector.hpp"

namespace render {

  class cilindro {
  private:
    vector base;
    vector eje;
    double radio;
    double altura;

    bool interseccion_curva(rayo const & r, double & t_temp) const;
    bool interseccion_base_superior(rayo const & r, double & t_temp) const;
    bool interseccion_base_inferior(rayo const & r, double & t_temp) const;

    void comprobar_lambda(double lambda, double c_altura, vector const & c_base,
                          vector const & c_eje, vector const & r_origen, vector const & r_direccion,
                          double & t_temp) const;

  public:
    cilindro(vector v_base, vector v_eje, double d_radio)
        : base{v_base}, eje{v_eje.normalizar()}, radio{d_radio}, altura{v_eje.magnitude()} { }

    [[nodiscard]] vector obtener_base() const { return base; }

    [[nodiscard]] vector obtener_eje() const { return eje; }

    [[nodiscard]] double obtener_radio() const { return radio; }

    [[nodiscard]] double obtener_altura() const { return altura; }

    bool interseccion(rayo const & r, double & t) const;
  };

}  // namespace render
#endif

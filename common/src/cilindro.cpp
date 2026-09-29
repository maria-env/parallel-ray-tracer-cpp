#include "cilindro.hpp"
#include "rayo.hpp"
#include "vector.hpp"
#include <algorithm>
#include <cmath>

namespace render {

  bool cilindro::interseccion(rayo const & r, double & t) const {
    double t_temp = INFINITY;

    // LLAMAMOS A LAS TRES PARTES DE LA INTERSECCIÓN
    interseccion_curva(r, t_temp);
    interseccion_base_superior(r, t_temp);
    interseccion_base_inferior(r, t_temp);

    if (t_temp < INFINITY) {
      t = t_temp;
      return true;
    }

    return false;
  }

  bool cilindro::interseccion_curva(
      rayo const & r, double & t_temp) const {      /* Obtenemos los datos del rayo y el cilindro */
    vector const r_origen    = r.obtener_origen();  // O_r
    vector const r_direccion = r.obtener_direccion();  // d_r
    vector const c_base      = obtener_base();         // B
    vector const c_eje       = obtener_eje();          // V (normalizado)
    double const c_radio     = obtener_radio();        // r
    double const c_altura    = obtener_altura();       // h
    vector const r_c         = r_origen.resta(c_base);
    /*Calculo de las perpendiculares*/
    double const r_c_a             = r_c.producto_escalar(c_eje);
    vector const r_c_perpendicular = r_c.resta(c_eje.producto_constante(r_c_a));
    double const d_a               = r_direccion.producto_escalar(c_eje);
    vector const d_perpendicular   = r_direccion.resta(c_eje.producto_constante(d_a));
    /*Parametros para la ec de segundo grado*/
    double const a_curva = d_perpendicular.producto_escalar(d_perpendicular);
    double const b_curva = 2 * d_perpendicular.producto_escalar(r_c_perpendicular);
    double const c_curva =
        r_c_perpendicular.producto_escalar(r_c_perpendicular) - (c_radio * c_radio);
    double const discriminante_curva = (b_curva * b_curva) - (4 * a_curva * c_curva);
    if (discriminante_curva >= 0) {
      double const discriminante_raiz = std::sqrt(discriminante_curva);
      double const lambda1            = (-b_curva - discriminante_raiz) / (2 * a_curva);
      double const lambda2            = (-b_curva + discriminante_raiz) / (2 * a_curva);
      if (lambda1 > 1e-6)
      { /* Comprobamos si las soluciones están dentro de la altura del cilindro */
        vector const punto_interseccion = r_origen.suma(r_direccion.producto_constante(lambda1));
        double const altura_punto       = punto_interseccion.resta(c_base).producto_escalar(c_eje);
        if (altura_punto >= -c_altura / 2.0 and altura_punto <= c_altura / 2.0) {
          t_temp = std::min(t_temp, lambda1);
        }
      }
      if (lambda2 > 1e-6) {
        vector const punto_interseccion = r_origen.suma(r_direccion.producto_constante(lambda2));
        double const altura_punto       = punto_interseccion.resta(c_base).producto_escalar(c_eje);
        if (altura_punto >= c_altura / 2.0 and altura_punto <= c_altura / 2.0) {
          t_temp = std::min(t_temp, lambda2);
        }
      }
    }
    return true;
  }

  bool cilindro::interseccion_base_superior(rayo const & r, double & t_temp) const {
    /*Interseccion con la base superior*/

    vector const r_origen    = r.obtener_origen();
    vector const r_direccion = r.obtener_direccion();

    vector const c_base   = obtener_base();  // Centro del cilindro
    vector const c_eje    = obtener_eje();
    double const c_radio  = obtener_radio();
    double const c_altura = obtener_altura();

    vector const punto_sup = c_base.suma(c_eje.producto_constante(c_altura / 2.0));  // base arriba
    double const denom_sup = r_direccion.producto_escalar(c_eje);

    if (std::abs(denom_sup) > 1e-8) {
      double const lambda = punto_sup.resta(r_origen).producto_escalar(c_eje) / denom_sup;

      if (lambda > 1e-6 and lambda < t_temp) {
        vector const I = r_origen.suma(r_direccion.producto_constante(lambda));
        /* Verificar distancia al centro de la base */
        double const distancia = I.resta(punto_sup).magnitude();
        if (distancia <= c_radio) {
          t_temp = lambda;
        }
      }
    }
    return true;
  }

  bool cilindro::interseccion_base_inferior(rayo const & r, double & t_temp) const {
    /*Interseccion con la base inferior*/

    vector const r_origen    = r.obtener_origen();
    vector const r_direccion = r.obtener_direccion();

    vector const c_base   = obtener_base();  // Centro del cilindro
    vector const c_eje    = obtener_eje();
    double const c_radio  = obtener_radio();
    double const c_altura = obtener_altura();

    vector const punto_inf = c_base.resta(c_eje.producto_constante(c_altura / 2.0));  // base abajo
    double const denom_inf = r_direccion.producto_escalar(c_eje.producto_constante(-1.0));

    if (std::abs(denom_inf) > 1e-8) {
      double const lambda =
          punto_inf.resta(r_origen).producto_escalar(c_eje.producto_constante(-1.0)) / denom_inf;

      if (lambda > 1e-6 and lambda < t_temp) {
        vector const I = r_origen.suma(r_direccion.producto_constante(lambda));
        /* Verificar distancia al centro: ||I - P_inf|| ≤ r */
        double const distancia = I.resta(punto_inf).magnitude();
        if (distancia <= c_radio) {
          t_temp = lambda;
        }
      }
    }
    return true;
  }

}  // namespace render

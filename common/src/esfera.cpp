#include "esfera.hpp"
#include "rayo.hpp"
#include "vector.hpp"
#include <algorithm>
#include <cmath>

namespace render {

  bool esfera::interseccion(rayo const & r, double & t) const {
    /* Obtenemos los datos del rayo y la esfera */
    vector const r_origen    = r.obtener_origen();     // O_r
    vector const r_direccion = r.obtener_direccion();  // d_r

    vector const e_centro = obtener_centro();  // C
    double const e_radio  = obtener_radio();   // r

    /* Calculamos el vector r_c */
    vector const r_c = e_centro.resta(r_origen);

    /* Calculamos los parámetros necesarios para la ecuación de segundo grado */
    double const a        = r_direccion.producto_escalar(r_direccion);
    double const producto = r_direccion.producto_escalar(r_c);
    double const b        = -2.0 * producto;
    double const c        = r_c.producto_escalar(r_c) - (e_radio * e_radio);

    /* Calculamos el valor del discriminante */
    double const discriminante = (b * b) - (4.0 * a * c);

    if (discriminante < 0.0) {
      return false;
    }

    /* Si llegamos aquí es que existe solución, calculamos los valores de lambda */
    double const discriminante_raiz = std::sqrt(discriminante);
    double const lambda1            = (-b - discriminante_raiz) / (2.0 * a);
    double const lambda2            = (-b + discriminante_raiz) / (2.0 * a);

    /* Devolvemos el resultado que nos dé nuestra función solución */
    return resultado(lambda1, lambda2, t);
  }

  bool esfera::resultado(double lambda1, double lambda2, double & t) {
    /* Elegimos el valor más pequeño positivo */
    if (lambda1 >= 0.0 and lambda2 >= 0.0) {
      t = std::min(lambda1, lambda2);
      return true;
    }
    if (lambda1 > 0.0) {
      t = lambda1;
      return true;
    }
    if (lambda2 > 0.0) {
      t = lambda2;
      return true;
    }

    return false;
  }

}  // namespace render

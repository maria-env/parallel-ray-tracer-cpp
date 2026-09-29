#include "material.hpp"
#include "mersenne_twister.hpp"
#include "vector.hpp"
#include <algorithm>
#include <cmath>

namespace render {

  vector material_mate::calcular_direccion_reflexion(vector const &, vector const & normal, bool,
                                                     mersenne_twister & mt) const {
    /* Calculamos coordenadas en el rango [-1, 1] */
    constexpr double SCALE = 2.0;
    double const x         = mt.siguiente_numero() * SCALE;
    double const y         = mt.siguiente_numero() * SCALE;
    double const z         = mt.siguiente_numero() * SCALE;

    /* Sacamos un vector aleatorio con las coordenadas anteriores y sumamos la normal */
    vector const aleatorio(x, y, z);
    vector const direccion = normal.suma(aleatorio);

    /* Si la dirección es nula o casi nula devolvemos la normal */
    constexpr double EPSILON = 1e-8;
    if (std::abs(direccion.getX()) < EPSILON and
        std::abs(direccion.getY()) < EPSILON and
        std::abs(direccion.getZ()) < EPSILON)
    {
      return normal;
    }

    /* En caso contrario, normalizamos el vector dado por la suma */
    return direccion.normalizar();
  }

  vector material_metal::calcular_direccion_reflexion(vector const & dir_incidente,
                                                      vector const & normal, bool,
                                                      mersenne_twister & mt) const {
    /* Calculamos la reflexión inicial */
    double const producto             = dir_incidente.producto_escalar(normal);
    constexpr double REFLECTION_SCALE = 2.0;
    vector reflexion = dir_incidente.resta(normal.producto_constante(REFLECTION_SCALE * producto));

    /* Normalizamos el vector de reflexión */
    reflexion = reflexion.normalizar();

    if (difusion > 0.0) {
      /* Generamos coordenadas en un rango entre −Φ y +Φ aproximadamente */
      constexpr double PERTURBATION_SCALE = 2.0;
      double const x = (mt.siguiente_numero() + 0.5) * PERTURBATION_SCALE * difusion - difusion;
      double const y = (mt.siguiente_numero() + 0.5) * PERTURBATION_SCALE * difusion - difusion;
      double const z = (mt.siguiente_numero() + 0.5) * PERTURBATION_SCALE * difusion - difusion;

      /* Sacamos el vector con las coordenadas y la reflexión final */
      vector const perturbacion(x, y, z);
      reflexion = reflexion.suma(perturbacion);
    }

    return reflexion.normalizar();
  }

  vector material_refractivo::calcular_direccion_reflexion(vector const & dir_incidente,
                                                           vector const & normal,
                                                           bool frente_externo,
                                                           mersenne_twister &) const {
    /* Calculamos el coseno */
    double cos_theta = std::min(-dir_incidente.producto_escalar(normal), 1.0);
    cos_theta        = std::max(cos_theta, 0.0);

    /* Calculamos el seno */
    double const sin_theta = std::sqrt(std::max(0.0, 1.0 - cos_theta * cos_theta));

    /* Determinamos el valor de rho */
    double const rho_prima = frente_externo ? (1.0 / indice_refraccion) : indice_refraccion;

    /* Comprobamos */
    double const lhs = rho_prima * sin_theta;
    if (lhs > 1.0 - 1e-12) {
      double const producto = dir_incidente.producto_escalar(normal);
      return dir_incidente.resta(normal.producto_constante(2.0 * producto));
    }

    /* Caso de refracción, calculamos u y v */
    vector const u =
        dir_incidente.suma(normal.producto_constante(cos_theta)).producto_constante(rho_prima);

    double const u_norm_sq = u.producto_escalar(u);
    double inside          = 1.0 - u_norm_sq;
    inside                 = std::max(inside, 0.0);

    double const factor_v = -std::sqrt(inside);
    vector const v        = normal.producto_constante(factor_v);

    /* Calculamos dr */
    vector dr = u.suma(v);

    return dr;
  }

  bool material_refractivo::calcular_refraccion(vector const & dir_incidente, vector const & normal,
                                                bool hacia_afuera, vector & dir_refractada) const {
    double const ratio = hacia_afuera ? (1.0 / indice_refraccion) : indice_refraccion;

    vector const dir_norm          = dir_incidente;
    constexpr double MAX_COS_THETA = 1.0;
    double const cos_theta         = std::min(-dir_norm.producto_escalar(normal), MAX_COS_THETA);
    double const sin_theta         = std::sqrt(1.0 - cos_theta * cos_theta);

    constexpr double CRITICAL_ANGLE = 1.0;
    if (ratio * sin_theta > CRITICAL_ANGLE) {
      return false;
    }

    vector perpendicular = dir_norm.suma(normal.producto_constante(cos_theta));
    perpendicular        = perpendicular.producto_constante(ratio);

    double const cos_theta_prima = std::sqrt(1.0 - perpendicular.producto_escalar(perpendicular));
    vector const paralelo        = normal.producto_constante(-cos_theta_prima);

    dir_refractada = perpendicular.suma(paralelo);
    return true;
  }

}  // namespace render

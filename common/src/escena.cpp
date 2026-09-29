#include "escena.hpp"
#include "cilindro.hpp"
#include "esfera.hpp"
#include "interseccion.hpp"
#include "rayo.hpp"
#include "vector.hpp"
#include <cmath>
#include <limits>

namespace render {

  void escena::agregar_esfera(esfera const & e) {
    esferas.push_back(e);
  }

  void escena::agregar_cilindro(cilindro const & c) {
    cilindros.push_back(c);
  }

  interseccion escena::interseccion_mas_cercana(rayo const & r) const {
    /* Calculamos la interseccion más cercana de cada tipo */
    interseccion inter_esfera   = interseccion_esferas(r);
    interseccion inter_cilindro = interseccion_cilindros(r);

    /* Si no ha habido ninguna intersección */
    if (!inter_esfera.obtener_existe() and !inter_cilindro.obtener_existe()) {
      return {};  // Se devuelve el default
    }

    if (!inter_cilindro.obtener_existe() or
        (inter_esfera.obtener_existe() and inter_esfera.obtener_t() < inter_cilindro.obtener_t()))
    {
      return inter_esfera;
    }

    return inter_cilindro;
  }

  interseccion escena::interseccion_esferas(rayo const & r) const {
    double t_min               = std::numeric_limits<double>::infinity();
    bool hay_interseccion      = false;
    vector punto_interseccion  = vector(0.0, 0.0, 0.0);
    vector normal_interseccion = vector(0.0, 0.0, 0.0);
    bool frente_externo        = true;  // Por defecto

    /* Calculamos qué esfera tiene la intersección más cercana */
    for (auto const & e : esferas) {
      double t = 0.0;
      if (e.interseccion(r, t) and t > 0 and t < t_min) {
        hay_interseccion = true;
        t_min            = t;

        /* Calculamos la normal */
        vector const I     = r.posicion(t);
        vector const resta = I.resta(e.obtener_centro());
        vector n           = resta.producto_constante(1.0 / e.obtener_radio());

        /* Si es necesario, cambiamos el signo */
        frente_externo = r.obtener_direccion().producto_escalar(n) < 0.0;

        if (!frente_externo) {
          n = n.producto_constante(-1.0);
        }

        punto_interseccion  = I;
        normal_interseccion = n;
      }
    }

    return {hay_interseccion, t_min, punto_interseccion, normal_interseccion, frente_externo};
  }

  interseccion escena::interseccion_cilindros(rayo const & r) const {
    double t_min               = std::numeric_limits<double>::infinity();
    bool hay_interseccion      = false;
    vector punto_interseccion  = vector(0.0, 0.0, 0.0);
    vector normal_interseccion = vector(0.0, 0.0, 0.0);
    bool frente_externo        = true;  // Por defecto

    /* Calculamos qué cilindro tiene la intersección más cercana */
    for (auto const & c : cilindros) {
      double t = 0.0;
      if (c.interseccion(r, t) and t > 0.0 and t < t_min) {
        hay_interseccion = true;
        t_min            = t;

        vector const I = r.posicion(t);
        vector const C = c.obtener_base();
        vector const a = c.obtener_eje();

        /* Normal del cilindro */
        vector const resta1           = I.resta(C);
        double const producto         = resta1.producto_escalar(a);
        vector const producto_escalar = a.producto_constante(producto);
        vector n                      = resta1.resta(producto_escalar).normalizar();

        /* Si es necesario, cambiamos el signo */
        frente_externo = r.obtener_direccion().producto_escalar(n) < 0.0;

        if (!frente_externo) {
          n = n.producto_constante(-1.0);
        }

        punto_interseccion  = I;
        normal_interseccion = n;
      }
    }

    return {hay_interseccion, t_min, punto_interseccion, normal_interseccion, frente_externo};
  }

}  // namespace render

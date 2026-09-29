#include "ventana.hpp"
#include "geometria.hpp"
#include "mersenne_twister.hpp"
#include "rayo.hpp"
#include "tamaño_ventana.hpp"
#include <cmath>

namespace render {

  ventana::ventana(geometria geo, tamaño_imagen tam_img, mersenne_twister mt)
      : geo(geo), tam_img(tam_img), mt(mt) {
    calcular_vector_focal();
    calcular_distancia_focal();
    calcular_dimensiones_ventana();
    calcular_v_focal_normalizado();
    calcular_v_direccion();
    calcular_marco_ventana();
    calcular_delta_x_y();
    calcular_origen_ventana();
  }

  void ventana::calcular_vector_focal() {
    this->v_focal = this->geo.POV.resta(this->geo.DESTINO);
  }

  void ventana::calcular_distancia_focal() {
    this->d_focal = this->v_focal.magnitude();
  }

  void ventana::calcular_dimensiones_ventana() {
    this->tam_ventana_y = 2 * tan(this->geo.FOV / 2) * this->d_focal;
    this->tam_ventana_x = this->tam_ventana_y * ((double) this->tam_img.ancho_pixeles /
                                                 (double) this->tam_img.alto_pixeles);
  }

  void ventana::calcular_v_focal_normalizado() {
    this->v_focal_normalizado = this->v_focal.normalizar();
  }

  void ventana::calcular_v_direccion() {
    vector const v_director_x = this->geo.NORTE.producto_vectorial(v_focal_normalizado);
    this->v_direccion_x       = v_director_x.normalizar();
    this->v_direccion_y       = v_focal_normalizado.producto_vectorial(v_direccion_x);
  }

  void ventana::calcular_marco_ventana() {
    this->v_marco_x = this->v_direccion_x.producto_constante(this->tam_ventana_x);
    this->v_marco_y = this->v_direccion_y.producto_constante(-this->tam_ventana_y);
  }

  void ventana::calcular_delta_x_y() {
    this->delta_x =
        this->v_marco_x.producto_constante(1.0 / (double) (this->tam_img.ancho_pixeles));
    this->delta_y = this->v_marco_y.producto_constante(1.0 / (double) (this->tam_img.alto_pixeles));
  }

  void ventana::calcular_origen_ventana() {
    vector const centro_focal = this->geo.POV.resta(this->v_focal);
    vector const esquina      = (this->v_marco_x.suma(this->v_marco_y)).producto_constante(0.5);
    vector const centrar      = (this->delta_x.suma(this->delta_y)).producto_constante(0.5);
    this->origen_ventana      = centro_focal.resta(esquina).suma(centrar);
  }

  vector ventana::calcular_coordenadas_dispersas(int pixel_x, int pixel_y) {
    vector const desplazamiento_x =
        this->delta_x.producto_constante(pixel_x + this->mt.siguiente_numero());
    vector const desplazamiento_y =
        this->delta_y.producto_constante(pixel_y + this->mt.siguiente_numero());
    vector const desplazamiento_pixel = desplazamiento_x.suma(desplazamiento_y);
    vector const interseccion_ventana = this->origen_ventana.suma(desplazamiento_pixel);
    vector direccion_dispersa         = interseccion_ventana.resta(this->geo.POV);
    return direccion_dispersa;
  }

  rayo ventana::generar_rayos_pixel(int pixel_x, int pixel_y) {
    vector const direccion = calcular_coordenadas_dispersas(pixel_x, pixel_y);
    rayo rayo_generado(this->geo.POV, direccion.normalizar());
    return rayo_generado;
  }

  // Versiones thread-safe que usan un generador externo
  vector ventana::calcular_coordenadas_dispersas_con_mt(int pixel_x, int pixel_y,
                                                        mersenne_twister & mt_local) const {
    vector const desplazamiento_x =
        this->delta_x.producto_constante(pixel_x + mt_local.siguiente_numero());
    vector const desplazamiento_y =
        this->delta_y.producto_constante(pixel_y + mt_local.siguiente_numero());
    vector const desplazamiento_pixel = desplazamiento_x.suma(desplazamiento_y);
    vector const interseccion_ventana = this->origen_ventana.suma(desplazamiento_pixel);
    vector direccion_dispersa         = interseccion_ventana.resta(this->geo.POV);
    return direccion_dispersa;
  }

  rayo ventana::generar_rayos_pixel_con_mt(int pixel_x, int pixel_y,
                                           mersenne_twister & mt_local) const {
    vector const direccion = calcular_coordenadas_dispersas_con_mt(pixel_x, pixel_y, mt_local);
    rayo rayo_generado(this->geo.POV, direccion.normalizar());
    return rayo_generado;
  }

}  // namespace render

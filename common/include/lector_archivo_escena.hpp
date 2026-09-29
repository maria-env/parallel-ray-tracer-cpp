// lector_archivo_escena.hpp
#ifndef LECTOR_ESCENA_HPP
#define LECTOR_ESCENA_HPP

#include "vector.hpp"
#include <string>
#include <vector>

enum tipo_material { MATE, METAL, REFRACTIVO };

struct esfera {
  render::vector centro;
  double radio;
  tipo_material tipo_mat;
  render::vector reflectancia;
  double difusion;
  double indice_refraccion;
};

struct cilindro {
  render::vector centro;
  double radio;
  render::vector eje;
  tipo_material tipo_mat;
  render::vector reflectancia;
  double difusion;
  double indice_refraccion;
};

void leer_escena(std::string const & ruta, std::vector<esfera> & esferas_out,
                 std::vector<cilindro> & cilindros_out);

#endif

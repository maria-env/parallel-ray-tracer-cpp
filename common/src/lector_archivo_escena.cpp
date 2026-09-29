// lector_archivo_escena.cpp
#include "lector_archivo_escena.hpp"
#include "vector.hpp"
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

  struct contexto_parseo {
    std::string linea;
    int num_linea = 0;
  };

  // Estructura temporal para guardar info de un material mientras leemos
  struct material_temporal {
    std::string nombre;
    tipo_material tipo = MATE;
    render::vector reflectancia{0.0, 0.0, 0.0};
    double difusion          = 0.0;
    double indice_refraccion = 0.0;
  };

  void trim(std::string & s) {
    size_t const start = s.find_first_not_of(" \t\r\n");
    size_t const end   = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) {
      s.clear();
    } else {
      s = s.substr(start, end - start + 1);
    }
  }

  // Buscar un material por nombre en el vector (range-based loop)
  material_temporal const * buscar_material(std::vector<material_temporal> const & materiales,
                                            std::string const & nombre) {
    for (material_temporal const & mat : materiales) {
      if (mat.nombre == nombre) {
        return &mat;
      }
    }
    return nullptr;
  }

  void validar_extra(std::istringstream & iss, std::string const & etiqueta,
                     contexto_parseo const & ctx) {
    std::string extra;
    if (iss >> extra) {
      std::string msg = "Error: Extra data after configuration value for key: [";
      msg += etiqueta;
      msg += "]\nExtra: \"";
      msg += extra;
      msg += "\"\nLine: \"";
      msg += ctx.linea;
      msg += "\"";
      throw std::runtime_error(msg);
    }
  }

  void validar_material_duplicado(std::vector<material_temporal> const & materiales,
                                  std::string const & nombre, contexto_parseo const & ctx) {
    if (buscar_material(materiales, nombre) != nullptr) {
      std::string msg = "Error: Material with name [";
      msg += nombre;
      msg += "] already exists\nLine: \"";
      msg += ctx.linea;
      msg += "\"";
      throw std::runtime_error(msg);
    }
  }

  void parsear_mate(std::vector<material_temporal> & materiales, std::istringstream & iss,
                    contexto_parseo const & ctx) {
    std::string nombre;
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;

    if (!(iss >> nombre >> r >> g >> b)) {
      std::string msg = "Error: Invalid matte material parameters\nLine: \"";
      msg += ctx.linea;
      msg += "\"";
      throw std::runtime_error(msg);
    }

    validar_material_duplicado(materiales, nombre, ctx);
    validar_extra(iss, "matte:", ctx);

    material_temporal mat;
    mat.nombre            = nombre;
    mat.tipo              = MATE;
    mat.reflectancia      = render::vector(r, g, b);
    mat.difusion          = 0.0;
    mat.indice_refraccion = 0.0;
    materiales.push_back(mat);
  }

  void parsear_metal(std::vector<material_temporal> & materiales, std::istringstream & iss,
                     contexto_parseo const & ctx) {
    std::string nombre;
    double r        = 0.0;
    double g        = 0.0;
    double b        = 0.0;
    double difusion = 0.0;

    if (!(iss >> nombre >> r >> g >> b >> difusion)) {
      std::string msg = "Error: Invalid metal material parameters\nLine: \"";
      msg += ctx.linea;
      msg += "\"";
      throw std::runtime_error(msg);
    }

    validar_material_duplicado(materiales, nombre, ctx);
    validar_extra(iss, "metal:", ctx);

    material_temporal mat;
    mat.nombre            = nombre;
    mat.tipo              = METAL;
    mat.reflectancia      = render::vector(r, g, b);
    mat.difusion          = difusion;
    mat.indice_refraccion = 0.0;
    materiales.push_back(mat);
  }

  void parsear_refractivo(std::vector<material_temporal> & materiales, std::istringstream & iss,
                          contexto_parseo const & ctx) {
    std::string nombre;
    double indice = 0.0;

    if (!(iss >> nombre >> indice)) {
      std::string msg = "Error: Invalid refractive material parameters\nLine: \"";
      msg += ctx.linea;
      msg += "\"";
      throw std::runtime_error(msg);
    }

    validar_material_duplicado(materiales, nombre, ctx);
    validar_extra(iss, "refractive:", ctx);

    material_temporal mat;
    mat.nombre            = nombre;
    mat.tipo              = REFRACTIVO;
    mat.reflectancia      = render::vector(0.0, 0.0, 0.0);
    mat.difusion          = 0.0;
    mat.indice_refraccion = indice;
    materiales.push_back(mat);
  }

  void parsear_esfera(std::vector<material_temporal> const & materiales,
                      std::vector<esfera> & esferas, std::istringstream & iss,
                      contexto_parseo const & ctx) {
    double cx    = 0.0;
    double cy    = 0.0;
    double cz    = 0.0;
    double radio = 0.0;
    std::string nombre_material;

    if (!(iss >> cx >> cy >> cz >> radio >> nombre_material)) {
      std::string msg = "Error: Invalid sphere parameters\nLine: \"";
      msg += ctx.linea;
      msg += "\"";
      throw std::runtime_error(msg);
    }

    if (radio <= 0.0) {
      std::string msg = "Error: Invalid sphere parameters\nLine: \"";
      msg += ctx.linea;
      msg += "\"";
      throw std::runtime_error(msg);
    }

    material_temporal const * mat = buscar_material(materiales, nombre_material);
    if (mat == nullptr) {
      std::string msg = "Error: Material not found: [";
      msg += nombre_material;
      msg += "]\nLine: \"";
      msg += ctx.linea;
      msg += "\"";
      throw std::runtime_error(msg);
    }

    validar_extra(iss, "sphere:", ctx);

    esfera const nueva_esfera{render::vector(cx, cy, cz), radio,         mat->tipo,
                              mat->reflectancia,          mat->difusion, mat->indice_refraccion};

    esferas.push_back(nueva_esfera);
  }

  void parsear_cilindro(std::vector<material_temporal> const & materiales,
                        std::vector<cilindro> & cilindros, std::istringstream & iss,
                        contexto_parseo const & ctx) {
    double cx    = 0.0;
    double cy    = 0.0;
    double cz    = 0.0;
    double radio = 0.0;
    double ex    = 0.0;
    double ey    = 0.0;
    double ez    = 0.0;
    std::string nombre_material;
    if (!(iss >> cx >> cy >> cz >> radio >> ex >> ey >> ez >> nombre_material)) {
      std::string msg = "Error: Invalid cylinder parameters\nLine: \"";
      msg += ctx.linea;
      msg += "\"";
      throw std::runtime_error(msg);
    }

    if (radio <= 0.0) {
      std::string msg = "Error: Invalid cylinder parameters\nLine: \"";
      msg += ctx.linea;
      msg += "\"";
      throw std::runtime_error(msg);
    }

    material_temporal const * mat = buscar_material(materiales, nombre_material);
    if (mat == nullptr) {
      std::string msg = "Error: Material not found: [";
      msg += nombre_material;
      msg += "]\nLine: \"";
      msg += ctx.linea;
      msg += "\"";
      throw std::runtime_error(msg);
    }

    validar_extra(iss, "cylinder:", ctx);

    cilindro const nuevo_cilindro{
      render::vector(cx, cy, cz), radio,         render::vector(ex, ey, ez), mat->tipo,
      mat->reflectancia,          mat->difusion, mat->indice_refraccion};

    cilindros.push_back(nuevo_cilindro);
  }

  struct parametros_procesamiento {
    std::vector<material_temporal> * materiales;
    std::vector<esfera> * esferas;
    std::vector<cilindro> * cilindros;
  };

  void procesar_etiqueta(parametros_procesamiento & params, std::string const & etiqueta,
                         std::istringstream & iss, contexto_parseo const & ctx) {
    if (etiqueta == "matte:") {
      parsear_mate(*params.materiales, iss, ctx);
    } else if (etiqueta == "metal:") {
      parsear_metal(*params.materiales, iss, ctx);
    } else if (etiqueta == "refractive:") {
      parsear_refractivo(*params.materiales, iss, ctx);
    } else if (etiqueta == "sphere:") {
      parsear_esfera(*params.materiales, *params.esferas, iss, ctx);
    } else if (etiqueta == "cylinder:") {
      parsear_cilindro(*params.materiales, *params.cilindros, iss, ctx);
    } else {
      std::string const entidad = etiqueta.substr(0, etiqueta.length() - 1);
      throw std::runtime_error("Error: Unknown scene entity: " + entidad);
    }
  }

}  // namespace

void leer_escena(std::string const & ruta, std::vector<esfera> & esferas_out,
                 std::vector<cilindro> & cilindros_out) {
  std::vector<material_temporal> materiales;

  std::ifstream archivo(ruta);
  if (!archivo.is_open()) {
    throw std::runtime_error("No se pudo abrir el archivo de escena: " + ruta);
  }

  std::string linea;
  int numero_linea = 0;

  while (std::getline(archivo, linea)) {
    numero_linea++;
    trim(linea);

    if (linea.empty()) {
      continue;
    }

    std::istringstream iss(linea);
    std::string etiqueta;
    iss >> etiqueta;

    if (etiqueta.empty() or etiqueta.back() != ':') {
      std::string const entidad = etiqueta.empty() ? "" : etiqueta;
      throw std::runtime_error("Error: Unknown scene entity: " + entidad);
    }

    contexto_parseo const ctx{linea, numero_linea};
    parametros_procesamiento params{&materiales, &esferas_out, &cilindros_out};
    procesar_etiqueta(params, etiqueta, iss, ctx);
  }
}

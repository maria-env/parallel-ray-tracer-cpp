#ifndef RENDER_MATERIAL_HPP
#define RENDER_MATERIAL_HPP

#include "mersenne_twister.hpp"
#include "vector.hpp"

namespace render {

  enum class tipo_material { MATE, METAL, REFRACTIVO };

  class material {
  protected:
    vector reflectancia;
    tipo_material tipo;

  public:
    material(vector const & refl, tipo_material t) : reflectancia{refl}, tipo{t} { }

    // Regla de los cinco
    material(material const &)             = default;
    material & operator=(material const &) = default;
    material(material &&)                  = default;
    material & operator=(material &&)      = default;
    virtual ~material()                    = default;

    [[nodiscard]] vector obtener_reflectancia() const { return reflectancia; }

    [[nodiscard]] tipo_material obtener_tipo() const { return tipo; }

    [[nodiscard]] virtual vector calcular_direccion_reflexion(vector const & dir_incidente,
                                                              vector const & normal,
                                                              bool frente_externo,
                                                              mersenne_twister & mt) const = 0;
  };

  class material_mate : public material {
  public:
    explicit material_mate(vector const & refl) : material{refl, tipo_material::MATE} { }

    // Regla de los cinco
    material_mate(material_mate const &)             = default;
    material_mate & operator=(material_mate const &) = default;
    material_mate(material_mate &&)                  = default;
    material_mate & operator=(material_mate &&)      = default;
    ~material_mate() override                        = default;

    [[nodiscard]] vector calcular_direccion_reflexion(vector const & dir_incidente,
                                                      vector const & normal, bool frente_externo,
                                                      mersenne_twister & mt) const override;
  };

  class material_metal : public material {
  private:
    double difusion;

  public:
    material_metal(vector const & refl, double dif)
        : material{refl, tipo_material::METAL}, difusion{dif} { }

    // Regla de los cinco
    material_metal(material_metal const &)             = default;
    material_metal & operator=(material_metal const &) = default;
    material_metal(material_metal &&)                  = default;
    material_metal & operator=(material_metal &&)      = default;
    ~material_metal() override                         = default;

    [[nodiscard]] double obtener_difusion() const { return difusion; }

    [[nodiscard]] vector calcular_direccion_reflexion(vector const & dir_incidente,
                                                      vector const & normal, bool frente_externo,
                                                      mersenne_twister & mt) const override;
  };

  class material_refractivo : public material {
  private:
    double indice_refraccion;

  public:
    explicit material_refractivo(double indice)
        : material{vector(1.0, 1.0, 1.0), tipo_material::REFRACTIVO}, indice_refraccion{indice} { }

    // Regla de los cinco
    material_refractivo(material_refractivo const &)             = default;
    material_refractivo & operator=(material_refractivo const &) = default;
    material_refractivo(material_refractivo &&)                  = default;
    material_refractivo & operator=(material_refractivo &&)      = default;
    ~material_refractivo() override                              = default;

    [[nodiscard]] double obtener_indice() const { return indice_refraccion; }

    [[nodiscard]] vector calcular_direccion_reflexion(vector const & dir_incidente,
                                                      vector const & normal, bool frente_externo,
                                                      mersenne_twister & mt) const override;

    [[nodiscard]] bool calcular_refraccion(vector const & dir_incidente, vector const & normal,
                                           bool hacia_afuera, vector & dir_refractada) const;
  };

}  // namespace render

#endif

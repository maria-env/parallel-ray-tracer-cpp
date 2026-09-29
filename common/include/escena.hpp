#ifndef RENDER_ESCENA_HPP
#define RENDER_ESCENA_HPP

#include "cilindro.hpp"
#include "esfera.hpp"
#include "interseccion.hpp"
#include "rayo.hpp"
#include <vector>

namespace render {

  class escena {
  private:
    std::vector<esfera> esferas;      // Lista dinámica de las esferas
    std::vector<cilindro> cilindros;  // Lista dinámica de los cilindros

  public:
    [[nodiscard]] std::vector<esfera> const & obtener_esferas() const { return esferas; }

    [[nodiscard]] std::vector<cilindro> const & obtener_cilindros() const { return cilindros; }

    [[nodiscard]] interseccion interseccion_mas_cercana(rayo const & r) const;

    [[nodiscard]] interseccion interseccion_esferas(rayo const & r) const;

    [[nodiscard]] interseccion interseccion_cilindros(rayo const & r) const;

    void agregar_esfera(esfera const & e);

    void agregar_cilindro(cilindro const & c);
  };

}  // namespace render

#endif

#include "rayo.hpp"
#include "vector.hpp"

namespace render {

  vector rayo::posicion(double distancia) const {
    /* Hacemos producto constante de direccion y distancia */
    vector const producto = direccion.producto_constante(distancia);

    /* Devolvemos la suma */
    return origen.suma(producto);
  }

}  // namespace render

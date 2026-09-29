#include "geometria.hpp"
#include "vector.hpp"
#include <numbers>

namespace render {

  geometria::geometria(vector vector_pov, vector vector_destino, vector vector_norte, double fov)
      : POV(vector_pov), DESTINO(vector_destino), NORTE(vector_norte),
        FOV(fov * (std::numbers::pi / 180)) { }

}  // namespace render

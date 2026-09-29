#ifndef RENDER_MERSENNE_TWISTER_HPP
#define RENDER_MERSENNE_TWISTER_HPP

#include <random>

namespace render {

  class mersenne_twister {
  private:
    std::mt19937_64 estado_mt;

  public:
    // Constructor de clase con una semilla
    explicit mersenne_twister(unsigned int seed);

    // Constructor de copia y movimiento
    mersenne_twister(mersenne_twister const &)             = default;
    mersenne_twister & operator=(mersenne_twister const &) = default;
    mersenne_twister(mersenne_twister &&)                  = default;
    mersenne_twister & operator=(mersenne_twister &&)      = default;
    ~mersenne_twister()                                    = default;

    // Función que retorna el siguiente número aleatorio en [-0.5, 0.5]
    double siguiente_numero();

    // Obtener la semilla actual (para debugging)
    [[nodiscard]] static std::uint64_t obtener_estado();
  };

  class generador_semillas {
  private:
    std::vector<std::uint64_t> semillas;

  public:
    generador_semillas(std::uint64_t semilla_inicial, std::size_t num_semillas);

    [[nodiscard]] std::uint64_t obtener_semilla(std::size_t indice) const;

    [[nodiscard]] std::vector<std::uint64_t> const & obtener_todas() const { return semillas; }

    [[nodiscard]] std::size_t size() const { return semillas.size(); }
  };

}  // namespace render

#endif

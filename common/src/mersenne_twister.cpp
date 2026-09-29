#include "mersenne_twister.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <random>
#include <stdexcept>

namespace render {

  mersenne_twister::mersenne_twister(unsigned int seed) : estado_mt(seed) { }

  double mersenne_twister::siguiente_numero() {
    std::uniform_real_distribution<double> rango(-0.5, 0.5);
    return rango(estado_mt);
  }

  std::uint64_t mersenne_twister::obtener_estado() {
    return 0;
  }

  // Implementación del generador de semillas
  generador_semillas::generador_semillas(std::uint64_t semilla_inicial, std::size_t num_semillas) {
    if (num_semillas == 0) {
      throw std::invalid_argument("El número de semillas debe ser mayor que 0");
    }

    semillas.resize(num_semillas);

    // Usamos un generador Mersenne Twister para generar las semillas
    std::mt19937_64 const seed_gen{semilla_inicial};

    // Generamos cada semilla usando el generador
    std::ranges::generate(semillas, seed_gen);
  }

  std::uint64_t generador_semillas::obtener_semilla(std::size_t indice) const {
    if (indice >= semillas.size()) {
      throw std::out_of_range("Índice de semilla fuera de rango");
    }
    return semillas[indice];
  }

}  // namespace render

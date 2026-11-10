/**
 * @file tm_parser.h
 * @brief Declaración del parser de ficheros de configuración de MT.
 *
 * El parser lee el formato de fichero especificado en la práctica,
 * ignorando líneas de comentario (que empiezan con '#') y líneas vacías.
 *
 * Formato esperado (líneas en orden):
 * @code
 *   # Comentarios opcionales
 *   <estados separados por espacios>
 *   <alfabeto de entrada>
 *   <alfabeto de cinta>
 *   <estado inicial>
 *   <símbolo blanco>
 *   <estados de aceptación>
 *   <número de cintas>
 *   <transición 1>
 *   <transición 2>
 *   ...
 * @endcode
 *
 * Formato de transición para 1 cinta:
 * @code
 *   q_actual simLeido q_sig simEscrito dirección
 * @endcode
 * Para N cintas (N grupos de 3: simLeido simEscrito dirección):
 * @code
 *   q_actual simL1 simL2 ... q_sig simE1 dir1 simE2 dir2 ...
 * @endcode
 *
 * @author  Guillermo Lopez Concepcion
 * @date    18 Oct 2026
 * @note    Asignatura: Complejidad Computacional
 */

#pragma once

#include <set>
#include <string>
#include <vector>

#include "transition.h"

namespace turing {

/**
 * @struct TuringMachineConfig
 * @brief Estructura de datos que almacena la configuración completa de una MT.
 */
struct TuringMachineConfig {
  /// Conjunto de nombres de estados.
  std::set<std::string> states;

  /// Alfabeto de entrada (símbolos válidos como input).
  std::set<char> input_alphabet;

  /// Alfabeto de cinta (incluye el blanco y todos los símbolos usables).
  std::set<char> tape_alphabet;

  /// Nombre del estado inicial.
  std::string initial_state;

  /// Símbolo blanco.
  char blank_symbol{};

  /// Conjunto de nombres de estados de aceptación.
  std::set<std::string> accepting_states;

  /// Número de cintas.
  int num_tapes{1};

  /// Lista de transiciones.
  std::vector<Transition> transitions;
};

/**
 * @class TuringMachineParser
 * @brief Parsea ficheros de configuración de MT y produce un TuringMachineConfig.
 *
 * Cumple el SRP: su única responsabilidad es leer y validar el fichero de
 * configuración. No construye ni ejecuta ninguna MT.
 */
class TuringMachineParser {
 public:
  /**
   * @brief Parsea el fichero en la ruta indicada.
   *
   * @param filepath Ruta al fichero de configuración.
   * @return Configuración completa de la MT.
   * @throws std::runtime_error Si el fichero no se puede abrir o tiene
   *         formato incorrecto.
   */
  TuringMachineConfig Parse(const std::string& filepath) const;

 private:
  /**
   * @brief Devuelve las líneas del fichero que no son comentarios ni vacías.
   *
   * @param filepath Ruta al fichero.
   * @return Vector de líneas significativas.
   */
  std::vector<std::string> ReadSignificantLines(
      const std::string& filepath) const;

  /**
   * @brief Parsea una línea de transición y devuelve un objeto Transition.
   *
   * @param line      Línea de texto con la transición.
   * @param num_tapes Número de cintas (determina el número de tokens esperados).
   * @return Objeto Transition construido.
   * @throws std::runtime_error Si el formato es incorrecto.
   */
  Transition ParseTransition(const std::string& line, int num_tapes) const;
};

}  // namespace turing

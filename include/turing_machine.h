/**
 * @file turing_machine.h
 * @brief Declaración de la clase TuringMachine (Máquina de Turing multicinta).
 *
 * La MT es determinista y soporta movimientos L, R y S en cada cinta de
 * forma independiente. Las cintas son infinitas en ambas direcciones.
 *
 * @author  Guillermo Lopez Concepcion
 * @date    18 Oct 2026
 * @note    Asignatura: Complejidad Computacional
 */

#pragma once

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "tape.h"
#include "tm_parser.h"
#include "transition.h"

namespace turing {

/**
 * @class TuringMachine
 * @brief Simulador de Máquina de Turing determinista multicinta.
 *
 * Recibe una configuración (@c TuringMachineConfig) y ofrece el método
 * @c Run() para ejecutar la MT sobre una cadena de entrada.
 *
 * Variaciones implementadas:
 *  - Movimientos: L (izquierda), R (derecha), S (parada/stay).
 *  - Escritura y movimiento independientes por cinta.
 *  - Cinta infinita en ambas direcciones.
 *
 * Cumple el SRP: sólo ejecuta la MT. La carga de la configuración es
 * responsabilidad de @c TuringMachineParser.
 */
class TuringMachine {
 public:
  /**
   * @brief Construye la MT a partir de una configuración parseada.
   * @param config Configuración completa de la MT.
   */
  explicit TuringMachine(const TuringMachineConfig& config);

  /**
   * @brief Ejecuta la MT sobre una cadena de entrada.
   *
   * Carga @p input en la cinta 0 (el resto permanecen vacías) y simula
   * la MT paso a paso hasta que:
   *  - Se alcanza un estado de aceptación.
   *  - No existe transición aplicable (rechazo).
   *  - Se supera el límite de pasos @p max_steps (prevención de bucles).
   *
   * @param input     Cadena de entrada (puede estar vacía).
   * @param max_steps Número máximo de pasos de simulación (0 = sin límite).
   * @return Verdadero si la MT acepta la cadena.
   */
  bool Run(const std::string& input, size_t max_steps = 0);

  /**
   * @brief Devuelve el contenido de todas las cintas tras la ejecución.
   *
   * El símbolo bajo la cabeza aparece entre corchetes: "ab[c]d".
   *
   * @return Vector de cadenas, una por cinta.
   */
  std::vector<std::string> GetTapeContents() const;

  /**
   * @brief Devuelve el estado en que se detuvo la MT.
   * @return Nombre del estado final (vacío si aún no se ha ejecutado).
   */
  const std::string& GetCurrentState() const;

 private:
  /**
   * @brief Busca la transición aplicable al estado y símbolos actuales.
   *
   * @param current_state Estado actual.
   * @param read_symbols  Símbolos leídos de cada cinta.
   * @return Puntero a la transición encontrada, o nullptr si no existe.
   */
  const Transition* FindTransition(
      const std::string& current_state,
      const std::vector<char>& read_symbols) const;

  /// Número de cintas.
  int num_tapes_;

  /// Cintas de la MT.
  std::vector<Tape> tapes_;

  /// Mapa de transiciones: (estado, [símbolos]) → Transition.
  /// Clave: par (estado_actual, vector_de_símbolos).
  std::map<std::pair<std::string, std::vector<char>>, Transition>
      transition_table_;

  /// Conjunto de nombres de estados de aceptación.
  std::set<std::string> accepting_states_;

  /// Nombre del estado inicial.
  std::string initial_state_;

  /// Estado actual durante/tras la ejecución.
  std::string current_state_;

  /// Símbolo blanco de la MT.
  char blank_symbol_;
};

}  // namespace turing

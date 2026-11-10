/**
 * @file transition.h
 * @brief Definición de la clase Transition (Transición) para MT multicinta.
 *
 * Una transición de MT multicinta lee un símbolo por cinta, escribe un símbolo
 * por cinta y mueve cada cabeza de forma independiente.
 *
 * @author  Guillermo Lopez Concepcion
 * @date    18 Oct 2026
 * @note    Asignatura: Complejidad Computacional
 */

#pragma once

#include <string>
#include <vector>

namespace turing {

/**
 * @struct TapeAction
 * @brief Agrupa la acción sobre una cinta individual: símbolo a escribir y
 *        dirección de movimiento.
 */
struct TapeAction {
  /// Símbolo que se escribe en la cinta.
  char write_symbol;

  /// Dirección de movimiento ('L', 'R' o 'S').
  char direction;
};

/**
 * @class Transition
 * @brief Modela una transición de la Máquina de Turing multicinta determinista.
 *
 * Una transición queda determinada por:
 *  - Estado actual (@p from_state).
 *  - Vector de símbolos leídos, uno por cinta (@p read_symbols).
 *  - Estado siguiente (@p to_state).
 *  - Vector de acciones sobre cada cinta (@p tape_actions).
 *
 * Cumple el SRP: sólo almacena la descripción de una transición.
 */
class Transition {
 public:
  /**
   * @brief Construye una transición completa.
   *
   * @param from_state   Estado de origen.
   * @param read_symbols Símbolos leídos (uno por cinta).
   * @param to_state     Estado de destino.
   * @param tape_actions Acciones por cinta (escritura + movimiento).
   */
  Transition(std::string from_state,
             std::vector<char> read_symbols,
             std::string to_state,
             std::vector<TapeAction> tape_actions);

  /** @return Estado de origen. */
  const std::string& GetFromState() const;

  /** @return Vector de símbolos leídos. */
  const std::vector<char>& GetReadSymbols() const;

  /** @return Estado de destino. */
  const std::string& GetToState() const;

  /** @return Vector de acciones por cinta. */
  const std::vector<TapeAction>& GetTapeActions() const;

 private:
  /// Estado de origen.
  std::string from_state_;

  /// Símbolos leídos, indexados por número de cinta.
  std::vector<char> read_symbols_;

  /// Estado de destino.
  std::string to_state_;

  /// Acciones (escritura + movimiento) por cinta.
  std::vector<TapeAction> tape_actions_;
};

}  // namespace turing

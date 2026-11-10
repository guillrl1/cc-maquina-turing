/**
 * @file transition.cc
 * @brief Implementación de la clase Transition.
 *
 * @author  Guillermo Lopez Concepcion
 * @date    18 Oct 2026
 * @note    Asignatura: Complejidad Computacional
 */

#include "transition.h"

#include <utility>

namespace turing {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

Transition::Transition(std::string from_state,
                       std::vector<char> read_symbols,
                       std::string to_state,
                       std::vector<TapeAction> tape_actions)
    : from_state_(std::move(from_state)),
      read_symbols_(std::move(read_symbols)),
      to_state_(std::move(to_state)),
      tape_actions_(std::move(tape_actions)) {}

// ---------------------------------------------------------------------------
// Getters
// ---------------------------------------------------------------------------

const std::string& Transition::GetFromState() const { return from_state_; }

const std::vector<char>& Transition::GetReadSymbols() const {
  return read_symbols_;
}

const std::string& Transition::GetToState() const { return to_state_; }

const std::vector<TapeAction>& Transition::GetTapeActions() const {
  return tape_actions_;
}

}  // namespace turing

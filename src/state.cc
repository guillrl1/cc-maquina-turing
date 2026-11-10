/**
 * @file state.cc
 * @brief Implementación de la clase State.
 *
 * @author  Guillermo Lopez Concepcion
 * @date    18 Oct 2026
 * @note    Asignatura: Complejidad Computacional
 */

#include "state.h"

namespace turing {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

State::State(std::string name, bool accepting)
    : name_(std::move(name)), is_accepting_(accepting) {}

// ---------------------------------------------------------------------------
// Getters / Setters
// ---------------------------------------------------------------------------

const std::string& State::GetName() const { return name_; }

bool State::IsAccepting() const { return is_accepting_; }

void State::SetAccepting(bool accepting) { is_accepting_ = accepting; }

// ---------------------------------------------------------------------------
// Operadores
// ---------------------------------------------------------------------------

bool State::operator==(const State& other) const {
  return name_ == other.name_;
}

bool State::operator<(const State& other) const {
  return name_ < other.name_;
}

}  // namespace turing

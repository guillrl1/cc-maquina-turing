/**
 * @file turing_machine.cc
 * @brief Implementación de la clase TuringMachine.
 *
 * La finalización de la ejecución viene determinada ÚNICAMENTE por la
 * ausencia de transiciones (o superación del límite de pasos de seguridad).
 * Al detenerse, se comprueba si el estado final pertenece a F (estados de aceptación).
 *
 * @author  Guillermo Lopez Concepcion
 * @date    18 Oct 2026
 * @note    Asignatura: Complejidad Computacional
 */

#include "turing_machine.h"

#include <iostream>
#include <stdexcept>

namespace turing {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

TuringMachine::TuringMachine(const TuringMachineConfig& config)
    : num_tapes_(config.num_tapes),
      accepting_states_(config.accepting_states),
      initial_state_(config.initial_state),
      current_state_(config.initial_state),
      blank_symbol_(config.blank_symbol) {
  // Crear las cintas con el símbolo blanco correcto.
  tapes_.reserve(static_cast<size_t>(num_tapes_));
  for (int i = 0; i < num_tapes_; ++i) {
    tapes_.emplace_back(blank_symbol_);
  }

  // Construir la tabla de transiciones indexada por (estado, [símbolos]).
  for (const auto& t : config.transitions) {
    auto key = std::make_pair(t.GetFromState(), t.GetReadSymbols());
    if (transition_table_.count(key)) {
      throw std::runtime_error(
          "Transición duplicada detectada para el estado '" +
          t.GetFromState() + "': la MT debe ser determinista.");
    }
    transition_table_.emplace(std::move(key), t);
  }
}

// ---------------------------------------------------------------------------
// Run
// ---------------------------------------------------------------------------

bool TuringMachine::Run(const std::string& input, size_t max_steps) {
  // Reinicializar cintas y estado.
  for (auto& tape : tapes_) {
    tape.Reset();
  }
  tapes_[0].LoadInput(input);
  current_state_ = initial_state_;

  size_t steps = 0;

  while (true) {
    // ── Límite de pasos ─────────────────────────────────────────────────────
    if (max_steps > 0 && steps >= max_steps) {
      std::cerr << "[Aviso] Se ha alcanzado el límite de " << max_steps
                << " pasos. Simulación detenida (rechazo por timeout).\n";
      return false;
    }

    // ── Leer símbolo de cada cinta ──────────────────────────────────────────
    std::vector<char> read_symbols;
    read_symbols.reserve(static_cast<size_t>(num_tapes_));
    for (const auto& tape : tapes_) {
      read_symbols.push_back(tape.Read());
    }

    // ── Buscar transición aplicable ─────────────────────────────────────────
    const Transition* trans = FindTransition(current_state_, read_symbols);
    if (trans == nullptr) {
      // Fin de la ejecución: determinada únicamente por ausencia de transiciones.
      // Se acepta si y sólo si el estado actual está en el conjunto F.
      return accepting_states_.count(current_state_) > 0;
    }

    // ── Aplicar transición ──────────────────────────────────────────────────
    const auto& actions = trans->GetTapeActions();
    for (int i = 0; i < num_tapes_; ++i) {
      tapes_[static_cast<size_t>(i)].Write(actions[static_cast<size_t>(i)].write_symbol);
      tapes_[static_cast<size_t>(i)].Move(actions[static_cast<size_t>(i)].direction);
    }
    current_state_ = trans->GetToState();
    ++steps;
  }
}

// ---------------------------------------------------------------------------
// Consultas
// ---------------------------------------------------------------------------

std::vector<std::string> TuringMachine::GetTapeContents() const {
  std::vector<std::string> contents;
  contents.reserve(static_cast<size_t>(num_tapes_));
  for (const auto& tape : tapes_) {
    contents.push_back(tape.ToString());
  }
  return contents;
}

const std::string& TuringMachine::GetCurrentState() const {
  return current_state_;
}

// ---------------------------------------------------------------------------
// Privado
// ---------------------------------------------------------------------------

const Transition* TuringMachine::FindTransition(
    const std::string& current_state,
    const std::vector<char>& read_symbols) const {
  auto it = transition_table_.find({current_state, read_symbols});
  if (it == transition_table_.end()) {
    return nullptr;
  }
  return &it->second;
}

}  // namespace turing

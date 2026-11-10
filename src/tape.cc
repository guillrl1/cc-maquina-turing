/**
 * @file tape.cc
 * @brief Implementación de la clase Tape.
 *
 * @author  Guillermo Lopez Concepcion
 * @date    18 Oct 2026
 * @note    Asignatura: Complejidad Computacional
 */

#include "tape.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace turing {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

Tape::Tape(char blank) : blank_symbol_(blank), head_position_(0) {}

// ---------------------------------------------------------------------------
// Operaciones públicas
// ---------------------------------------------------------------------------

void Tape::LoadInput(const std::string& input) {
  Reset();
  for (int i = 0; i < static_cast<int>(input.size()); ++i) {
    cells_[i] = input[static_cast<size_t>(i)];
  }
}

char Tape::Read() const {
  auto it = cells_.find(head_position_);
  if (it == cells_.end()) {
    return blank_symbol_;
  }
  return it->second;
}

void Tape::Write(char symbol) {
  cells_[head_position_] = symbol;
}

void Tape::Move(char direction) {
  switch (direction) {
    case 'L':
      --head_position_;
      break;
    case 'R':
      ++head_position_;
      break;
    case 'S':
      // Sin movimiento.
      break;
    default:
      throw std::invalid_argument(
          std::string("Dirección de movimiento inválida: ") + direction);
  }
}

int Tape::GetHeadPosition() const { return head_position_; }

std::string Tape::ToString() const {
  if (cells_.empty()) {
    // Cinta vacía: sólo mostrar el símbolo blanco bajo la cabeza.
    std::string result;
    result += '[';
    result += blank_symbol_;
    result += ']';
    return result;
  }

  // Rango visible: desde la posición mínima ocupada hasta la máxima,
  // ampliado para incluir siempre la posición de la cabeza.
  int min_pos = std::min(cells_.begin()->first, head_position_);
  int max_pos = std::max(cells_.rbegin()->first, head_position_);

  // Recortar símbolos blancos a la izquierda que queden antes de la posición
  // de la cabeza, para no mostrar blancos iniciales innecesarios.
  while (min_pos < head_position_) {
    auto it = cells_.find(min_pos);
    if (it != cells_.end() && it->second == blank_symbol_) {
      ++min_pos;
    } else {
      break;
    }
  }

  std::ostringstream oss;
  for (int pos = min_pos; pos <= max_pos; ++pos) {
    auto it = cells_.find(pos);
    char sym = (it != cells_.end()) ? it->second : blank_symbol_;

    if (pos == head_position_) {
      oss << '[' << sym << ']';
    } else {
      oss << sym;
    }
  }
  return oss.str();
}

void Tape::Reset() {
  cells_.clear();
  head_position_ = 0;
}

}  // namespace turing

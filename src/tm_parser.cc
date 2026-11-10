/**
 * @file tm_parser.cc
 * @brief Implementación del parser de ficheros de configuración de MT.
 *
 * El parser ignora comentarios (cualquier texto tras '#' hasta el final de línea)
 * y líneas vacías. Valida formalmente todas las restricciones matemáticas de una MT:
 *  - s ∈ Q
 *  - F ⊆ Q
 *  - b ∈ Γ
 *  - b ∉ Σ
 *  - Σ ⊆ Γ
 *  - q_from, q_to ∈ Q para cada transición
 *  - símbolos leídos y escritos ∈ Γ
 *  - direcciones ∈ {L, R, S}
 *  - determinismo (sin transiciones duplicadas)
 *
 * @author  Guillermo Lopez Concepcion
 * @date    18 Oct 2026
 * @note    Asignatura: Complejidad Computacional
 */

#include "tm_parser.h"

#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace turing {

// ---------------------------------------------------------------------------
// Helpers privados
// ---------------------------------------------------------------------------

std::vector<std::string> TuringMachineParser::ReadSignificantLines(
    const std::string& filepath) const {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    throw std::runtime_error("No se puede abrir el fichero: " + filepath);
  }

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(file, line)) {
    // Eliminar retorno de carro (\r) para compatibilidad con ficheros Windows.
    if (!line.empty() && line.back() == '\r') {
      line.pop_back();
    }
    // Eliminar comentarios (desde '#' hasta fin de línea).
    auto comment_pos = line.find('#');
    if (comment_pos != std::string::npos) {
      line = line.substr(0, comment_pos);
    }
    // Comprobar si la línea resultante sólo contiene espacios en blanco.
    bool all_whitespace = true;
    for (char c : line) {
      if (!std::isspace(static_cast<unsigned char>(c))) {
        all_whitespace = false;
        break;
      }
    }
    if (all_whitespace) {
      continue;
    }
    lines.push_back(line);
  }
  return lines;
}

Transition TuringMachineParser::ParseTransition(const std::string& line,
                                                int num_tapes) const {
  std::istringstream iss(line);
  std::vector<std::string> tokens;
  std::string token;
  while (iss >> token) {
    tokens.push_back(token);
  }

  // Para N cintas la línea tiene: q_from  (sim_leido)×N  q_to  (sim_escrito dir)×N
  // Total de tokens: 1 + N + 1 + 2*N = 2 + 3*N
  const int expected = 2 + 3 * num_tapes;
  if (static_cast<int>(tokens.size()) != expected) {
    throw std::runtime_error(
        "Transición con formato incorrecto (se esperaban " +
        std::to_string(expected) + " tokens, se obtuvieron " +
        std::to_string(tokens.size()) + "): " + line);
  }

  std::string from_state = tokens[0];

  // Símbolos leídos: tokens [1 .. num_tapes]
  std::vector<char> read_symbols;
  for (int i = 0; i < num_tapes; ++i) {
    const std::string& sym = tokens[static_cast<size_t>(1 + i)];
    if (sym.size() != 1) {
      throw std::runtime_error(
          "Símbolo de lectura inválido (debe ser un carácter): " + sym);
    }
    read_symbols.push_back(sym[0]);
  }

  std::string to_state = tokens[static_cast<size_t>(1 + num_tapes)];

  // Acciones por cinta: tokens [2+num_tapes .. 2+3*num_tapes-1] en pares
  std::vector<TapeAction> tape_actions;
  for (int i = 0; i < num_tapes; ++i) {
    const std::string& write_sym = tokens[static_cast<size_t>(2 + num_tapes + 2 * i)];
    const std::string& dir_str   = tokens[static_cast<size_t>(2 + num_tapes + 2 * i + 1)];

    if (write_sym.size() != 1) {
      throw std::runtime_error(
          "Símbolo de escritura inválido (debe ser un carácter): " + write_sym);
    }
    if (dir_str.size() != 1 ||
        (dir_str[0] != 'L' && dir_str[0] != 'R' && dir_str[0] != 'S')) {
      throw std::runtime_error(
          "Dirección inválida (debe ser L, R o S): " + dir_str);
    }

    tape_actions.push_back({write_sym[0], dir_str[0]});
  }

  return Transition(std::move(from_state), std::move(read_symbols),
                    std::move(to_state), std::move(tape_actions));
}

// ---------------------------------------------------------------------------
// Parse público y validación formal
// ---------------------------------------------------------------------------

TuringMachineConfig TuringMachineParser::Parse(
    const std::string& filepath) const {
  std::vector<std::string> lines = ReadSignificantLines(filepath);

  if (lines.size() < 7) {
    throw std::runtime_error(
        "El fichero de configuración tiene menos secciones de las requeridas (mínimo 7).");
  }

  TuringMachineConfig cfg;
  size_t idx = 0;

  // Línea 0: Conjunto de estados Q.
  {
    std::istringstream iss(lines[idx++]);
    std::string s;
    while (iss >> s) {
      cfg.states.insert(s);
    }
    if (cfg.states.empty()) {
      throw std::runtime_error("El conjunto de estados Q no puede estar vacío.");
    }
  }

  // Línea 1: Alfabeto de entrada Σ.
  {
    std::istringstream iss(lines[idx++]);
    std::string s;
    while (iss >> s) {
      if (s.size() != 1) {
        throw std::runtime_error(
            "Símbolo de alfabeto de entrada inválido: " + s);
      }
      cfg.input_alphabet.insert(s[0]);
    }
    if (cfg.input_alphabet.empty()) {
      throw std::runtime_error("El alfabeto de entrada Σ no puede estar vacío.");
    }
  }

  // Línea 2: Alfabeto de la cinta Γ.
  {
    std::istringstream iss(lines[idx++]);
    std::string s;
    while (iss >> s) {
      if (s.size() != 1) {
        throw std::runtime_error(
            "Símbolo de alfabeto de cinta inválido: " + s);
      }
      cfg.tape_alphabet.insert(s[0]);
    }
    if (cfg.tape_alphabet.empty()) {
      throw std::runtime_error("El alfabeto de cinta Γ no puede estar vacío.");
    }
  }

  // Línea 3: Estado inicial s.
  {
    std::istringstream iss(lines[idx++]);
    iss >> cfg.initial_state;
    if (cfg.initial_state.empty()) {
      throw std::runtime_error("No se especificó un estado inicial.");
    }
  }

  // Línea 4: Símbolo blanco b.
  {
    std::istringstream iss(lines[idx++]);
    std::string s;
    iss >> s;
    if (s.size() != 1) {
      throw std::runtime_error("Símbolo blanco inválido: " + s);
    }
    cfg.blank_symbol = s[0];
  }

  // Línea 5: Estados de aceptación F.
  {
    std::istringstream iss(lines[idx++]);
    std::string s;
    while (iss >> s) {
      cfg.accepting_states.insert(s);
    }
  }

  // Línea 6: Número de cintas.
  {
    std::istringstream iss(lines[idx++]);
    iss >> cfg.num_tapes;
    if (cfg.num_tapes < 1) {
      throw std::runtime_error("El número de cintas debe ser >= 1.");
    }
  }

  // Líneas restantes: transiciones.
  while (idx < lines.size()) {
    cfg.transitions.push_back(ParseTransition(lines[idx++], cfg.num_tapes));
  }

  // ── Validación formal de la Máquina de Turing ─────────────────────────────
  // 1. s ∈ Q
  if (cfg.states.find(cfg.initial_state) == cfg.states.end()) {
    throw std::runtime_error(
        "Violación formal: el estado inicial '" + cfg.initial_state +
        "' no pertenece a Q.");
  }

  // 2. F ⊆ Q
  for (const auto& acc : cfg.accepting_states) {
    if (cfg.states.find(acc) == cfg.states.end()) {
      throw std::runtime_error(
          "Violación formal: el estado de aceptación '" + acc +
          "' no pertenece a Q.");
    }
  }

  // 3. b ∈ Γ
  if (cfg.tape_alphabet.find(cfg.blank_symbol) == cfg.tape_alphabet.end()) {
    throw std::runtime_error(
        std::string("Violación formal: el símbolo blanco '") +
        cfg.blank_symbol + "' no pertenece a Γ.");
  }

  // 4. b ∉ Σ
  if (cfg.input_alphabet.find(cfg.blank_symbol) != cfg.input_alphabet.end()) {
    throw std::runtime_error(
        std::string("Violación formal: el símbolo blanco '") +
        cfg.blank_symbol + "' no puede pertenecer a Σ.");
  }

  // 5. Σ ⊆ Γ
  for (char sym : cfg.input_alphabet) {
    if (cfg.tape_alphabet.find(sym) == cfg.tape_alphabet.end()) {
      throw std::runtime_error(
          std::string("Violación formal: el símbolo de entrada '") +
          sym + "' no pertenece al alfabeto de cinta Γ.");
    }
  }

  // 6. Validar transiciones respecto a Q y Γ
  for (const auto& t : cfg.transitions) {
    if (cfg.states.find(t.GetFromState()) == cfg.states.end()) {
      throw std::runtime_error(
          "Violación formal en transición: el estado origen '" +
          t.GetFromState() + "' no pertenece a Q.");
    }
    if (cfg.states.find(t.GetToState()) == cfg.states.end()) {
      throw std::runtime_error(
          "Violación formal en transición: el estado destino '" +
          t.GetToState() + "' no pertenece a Q.");
    }
    for (char sym : t.GetReadSymbols()) {
      if (cfg.tape_alphabet.find(sym) == cfg.tape_alphabet.end()) {
        throw std::runtime_error(
            std::string("Violación formal en transición: símbolo leído '") +
            sym + "' no pertenece a Γ.");
      }
    }
    for (const auto& act : t.GetTapeActions()) {
      if (cfg.tape_alphabet.find(act.write_symbol) == cfg.tape_alphabet.end()) {
        throw std::runtime_error(
            std::string("Violación formal en transición: símbolo escrito '") +
            act.write_symbol + "' no pertenece a Γ.");
      }
    }
  }

  return cfg;
}

}  // namespace turing

/**
 * @file main.cc
 * @brief Punto de entrada del simulador de Máquina de Turing multicinta.
 *
 * Uso:
 * @code
 *   ./turing_machine -config <fichero.mt> [-in <fichero_entrada>]
 * @endcode
 *
 * Si se proporciona `-in`, se ejecutan todas las cadenas contenidas en el fichero
 * (una por línea). Si no se proporciona `-in`, se leen las cadenas desde teclado
 * hasta final de entrada (EOF / Ctrl+D).
 *
 * @author  Guillermo Lopez Concepcion
 * @date    18 Oct 2026
 * @note    Asignatura: Complejidad Computacional
 */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "tm_parser.h"
#include "turing_machine.h"

// ---------------------------------------------------------------------------
// Helpers de parseo de argumentos
// ---------------------------------------------------------------------------

/**
 * @brief Extrae el valor de un argumento de línea de comandos.
 *
 * Busca @p flag en @p argv y devuelve el token siguiente.
 *
 * @param argc    Número de argumentos.
 * @param argv    Vector de argumentos.
 * @param flag    Flag a buscar (e.g., "-config").
 * @return        Valor asociado al flag, o cadena vacía si no se encontró.
 */
static std::string GetArgValue(int argc, char* argv[],
                               const std::string& flag) {
  for (int i = 1; i < argc - 1; ++i) {
    if (std::string(argv[i]) == flag) {
      return std::string(argv[i + 1]);
    }
  }
  return {};
}

/**
 * @brief Comprueba si un flag está presente en los argumentos.
 * @param argc Número de argumentos.
 * @param argv Vector de argumentos.
 * @param flag Flag a buscar.
 * @return Verdadero si el flag aparece en argv.
 */
static bool HasFlag(int argc, char* argv[], const std::string& flag) {
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == flag) {
      return true;
    }
  }
  return false;
}

/**
 * @brief Ejecuta la MT sobre una cadena de entrada e imprime el resultado.
 *
 * @param tm           Referencia a la Máquina de Turing construida.
 * @param input_string Cadena de entrada a evaluar.
 * @param index        Número ordinal de la prueba (1-indexado).
 * @return Verdadero si la cadena fue aceptada.
 */
static bool SimulateAndReport(turing::TuringMachine& tm,
                              const std::string& input_string,
                              size_t index) {
  constexpr size_t kMaxSteps = 500'000;
  bool accepted = false;
  try {
    accepted = tm.Run(input_string, kMaxSteps);
  } catch (const std::exception& e) {
    std::cerr << "[Error durante la ejecución] " << e.what() << '\n';
    return false;
  }

  std::cout << "\n--- Ejecución #" << index << " ---\n";
  std::cout << "Cadena de entrada : \"" << input_string << "\"\n";
  std::cout << "Estado de parada  : " << tm.GetCurrentState() << "\n";
  std::cout << "Resultado         : "
            << (accepted ? "ACEPTADA (estado ∈ F) ✓" : "RECHAZADA (estado ∉ F) ✗")
            << "\n";

  const auto tape_contents = tm.GetTapeContents();
  for (size_t i = 0; i < tape_contents.size(); ++i) {
    std::cout << "Cinta " << (i + 1) << "           : " << tape_contents[i]
              << "\n";
  }

  return accepted;
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------

int main(int argc, char* argv[]) {
  // ── Validación de argumentos ───────────────────────────────────────────────
  if (HasFlag(argc, argv, "-h") || HasFlag(argc, argv, "--help")) {
    std::cout
        << "Uso: " << argv[0]
        << " -config <fichero.mt> [-in <fichero_entrada>]\n\n"
        << "  -config <f>   Fichero de configuración de la MT (obligatorio).\n"
        << "  -in     <f>   Fichero con una o varias cadenas de entrada (opcional).\n"
        << "                Si se omite, se leen interactivamente de teclado.\n";
    return 0;
  }

  const std::string config_file = GetArgValue(argc, argv, "-config");
  if (config_file.empty()) {
    std::cerr << "[Error] Se requiere el argumento -config <fichero.mt>\n"
              << "Uso: " << argv[0]
              << " -config <fichero.mt> [-in <fichero_entrada>]\n";
    return 1;
  }

  // ── Parsear configuración ─────────────────────────────────────────────────
  turing::TuringMachineConfig config;
  try {
    turing::TuringMachineParser parser;
    config = parser.Parse(config_file);
  } catch (const std::exception& e) {
    std::cerr << "[Error al parsear configuración] " << e.what() << '\n';
    return 1;
  }

  // ── Construir la MT ───────────────────────────────────────────────────────
  turing::TuringMachine tm(config);

  // ── Procesar cadenas de entrada ───────────────────────────────────────────
  const std::string input_file = GetArgValue(argc, argv, "-in");

  if (!input_file.empty()) {
    std::ifstream fin(input_file);
    if (!fin.is_open()) {
      std::cerr << "[Error] No se puede abrir el fichero de entrada: "
                << input_file << '\n';
      return 1;
    }

    std::string line;
    size_t count = 0;
    while (std::getline(fin, line)) {
      if (!line.empty() && line.back() == '\r') {
        line.pop_back();
      }
      ++count;
      SimulateAndReport(tm, line, count);
    }

    if (count == 0) {
      std::cout << "[Aviso] El fichero de entrada estaba vacío.\n";
    }
  } else {
    std::cout << "Introduce las cadenas de entrada (una por línea, Ctrl+D para finalizar):\n";
    std::string line;
    size_t count = 0;
    while (std::cout << "> " && std::getline(std::cin, line)) {
      if (!line.empty() && line.back() == '\r') {
        line.pop_back();
      }
      ++count;
      SimulateAndReport(tm, line, count);
    }
  }

  return 0;
}

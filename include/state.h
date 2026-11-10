/**
 * @file state.h
 * @brief Definición de la clase State (Estado) de la Máquina de Turing.
 *
 * Un estado es simplemente un identificador de cadena con una bandera
 * que indica si es estado de aceptación.
 *
 * @author  Guillermo Lopez Concepcion
 * @date    18 Oct 2026
 * @note    Asignatura: Complejidad Computacional
 */

#pragma once

#include <string>

namespace turing {

/**
 * @class State
 * @brief Representa un estado de la Máquina de Turing.
 *
 * Encapsula el nombre del estado y su condición de aceptación.
 * Cumple el principio de responsabilidad única (SRP): sólo gestiona
 * la identidad y el tipo de un estado.
 */
class State {
 public:
  /**
   * @brief Construye un estado con nombre y condición de aceptación.
   * @param name      Identificador del estado (e.g., "q0", "q_accept").
   * @param accepting Verdadero si el estado es de aceptación.
   */
  explicit State(std::string name, bool accepting = false);

  /**
   * @brief Devuelve el nombre del estado.
   * @return Referencia constante al nombre.
   */
  const std::string& GetName() const;

  /**
   * @brief Indica si el estado es de aceptación.
   * @return Verdadero si es estado de aceptación.
   */
  bool IsAccepting() const;

  /**
   * @brief Establece si el estado es de aceptación.
   * @param accepting Nuevo valor.
   */
  void SetAccepting(bool accepting);

  /**
   * @brief Operador de comparación por nombre.
   * @param other Otro estado a comparar.
   * @return Verdadero si tienen el mismo nombre.
   */
  bool operator==(const State& other) const;

  /**
   * @brief Operador menor-que (necesario para uso en std::map/std::set).
   * @param other Otro estado a comparar.
   * @return Verdadero si el nombre de este estado es lexicográficamente menor.
   */
  bool operator<(const State& other) const;

 private:
  /// Nombre/identificador del estado.
  std::string name_;

  /// Indica si el estado es de aceptación.
  bool is_accepting_;
};

}  // namespace turing

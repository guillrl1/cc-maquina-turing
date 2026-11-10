/**
 * @file tape.h
 * @brief Definición de la clase Tape (Cinta) para la Máquina de Turing multicinta.
 *
 * La cinta es infinita en ambas direcciones y se implementa con un
 * std::map<int, char> para modelar celdas dispersas de forma eficiente.
 *
 * @author  Guillermo Lopez Concepcion
 * @date    18 Oct 2026
 * @note    Asignatura: Complejidad Computacional
 */

#pragma once

#include <iostream>
#include <map>
#include <string>

namespace turing {

/**
 * @class Tape
 * @brief Modela una cinta infinita en ambas direcciones para la MT.
 *
 * Internamente se usa un std::map<int,char> donde la clave es la posición
 * (puede ser negativa) y el valor es el símbolo almacenado. Las posiciones
 * no presentes en el mapa contienen implícitamente el símbolo blanco.
 */
class Tape {
 public:
  /**
   * @brief Constructor por defecto.
   * @param blank Símbolo blanco de la cinta (por defecto '.').
   */
  explicit Tape(char blank = '.');

  /**
   * @brief Inicializa la cinta con una cadena de entrada.
   *
   * Escribe cada carácter de @p input a partir de la posición 0 y
   * coloca la cabeza lectora/escritora en la posición 0.
   *
   * @param input Cadena de entrada a escribir en la cinta.
   */
  void LoadInput(const std::string& input);

  /**
   * @brief Lee el símbolo bajo la cabeza lectora/escritora.
   * @return Símbolo actual (símbolo blanco si la celda está vacía).
   */
  char Read() const;

  /**
   * @brief Escribe un símbolo en la posición actual de la cabeza.
   * @param symbol Símbolo a escribir.
   */
  void Write(char symbol);

  /**
   * @brief Mueve la cabeza lectora/escritora.
   *
   * @param direction 'L' (izquierda), 'R' (derecha) o 'S' (parada).
   */
  void Move(char direction);

  /**
   * @brief Devuelve la posición actual de la cabeza.
   * @return Entero que representa la posición (puede ser negativo).
   */
  int GetHeadPosition() const;

  /**
   * @brief Representa el contenido de la cinta como cadena.
   *
   * Muestra los símbolos desde la posición mínima a la máxima, con el
   * símbolo bajo la cabeza entre corchetes. Ejemplo: "ab[c]d".
   *
   * @return Cadena con el contenido visible de la cinta.
   */
  std::string ToString() const;

  /**
   * @brief Resetea la cinta a su estado inicial (vacía, cabeza en 0).
   */
  void Reset();

 private:
  /// Símbolo blanco de la cinta.
  char blank_symbol_;

  /// Mapa disperso: posición → símbolo.
  std::map<int, char> cells_;

  /// Posición actual de la cabeza lectora/escritora.
  int head_position_;
};

}  // namespace turing

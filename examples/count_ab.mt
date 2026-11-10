# ==============================================================================
# Asignatura: Complejidad Computacional (Práctica 2)
# Autor:      Guillermo Lopez Concepcion
# Fecha:      18 Oct 2026
# Máquina de Turing 2: Contador y separador unario de 'b's y 'a's (3 Cintas, 6 Estados)
# ==============================================================================
#
# Descripción del problema:
#   Recibe como entrada en la Cinta 1 una cadena compuesta por símbolos 'a' y 'b'.
#   La MT sustituye la cadena por el número de símbolos 'b', seguido del número
#   de símbolos 'a', separados por un símbolo blanco '.'.
#   El número se codifica en unario como n = 1^n.
#   Si la cadena carece de algún símbolo, se refleja con un "0" la ausencia del mismo.
#   La cabeza lectora/escritora queda situada al principio del resultado.
#
# Ejemplos:
#   abbabaabb -> 11111.1111.
#   aa        -> 0.11.
#   bb        -> 11.0.
#
# Configuración fijada:
#   - MT multicinta determinista con 3 CINTAS:
#       * Cinta 1: entrada original y destino del resultado final.
#       * Cinta 2: acumulador unario para el conteo de 'b's.
#       * Cinta 3: acumulador unario para el conteo de 'a's.
#   - Símbolo blanco: '.'
#   - Movimientos: L (izquierda), R (derecha), S (no movimiento / stay).
#   - Escritura y movimientos independientes por cinta.
#   - Cintas infinitas en ambas direcciones.
#   - Parada determinada únicamente por ausencia de transiciones.
#
# Diseño óptimo de 6 estados:
#   Aprovecha la escritura en reversa (de derecha a izquierda) sobre Cinta 1
#   mientras se leen en reverso las Cintas 2 y 3. De esta forma, al terminar
#   de volcar las 'b's, la cabeza de Cinta 1 queda automáticamente posicionada
#   al principio del resultado (con un simple paso R), eliminando completamente
#   la necesidad de estados adicionales para rebobinado.
#
# Estados (Q = {q0, q1, q2, q3, q4, q5}, s = q0, F = {q5}):
#   - q0: Lectura de Cinta 1 de izq. a der., conteo en C2 ('b') y C3 ('a'),
#         y borrado simultáneo de Cinta 1 escribiendo blanco '.'.
#   - q1: Detección inicial de 'a's en C3. Si hay '1' escribe '1', si no escribe '0'.
#   - q2: Bucle de copia de 'a's restantes desde C3 a C1 hacia la izquierda.
#         Al terminar C3, escribe el separador '.' y transita a q3.
#   - q3: Detección inicial de 'b's en C2. Si hay '1' escribe '1', si no escribe '0'.
#   - q4: Bucle de copia de 'b's restantes desde C2 a C1 hacia la izquierda.
#         Al terminar C2, transita a q5 moviendo Cinta 1 a la derecha (R).
#   - q5: Estado final de aceptación. Cabeza de C1 situada sobre el primer símbolo.
#
# ------------------------------------------------------------------------------

# 1. Conjunto Q de estados
q0 q1 q2 q3 q4 q5

# 2. Alfabeto de entrada Sigma
a b

# 3. Alfabeto de la cinta Gamma
a b 0 1 .

# 4. Estado inicial s
q0

# 5. Símbolo blanco
.

# 6. Conjunto F de estados de aceptación
q5

# 7. Número de cintas
3

# 8. Función de transición delta
# Formato: q_act  r1 r2 r3  q_sig  w1 m1  w2 m2  w3 m3

# ------------------------------------------------------------------------------
# Estado q0: Conteo y borrado de Cinta 1 hacia la derecha
# ------------------------------------------------------------------------------
# Lee 'a': borra C1 con '.', incrementa C3 escribiendo '1'
q0 a . . q0 . R . S 1 R

# Lee 'b': borra C1 con '.', incrementa C2 escribiendo '1'
q0 b . . q0 . R 1 R . S

# Fin de entrada en C1: mover todas las cabezas a la izquierda y pasar a q1
q0 . . . q1 . L . L . L

# ------------------------------------------------------------------------------
# Estado q1 -> q2: Detección inicial de 'a's (Cinta 3)
# ------------------------------------------------------------------------------
# Caso na > 0 (C3 lee '1'): escribe primer '1' de las 'a's y mueve C3 a la izquierda
q1 . 1 1 q2 1 L 1 S 1 L
q1 . . 1 q2 1 L . S 1 L

# Caso na = 0 (C3 lee '.'): escribe '0' para las 'a's y mantiene C3
q1 . 1 . q2 0 L 1 S . S
q1 . . . q2 0 L . S . S

# ------------------------------------------------------------------------------
# Estado q2: Bucle de volcado de 'a's restantes y escritura del separador '.'
# ------------------------------------------------------------------------------
# Bucle: mientras C3 siga teniendo '1's, seguir escribiendo '1's en C1 hacia la izquierda
q2 . 1 1 q2 1 L 1 S 1 L
q2 . . 1 q2 1 L . S 1 L

# Fin de 'a's (C3 lee '.'): escribir separador '.' en C1 y pasar a q3
q2 . 1 . q3 . L 1 S . S
q2 . . . q3 . L . S . S

# ------------------------------------------------------------------------------
# Estado q3 -> q4: Detección inicial de 'b's (Cinta 2)
# ------------------------------------------------------------------------------
# Caso nb > 0 (C2 lee '1'): escribe primer '1' de las 'b's y mueve C2 a la izquierda
q3 . 1 . q4 1 L 1 L . S

# Caso nb = 0 (C2 lee '.'): escribe '0' para las 'b's
q3 . . . q4 0 L . S . S

# ------------------------------------------------------------------------------
# Estado q4: Bucle de volcado de 'b's restantes y posicionamiento final
# ------------------------------------------------------------------------------
# Bucle: mientras C2 siga teniendo '1's, seguir escribiendo '1's en C1 hacia la izquierda
q4 . 1 . q4 1 L 1 L . S

# Fin de 'b's (C2 lee '.'): dar un paso a la derecha (R) en C1 para situar
# la cabeza exactamente sobre el primer símbolo del resultado y parar en q5
q4 . . . q5 . R . S . S

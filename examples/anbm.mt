# ==============================================================================
# Asignatura: Complejidad Computacional (Práctica 2)
# Autor:      Guillermo Lopez Concepcion
# Fecha:      18 Oct 2026
# Máquina de Turing 1: Reconocedor del lenguaje L = { a^n b^m | m >= n, n > 0 }
# ==============================================================================
#
# Configuración fijada:
#   - MT multicinta (2 cintas) determinista.
#   - Cinta 1: contiene la cadena de entrada.
#   - Cinta 2: auxiliar para contar el número de símbolos 'a'.
#   - Movimientos: L (izquierda), R (derecha), S (no movimiento / stay).
#   - Escritura y movimientos independientes por cinta.
#   - Cintas infinitas en ambas direcciones.
#   - Parada determinada únicamente por ausencia de transiciones.
#
# Descripción de estados:
#   q0         : Estado inicial. Requiere leer al menos una 'a' (garantiza n > 0).
#   q1         : Lee el resto de 'a's, escribiendo una 'X' en cinta 2 por cada 'a'.
#   q_rewind   : Retrocede la cabeza de la cinta 2 hasta el primer 'X'.
#   q_match    : Empareja cada 'b' de cinta 1 con un 'X' de cinta 2.
#   q_more_b   : Consume 'b's sobrantes en cinta 1 cuando m > n.
#   q_accept   : Estado de aceptación (sin transiciones salientes).

q0 q1 q_rewind q_match q_more_b q_accept
a b
a b X .
q0
.
q_accept
2

# 1. Fase de lectura de 'a's y copia en cinta 2
# Se debe leer al menos una 'a' (n > 0)
q0 a . q1 a R X R
q1 a . q1 a R X R

# 2. Al encontrar la primera 'b', retroceder cinta 2 al inicio de las X
# La cabeza de cinta 1 permanece sobre 'b' (movimiento S)
q1 b . q_rewind b S . L
q_rewind b X q_rewind b S X L
q_rewind b . q_match b S . R

# 3. Fase de emparejamiento (b con X)
q_match b X q_match b R X R

# Si m == n, ambas cintas alcanzan el símbolo blanco simultáneamente
q_match . . q_accept . S . S

# Si m > n, cinta 2 llega a blanco pero cinta 1 aún tiene 'b's
q_match b . q_more_b b R . S
q_more_b b . q_more_b b R . S
q_more_b . . q_accept . S . S

cmake_minimum_required(VERSION 3.16)
project(TuringMachineSimulator
        VERSION 1.0
        DESCRIPTION "Simulador de Máquina de Turing Determinista Multicinta"
        LANGUAGES CXX)

# ── Estándar C++ ──────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Flags de compilación ──────────────────────────────────────────────────────
# Perfil Release por defecto si no se especifica otro.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Tipo de build" FORCE)
endif()

set(CMAKE_CXX_FLAGS_DEBUG   "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

# Warnings estrictos (estilo Google).
add_compile_options(
  -Wall
  -Wextra
  -Wpedantic
  -Wshadow
  -Wconversion
  -Wsign-conversion
  -Wnon-virtual-dtor
  -Woverloaded-virtual
)

# ── Directorio de cabeceras ───────────────────────────────────────────────────
include_directories(${PROJECT_SOURCE_DIR}/include)

# ── Fuentes de la biblioteca del simulador ────────────────────────────────────
set(LIB_SOURCES
    src/tape.cc
    src/state.cc
    src/transition.cc
    src/tm_parser.cc
    src/turing_machine.cc
)

add_library(turing_lib STATIC ${LIB_SOURCES})
target_include_directories(turing_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# ── Ejecutable principal ──────────────────────────────────────────────────────
add_executable(turing_machine src/main.cc)
target_link_libraries(turing_machine PRIVATE turing_lib)

# ── Mensajes informativos ─────────────────────────────────────────────────────
message(STATUS "Build type : ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ estándar : ${CMAKE_CXX_STANDARD}")
message(STATUS "Ejecutable : turing_machine")

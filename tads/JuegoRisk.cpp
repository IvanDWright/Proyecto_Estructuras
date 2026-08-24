#include "JuegoRisk.h"

/*
================================================================================

================================================================================
*/

JuegoRisk::JuegoRisk()
    : estado(NO_INICIALIZADO), indiceJugadorActual(0), ganador("") {}

bool JuegoRisk::estaInicializado() const { return estado == EN_CURSO; }

bool JuegoRisk::estaTerminado() const { return estado == TERMINADO; }

Jugador *JuegoRisk::obtenerJugadorEnTurno() {
  if (estado != EN_CURSO || jugadores.estaVacia()) {
    return NULL;
  }
  return &(jugadores.obtener(indiceJugadorActual));
}

Jugador *JuegoRisk::buscarJugadorPorNombre(const std::string &nombreBuscado) {
  for (int i = 0; i < jugadores.longitud(); i++) {
    if (jugadores.obtener(i).nombre == nombreBuscado) {
      return &(jugadores.obtener(i));
    }
  }
  return NULL;
}

void JuegoRisk::pasarAlSiguienteTurno() {
  if (jugadores.estaVacia()) {
    return;
  }

  // Rotación circular en la lista de jugadores
  indiceJugadorActual = (indiceJugadorActual + 1) % jugadores.longitud();
  jugadores.obtener(indiceJugadorActual).reiniciarFaseTurno();
}

bool JuegoRisk::verificarGanadorGlobal() {
  if (estado != EN_CURSO) {
    return false;
  }

  Jugador *actual = obtenerJugadorEnTurno();
  if (actual != NULL) {
    if (tablero.contarTerritoriosDe(actual->color) == 42) {
      estado = TERMINADO;
      ganador = actual->nombre;
      return true;
    }
  }
  return false;
}

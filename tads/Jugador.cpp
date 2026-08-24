#include "Jugador.h"

/*
================================================================================

================================================================================
*/

Jugador::Jugador()
    : nombre(""), color(""), faseActual(FASE_OBTENER_UNIDADES),
      haConquistadoEnEsteTurno(false) {}

Jugador::Jugador(const std::string &nom, const std::string &col)
    : nombre(nom), color(col), faseActual(FASE_OBTENER_UNIDADES),
      haConquistadoEnEsteTurno(false) {}

void Jugador::reiniciarFaseTurno() {
  faseActual = FASE_OBTENER_UNIDADES;
  haConquistadoEnEsteTurno = false;
}

void Jugador::avanzarFase(FaseTurno siguienteFase) {
  faseActual = siguienteFase;
}

void Jugador::agregarCarta(const Carta &nuevaCarta) {
  cartas.agregarAlFinal(nuevaCarta);
}

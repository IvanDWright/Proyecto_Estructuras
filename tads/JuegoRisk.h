#ifndef JUEGORISK_H
#define JUEGORISK_H

#include "Jugador.h"
#include "ListaLineal.h"
#include "TableroRisk.h"
#include <string>


/*
================================================================================

================================================================================
*/

enum EstadoJuego { NO_INICIALIZADO, EN_CURSO, TERMINADO };

class JuegoRisk {
public:
  EstadoJuego estado;             // Estado actual del ciclo de vida del juego
  ListaLineal<Jugador> jugadores; // Colección lineal de jugadores participantes
  int indiceJugadorActual; // Índice en 'jugadores' del jugador con el turno
                           // activo
  TableroRisk tablero;     // Mapa topológico con los 42 territorios
  std::string ganador; // Nombre del jugador victorioso si finalizó la partida

  // Constructores
  JuegoRisk();

  // Consultas de estado
  bool estaInicializado() const;
  bool estaTerminado() const;

  // Métodos de acceso a jugadores
  Jugador *obtenerJugadorEnTurno();
  Jugador *buscarJugadorPorNombre(const std::string &nombreBuscado);

  // Rotación y avance de turnos
  void pasarAlSiguienteTurno();

  // Verificación de victoria (si un jugador posee los 42 territorios)
  bool verificarGanadorGlobal();
};

#endif // JUEGORISK_H

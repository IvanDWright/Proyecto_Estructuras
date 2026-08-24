#ifndef JUGADOR_H
#define JUGADOR_H

#include "Carta.h"
#include "ListaLineal.h"
#include <string>


/*
================================================================================

================================================================================
*/

enum FaseTurno {
  FASE_OBTENER_UNIDADES,
  FASE_ATACAR,
  FASE_FORTIFICAR,
  FASE_FINALIZADA
};

class Jugador {
public:
  std::string nombre;            // Nombre del jugador (máx 8 caracteres)
  std::string color;             // Color distintivo asignado
  ListaLineal<Carta> cartas;     // Colección de cartas en mano
  FaseTurno faseActual;          // Estado actual dentro de su turno
  bool haConquistadoEnEsteTurno; // Indica si conquistó al menos un territorio
                                 // en el turno

  // Constructores
  Jugador();
  Jugador(const std::string &nom, const std::string &col);

  // Reinicia las banderas y fases para el inicio de un nuevo turno
  void reiniciarFaseTurno();

  // Avanza la máquina de estados a la siguiente fase
  void avanzarFase(FaseTurno siguienteFase);

  // Agrega una nueva carta a la mano del jugador
  void agregarCarta(const Carta &nuevaCarta);
};

#endif // JUGADOR_H

#ifndef JUEGORISK_H
#define JUEGORISK_H

#include <string>
#include "ListaLineal.h"
#include "Jugador.h"
#include "TableroRisk.h"

/*
================================================================================
TAD: JuegoRisk (Fachada y Controlador Principal del Juego)
================================================================================
Descripción:
  Clase controladora de alto nivel.
  Mantiene el estado global de la partida (NO_INICIALIZADO, EN_CURSO, TERMINADO),
  la lista lineal de jugadores participantes, el índice o apuntador hacia el jugador
  con el turno actual, la instancia central de TableroRisk y los métodos de negocio
  para ejecutar los comandos de la Entrega 1.

Recomendaciones de Implementación:
  1. Mantener las validaciones de turno estricto y de flujo de comandos antes de
     modificar los datos internos.
  2. Implementar 'pasarAlSiguienteTurno()' utilizando aritmética modular:
     indiceJugadorActual = (indiceJugadorActual + 1) % jugadores.longitud();
  3. Asegurar que al cambiar de turno se invoque 'reiniciarFaseTurno()' en el nuevo jugador.
================================================================================
*/

enum EstadoJuego {
    NO_INICIALIZADO,
    EN_CURSO,
    TERMINADO
};

class JuegoRisk {
public:
    EstadoJuego estado;                       // Estado actual del ciclo de vida del juego
    ListaLineal<Jugador> jugadores;           // Colección lineal de jugadores participantes
    int indiceJugadorActual;                  // Índice en 'jugadores' del jugador con el turno activo
    TableroRisk tablero;                      // Mapa topológico con los 42 territorios
    std::string ganador;                      // Nombre del jugador victorioso si finalizó la partida

    // Constructores
    JuegoRisk();

    // Consultas de estado
    bool estaInicializado() const;
    bool estaTerminado() const;

    // Métodos de acceso a jugadores
    Jugador* obtenerJugadorEnTurno();
    Jugador* buscarJugadorPorNombre(const std::string& nombreBuscado);

    // Rotación y avance de turnos
    void pasarAlSiguienteTurno();

    // Verificación de victoria (si un jugador posee los 42 territorios)
    bool verificarGanadorGlobal();
};

#endif // JUEGORISK_H

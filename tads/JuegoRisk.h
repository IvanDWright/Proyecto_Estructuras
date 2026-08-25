#ifndef JUEGORISK_H
#define JUEGORISK_H

#include <string>
#include "ListaLineal.h"
#include "Jugador.h"
#include "TableroRisk.h"

/*
============================================================================================
Esta clase representa el juego de risk, contiene el estado del juego, la lista de jugadores, 
el jugador actual, el tablero y el ganador.

Tiene el estado de el juego (NO_INICIALIZADO, EN_CURSO, TERMINADO estas opciones se definen
en la enumeracion), una lista de jugadores (Jugador), el indice del jugador actual (int) que
marca el numero en la lista de el jugador que esta en el turno en ese momento, un tablero
(TableroRisk) y el ganador (string). 

Funciones: 
1. constructores: uno por defecto que inicializa el juego en NO_INICIALIZADO.
2. estaInicializado: Verifica si el juego esta inicializado.
3. estaTerminado: Verifica si el juego esta terminado.
4. obtenerJugadorEnTurno: Obtiene el jugador en turno.
5. buscarJugadorPorNombre: Busca un jugador por nombre.
6. pasarAlSiguienteTurno: Pasa al siguiente turno.
7. verificarGanadorGlobal: Verifica si hay un ganador global.
============================================================================================
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


    JuegoRisk();


    bool estaInicializado() const;
    bool estaTerminado() const;


    Jugador* obtenerJugadorEnTurno();
    Jugador* buscarJugadorPorNombre(const std::string& nombreBuscado);


    void pasarAlSiguienteTurno();

    bool verificarGanadorGlobal();
};

#endif // JUEGORISK_H

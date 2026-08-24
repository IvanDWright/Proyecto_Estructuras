#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>
#include "ListaLineal.h"
#include "Carta.h"

/*
================================================================================
TAD: Jugador
================================================================================
Descripción:
  Modela a cada participante de la partida.
  Registra su nombre (máx 8 caracteres, sin espacios), color distintivo,
  mano de cartas y la máquina de estados local para validar el orden secuencial
  estricto de las fases de su turno (obtener_unidades -> atacar -> fortificar).

Recomendaciones de Implementación:
  1. Utilizar el enum 'FaseTurno' para controlar transiciones de estado:
     - FASE_OBTENER_UNIDADES: Solo puede ejecutar obtener_unidades.
     - FASE_ATACAR: Ya obtuvo unidades, ahora puede atacar.
     - FASE_FORTIFICAR: Ya terminó de atacar, ahora puede fortificar.
     - FASE_FINALIZADA: Completó su turno.
  2. La bandera 'haConquistadoEnEsteTurno' se activa si durante la fase de ataque
     conquistó al menos un territorio; al terminar de fortificar, si está activa,
     se le otorga una tarjeta y se reinicia la bandera.
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
    std::string nombre;                       // Nombre del jugador (máx 8 caracteres)
    std::string color;                        // Color distintivo asignado
    ListaLineal<Carta> cartas;                // Colección de cartas en mano
    FaseTurno faseActual;                     // Estado actual dentro de su turno
    bool haConquistadoEnEsteTurno;            // Indica si conquistó al menos un territorio en el turno

    // Constructores
    Jugador();
    Jugador(const std::string& nom, const std::string& col);

    // Reinicia las banderas y fases para el inicio de un nuevo turno
    void reiniciarFaseTurno();

    // Avanza la máquina de estados a la siguiente fase
    void avanzarFase(FaseTurno siguienteFase);

    // Agrega una nueva carta a la mano del jugador
    void agregarCarta(const Carta& nuevaCarta);
};

#endif // JUGADOR_H

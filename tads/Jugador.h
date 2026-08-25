#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>
#include "ListaLineal.h"
#include "Carta.h"

/*
===========================================================================================
Primero hay una enumeracion que es FaseTurno que representa el estado actual del turno
el cual tiene 4 opciones, la primera es FASE_OBTENER_UNIDADES que es cuando el jugador
recibe sus tropas al inicio de cada turno, la segunda es FASE_ATACAR que es cuando el jugador
decide si atacar o no un territorio enemigo, la tercera es FASE_FORTIFICAR que es cuando
el jugador decide si mover tropas entre sus territorios o no, y la ultima es FASE_FINALIZADA
que es cuando finaliza el turno.

La clase jugador tiene un nombre por cada jugador, el cual es limitado a 8 caracteres
al momento de añadirlo un string de color que es el qu se usa al momento de asignar un 
dueno de un territorio especifico, una lista lineal de cartas las cuales se le otorgan al
jugador cuando elimina a otro o cuando obtiene un territorio con el cual tiene conexion 
(limite de 1 por turno en este caso), un dato de el enumerador FaseTurno que define el 
estado del turno actual, y un bool que define si ha conquistado al menos un 
territorio en este turno.

Funciones: 
1. constructores por defecto y con los parametros, uno que por default esta vacio, y 
otro que recibe el nombre y color del jugador.
2. reiniciarFaseTurno: Reinicia el estado del turno.
3. avanzarFase: Avanza la máquina de estados a la siguiente fase.
4. agregarCarta: Agrega una nueva carta a la mano del jugador.
===========================================================================================
*/

enum FaseTurno {
    FASE_OBTENER_UNIDADES,
    FASE_ATACAR,
    FASE_FORTIFICAR,
    FASE_FINALIZADA
};

class Jugador {
public:
    std::string nombre;                       
    std::string color;                        
    ListaLineal<Carta> cartas;               
    FaseTurno faseActual;                     
    bool haConquistadoEnEsteTurno;            

    Jugador();
    Jugador(const std::string& nom, const std::string& col);

    void reiniciarFaseTurno();

    void avanzarFase(FaseTurno siguienteFase);


    void agregarCarta(const Carta& nuevaCarta);
};

#endif // JUGADOR_H

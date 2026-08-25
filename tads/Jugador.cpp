#include "Jugador.h"

/*
===========================================================================================
Funciones:

1. constructores: uno por defecto: vacio y otro con los parametros nom y col
que son los que recibe al momento de crear un jugador.
2. reiniciarFaseTurno: Reinicia la fase actual del turno al estado inicial, lo hace al
igualar la variable faseActual a FASE_OBTENER_UNIDADES y cambia el bool que consulta si
ha conquistado un territorio a false.
3. avanzarFase: Avanza la fase actual del turno a la fase siguiente, lo hace al
igualar la variable faseActual a siguienteFase, que es el parametro que recibe.
4. agregarCarta: Agrega una carta al jugador, lo hace al agregar la carta a la
lista de cartas.
===========================================================================================
*/

Jugador::Jugador() 
    : nombre(""), color(""), faseActual(FASE_OBTENER_UNIDADES), haConquistadoEnEsteTurno(false) {}

Jugador::Jugador(const std::string& nom, const std::string& col)
    : nombre(nom), color(col), faseActual(FASE_OBTENER_UNIDADES), haConquistadoEnEsteTurno(false) {}

void Jugador::reiniciarFaseTurno() {
    faseActual = FASE_OBTENER_UNIDADES;
    haConquistadoEnEsteTurno = false;
}

void Jugador::avanzarFase(FaseTurno siguienteFase) {
    faseActual = siguienteFase;
}

void Jugador::agregarCarta(const Carta& nuevaCarta) {
    cartas.agregarAlFinal(nuevaCarta);
}

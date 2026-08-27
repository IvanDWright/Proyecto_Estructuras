#include "JuegoRisk.h"

/*
=============================================================================================
Funciones:
1. constructor: inicializa el juego en NO_INICIALIZADO, y tiene los datos vacios.
2. estaInicializado: consulta si el estado del juego es EN_CURSO. lo cual es un bool.
3. estaTerminado: consulta si el estado del juego es TERMINADO. lo cual es un bool.
4. obtenerJugadorEnTurno: obtiene el jugador en turno. retorna un puntero a un jugador, 
primero verifica si el juego esta en curso y si la lista no esta vacia, para saber si seguir
si alguna de estas no es verdad retorna NULL. en cualquier otro caso retorna un puntero a 
la posicion de el jugador que esta actualmente en el turno.
5. BuscarJugadorPorNombre: recibe una constante referencia a el nombre de un jugador 
que se busca, recorre la lista de jugadores y retorna un puntero al jugador si lo encuentra.
6. pasarAlSiguienteTurno: revisa si la lista de jugadores no esta vacia, para saber si
retornar, si no esta vacia el jugador siguiente es igual al residuo de el jugador actual mas 1
sobre la cantidad de jugadores, ya que es un numero menor a la cantidad de jugadores el
residuo siempre sera igual a el jugador siguiente, amenos que sea el mimso numero de jugadores
el cual retornaria 0, el jugador siguiente y se reinicia la fase de turno.
7. verificarGanadorGlobal: revisa si el estado del juego es en curso, si no es asi retorna 
false, si lo esta, obtiene el puntero de el jugador actual, y verifica que no sea null,
finalmente revisa si la cantidad de territorios que posee es lo mismo que la totalidad
de territorios en el juego, si es asi le da el rop le ganador a ese jugador y pone el estado
de el juego en finalizado. retorna true.
=============================================================================================
*/

JuegoRisk::JuegoRisk() 
    : estado(NO_INICIALIZADO), indiceJugadorActual(0), ganador("") {}

bool JuegoRisk::estaInicializado() const {
    return estado == EN_CURSO;
}

bool JuegoRisk::estaTerminado() const {
    return estado == TERMINADO;
}

Jugador* JuegoRisk::obtenerJugadorEnTurno() {
    // codigo asistido por IA
    if (estado != EN_CURSO || jugadores.estaVacia()) {
        return NULL;
    }
    return &(jugadores.obtener(indiceJugadorActual));
}

Jugador* JuegoRisk::buscarJugadorPorNombre(const std::string& nombreBuscado) {
    for (int i = 0; i < jugadores.longitud(); i++) {
        if (jugadores.obtener(i).nombre == nombreBuscado) {
            return &(jugadores.obtener(i));
        }
    }
    return NULL;
}

void JuegoRisk::pasarAlSiguienteTurno() {
    // codigo asistido por IA
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

    Jugador* actual = obtenerJugadorEnTurno();
    if (actual != NULL) {
        if (tablero.contarTerritoriosDe(actual->color) == 42) {
            estado = TERMINADO;
            ganador = actual->nombre;
            return true;
        }
    }
    return false;
}

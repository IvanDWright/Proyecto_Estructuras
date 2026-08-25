#include "Continente.h"

/*
===========================================================================================
Funciones:

1. Constructores: Uno que por default esta vacio, otro que recibe el id, nombre y bono. (por
lo mismo que otros ejemplos es referencia y const)
2. agregarTerritorio: Recibe un puntero a un territorio y lo agrega a la lista, esto lo hace
si y solo si no esta dentro de la lista de territorios, lo cual rrevisa mediante el for,
despues si no esta agrega al final de la lista.
3. esControladoTotalmentePor: Consulta si un jugador domina todos los territorios del
continente, para esto recorre la lista de territorios y revisa si todos los territorios son
posesion del jugador.
===========================================================================================
*/

Continente::Continente() : id(0), nombre(""), bonoUnidades(0) {}

Continente::Continente(int identificador, const std::string& nom, int bono)
    : id(identificador), nombre(nom), bonoUnidades(bono) {}

void Continente::agregarTerritorio(Territorio* t) {
    if (t != NULL) {
        for (int i = 0; i < territorios.longitud(); i++) {
            if (territorios.obtener(i) == t) {
                return;
            }
        }
        territorios.agregarAlFinal(t);
    }
}

bool Continente::esControladoTotalmentePor(const std::string& colorJugador) const {
    if (territorios.estaVacia() || colorJugador.empty()) {
        return false;
    }

    for (int i = 0; i < territorios.longitud(); i++) {
        if (territorios.obtener(i)->colorOcupante != colorJugador) {
            return false;
        }
    }
    return true;
}

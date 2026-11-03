#include "Territorio.h"

// Constructor por defecto, inicializa los valores en blanco o cero
Territorio::Territorio() 
    : idTerritorio(""), nombreTerritorio(""), idContinente(0), colorPropietario(""), cantidadTropas(0) {}

// Constructor parametrizado: recibe el código como referencia constante (para que no sea copia y no sea editable),
// el nombre (igual que el código) y el ID del continente al que pertenece.
Territorio::Territorio(const std::string& idTer, const std::string& nomTer, int idCont)
    : idTerritorio(idTer), nombreTerritorio(nomTer), idContinente(idCont), colorPropietario(""), cantidadTropas(0) {}

// Asigna el jugador ocupante inicial (recibe una referencia constante al color del nuevo ocupante)
// y la cantidad de ejércitos a ubicar.
void Territorio::asignarOcupante(const std::string& nuevoColor, int cantidad) {
    colorPropietario = nuevoColor;
    cantidadTropas = cantidad;
}

// Modifica la cantidad de ejércitos actuales. Recibe un entero que es la cantidad de ejércitos a modificar.
void Territorio::modificarEjercitos(int delta) {
    cantidadTropas += delta;
}

// Recibe un puntero a un territorio para agregar a los adyacentes. 
// Primero revisa si es nulo o igual al territorio actual, si lo es retorna false. 
// Revisa si el puntero ya está en la lista de adyacentes, en caso de serlo retorna false. 
// Si pasa las validaciones, se agrega al final de la lista y se retorna true. 
// Recibe un puntero (que es tratado sin modificar el dato).
bool Territorio::agregarVecino(Territorio* ptrVecino) {
    if (ptrVecino == NULL || ptrVecino == this) {
        return false;
    }

    // Verificar si ya fue agregado previamente para evitar duplicados en la frontera
    for (int i = 0; i < adyacentes.longitud(); i++) {
        if (adyacentes.obtener(i)->idTerritorio == ptrVecino->idTerritorio) {
            return false;
        }
    }

    adyacentes.agregarAlFinal(ptrVecino);
    return true;
}

// Recibe una referencia constante al código de un territorio y revisa si es vecino de este.
// Primero revisa si ese código no es nulo o es del mismo territorio, en caso de que lo sea retorna false.
// En caso de que no, revisa la lista de vecinos, si está adentro de la lista retorna true, si no retorna false.
bool Territorio::esVecinoDe(const std::string& idDestino) const {
    if (idDestino == "" || idDestino == this->idTerritorio) {
        return false;
    }
    for (int i = 0; i < adyacentes.longitud(); i++) {
        if (adyacentes.obtener(i)->idTerritorio == idDestino) {
            return true;
        }
    }
    return false;
}

// Revisa si algun vecino tiene un color de propietario distinto al recibido; Recibe: const string& colorJugador; Retorna: bool
bool Territorio::tieneVecinoEnemigo(const std::string& colorJugador) const {
    for (int i = 0; i < adyacentes.longitud(); i++) {
        if (adyacentes.obtener(i)->colorPropietario != colorJugador) {
            return true;
        }
    }
    return false;
}

// Revisa si algun vecino tiene el mismo color de propietario que el recibido; Recibe: const string& colorJugador; Retorna: bool
bool Territorio::tieneVecinoPropio(const std::string& colorJugador) const {
    for (int i = 0; i < adyacentes.longitud(); i++) {
        if (adyacentes.obtener(i)->colorPropietario == colorJugador) {
            return true;
        }
    }
    return false;
}

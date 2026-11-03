#include "Continente.h"

// Constructor por defecto, inicializa los atributos en cero o vacio; Recibe: nada; Retorna: nada
Continente::Continente() : idContinente(0), nombreContinente(""), tropasAdicionales(0) {}

// Constructor parametrizado; Recibe: int identificador, const string& nom, int bono; Retorna: nada
Continente::Continente(int identificador, const std::string& nom, int bono)
    : idContinente(identificador), nombreContinente(nom), tropasAdicionales(bono) {}

// Consulta si un jugador domina todos los territorios del continente; Recibe: const string& colorJugador; Retorna: bool
// Recorre la lista de territorios internos y revisa si todos son posesion del jugador dado.
bool Continente::esControladoTotalmentePor(const std::string& colorJugador) const {
    if (territoriosInternos.estaVacia() || colorJugador.empty()) {
        return false;
    }

    for (int i = 0; i < territoriosInternos.longitud(); i++) {
        if (territoriosInternos.obtener(i)->colorPropietario != colorJugador) {
            return false;
        }
    }
    return true;
}

// Agrega un territorio al continente; Recibe: Territorio* t; Retorna: void
// Valida que el puntero no sea nulo y que no se encuentre ya registrado en la lista interna.
void Continente::agregarTerritorio(Territorio* t) {
    if (t != NULL) {
        for (int i = 0; i < territoriosInternos.longitud(); i++) {
            if (territoriosInternos.obtener(i) == t) {
                return;
            }
        }
        territoriosInternos.agregarAlFinal(t);
    }
}

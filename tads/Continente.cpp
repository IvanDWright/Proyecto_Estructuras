#include "Continente.h"

/*
================================================================================
Implementación del TAD Continente
================================================================================
Recomendaciones:
  - Validar que territorios no esté vacío al consultar esControladoTotalmentePor.
================================================================================
*/

Continente::Continente() : id(0), nombre(""), bonoUnidades(0) {}

Continente::Continente(int identificador, const std::string& nom, int bono)
    : id(identificador), nombre(nom), bonoUnidades(bono) {}

void Continente::agregarTerritorio(Territorio* t) {
    if (t != NULL) {
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

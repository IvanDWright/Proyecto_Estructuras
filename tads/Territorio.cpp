#include "Territorio.h"

/*
================================================================================

================================================================================
*/

Territorio::Territorio()
    : codigo(""), nombre(""), continenteId(0), colorOcupante(""),
      numEjercitos(0) {}

Territorio::Territorio(const std::string &cod, const std::string &nom,
                       int contId)
    : codigo(cod), nombre(nom), continenteId(contId), colorOcupante(""),
      numEjercitos(0) {}

bool Territorio::agregarVecino(Territorio *vecinoPtr) {
  if (vecinoPtr == NULL || vecinoPtr == this) {
    return false;
  }

  // Verificar si ya fue agregado previamente para evitar duplicados
  for (int i = 0; i < vecinos.longitud(); i++) {
    if (vecinos.obtener(i)->codigo == vecinoPtr->codigo) {
      return false;
    }
  }

  vecinos.agregarAlFinal(vecinoPtr);
  return true;
}

bool Territorio::esVecinoDe(const std::string &codigoDestino) const {
  for (int i = 0; i < vecinos.longitud(); i++) {
    if (vecinos.obtener(i)->codigo == codigoDestino) {
      return true;
    }
  }
  return false;
}

void Territorio::asignarOcupante(const std::string &nuevoColor, int cantidad) {
  colorOcupante = nuevoColor;
  numEjercitos = cantidad;
}

void Territorio::modificarEjercitos(int delta) { numEjercitos += delta; }

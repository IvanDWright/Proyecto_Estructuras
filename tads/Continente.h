#ifndef CONTINENTE_H
#define CONTINENTE_H

#include "ListaLineal.h"
#include "Territorio.h"
#include <string>

/*
================================================================================
TAD: Continente
================================================================================
Descripción:
  Agrupa un conjunto de territorios y define el bono de unidades adicionales
  que un jugador recibe al inicio de su turno si controla el 100% de dichos
  territorios.

Valores Reglamentarios de Bonos:
  - América del Sur: 2
  - Australia: 2
  - África: 3
  - América del Norte: 5
  - Europa: 5
  - Asia: 7

Recomendaciones de Implementación:
  1. Mantener una lista lineal de punteros a los territorios pertenecientes a
este continente.
  2. En esControladoTotalmentePor(), iterar sobre todos los territorios
comprobando si 'colorOcupante' coincide con el color del jugador consultado.
================================================================================
*/

class Continente {
public:
  int id;             // ID numérico (1: NA, 2: SA, 3: EU, 4: AF, 5: AS, 6: OC)
  std::string nombre; // Nombre oficial del continente
  int bonoUnidades;   // Tropas de bonificación otorgadas
  ListaLineal<Territorio *>
      territorios; // Punteros a los territorios del continente

  // Constructores
  Continente();
  Continente(int identificador, const std::string &nom, int bono);

  // Agrega un territorio al continente
  void agregarTerritorio(Territorio *t);

  // Verifica si un jugador domina todos los territorios del continente
  bool esControladoTotalmentePor(const std::string &colorJugador) const;
};

#endif // CONTINENTE_H

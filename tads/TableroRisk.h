#ifndef TABLERORISK_H
#define TABLERORISK_H

#include "Continente.h"
#include "ListaLineal.h"
#include "Territorio.h"
#include <string>


/*
================================================================================

================================================================================
*/

class TableroRisk {
public:
  ListaLineal<Territorio> territorios; // Colección lineal de los 42 territorios
  ListaLineal<Continente> continentes; // Colección lineal de los 6 continentes

  // Constructores y Destructor
  TableroRisk();
  ~TableroRisk();

  // Inicializa completamente los 42 territorios, los 6 continentes y todas las
  // fronteras oficiales
  void inicializarMapa();

  // Conecta dos territorios de forma bidireccional (grafo no dirigido)
  void conectarBidireccional(const std::string &cod1, const std::string &cod2);

  // Búsqueda de territorio por su código oficial (ej: "1.1") - O(N)
  Territorio *buscarTerritorio(const std::string &codigo);
  const Territorio *buscarTerritorio(const std::string &codigo) const;

  // Cuenta cuántos territorios están actualmente controlados por un color
  // determinado - O(N)
  int contarTerritoriosDe(const std::string &colorJugador) const;

  // Calcula la suma de bonos de los continentes dominados al 100% por un
  // jugador - O(N)
  int calcularBonoContinentes(const std::string &colorJugador) const;

  // Limpia y libera las colecciones
  void limpiar();
};

#endif // TABLERORISK_H

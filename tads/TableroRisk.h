#ifndef TABLERORISK_H
#define TABLERORISK_H

#include "Continente.h"
#include "ListaLineal.h"
#include "Territorio.h"
#include <string>


/*
================================================================================
TAD: TableroRisk
================================================================================
// codigo asistido por IA

Descripción:
  Este TAD representa el mapa completo del juego Risk. Funciona como el
  contenedor principal de las listas de territorios y continentes, y se encarga
  de inicializar toda la geografía del juego, incluyendo la creación de los
  42 territorios reglamentarios y la configuración del grafo bidireccional de
  las fronteras.

Funciones Principales:
  1. inicializarMapa: Instancia los continentes, luego los 42 territorios y, 
     finalmente, crea todas las aristas del grafo interconectando fronteras.
  2. conectarBidireccional: Crea una conexión de ida y vuelta entre dos
     territorios dados sus códigos.
  3. buscarTerritorio: Recorre la lista de territorios para encontrar un 
     territorio en específico usando su código (Ej. "1.1").
  4. contarTerritoriosDe: Utilidad para saber cuántos territorios domina
     un color (jugador) en todo el tablero.
  5. calcularBonoContinentes: Verifica cada continente para sumar tropas extra 
     si un jugador los domina por completo.
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

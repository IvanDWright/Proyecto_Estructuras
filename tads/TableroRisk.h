#ifndef TABLERORISK_H
#define TABLERORISK_H

#include "Continente.h"
#include "ListaLineal.h"
#include "Territorio.h"
#include <string>
#include <vector>

// =========================================================================
// TAD TableroRisk
// =========================================================================

class TableroRisk {
public:
  // =========================================================================
  // Atributos de TableroRisk
  // =========================================================================
  
  // Colección lineal de los 42 territorios
  ListaLineal<Territorio> listaTerritoriosGlob; 
  
  // Colección lineal de los 6 continentes
  ListaLineal<Continente> listaContinentesGlob; 

  // =========================================================================
  // Constructores y Destructor
  // =========================================================================
  
  TableroRisk();
  ~TableroRisk();

  // =========================================================================
  // Métodos Principales
  // =========================================================================

  // Inicializa completamente los 42 territorios, los 6 continentes y todas las
  // fronteras oficiales, armando la red interconectada.
  void configurarMapaBase();

  // Crea una conexión de ida y vuelta entre dos territorios dados sus códigos,
  // interconectando fronteras (grafo no dirigido).
  void establecerFrontera(const std::string &cod1, const std::string &cod2);

  // Recorre la lista de territorios para encontrar un territorio en específico 
  // usando su código oficial (Ej. "1.1"). Búsqueda en O(N).
  Territorio *obtenerTerritorioPorId(const std::string &idBuscado);
  const Territorio *obtenerTerritorioPorId(const std::string &idBuscado) const;

  // Cuenta cuántos territorios están actualmente controlados por un color
  // determinado (jugador) en todo el tablero. Complejidad: O(N)
  int cantidadTerritoriosOcupadosPor(const std::string &colorJugador) const;

  // Calcula la suma de bonos de los continentes dominados al 100% por un
  // jugador. Verifica cada continente para sumar tropas extra. Complejidad: O(N)
  int obtenerBonoPorContinentesDominados(const std::string &colorJugador) const;

  // Llena el vector 'resultado' con punteros a todos los territorios que ocupa
  // el jugador del color dado, en el orden del tablero. Se usa en los comandos para
  // mostrar todos los territorios del jugador junto con su código oficial. Complejidad: O(N)
  void territoriosDeJugador(const std::string &colorJugador,
                            std::vector<Territorio *> &resultado);

  // Llena 'resultado' con los territorios del jugador desde los cuales puede
  // atacar: mas de 1 tropa y al menos un vecino enemigo.
  void territoriosAptosParaAtacar(const std::string &colorJugador,
                                  std::vector<Territorio *> &resultado);

  // Llena 'resultado' con los territorios del jugador desde los cuales puede
  // fortificar: mas de 1 tropa y al menos un vecino propio.
  void territoriosAptosParaFortificar(const std::string &colorJugador,
                                      std::vector<Territorio *> &resultado);

  // Limpia y libera las colecciones de territorios y continentes.
  void vaciarTablero();

  // Intercambia los territorios y continentes con otro tablero en O(1), sin copiar territorios:
  // los punteros entre territorios (vecinos y continentes) siguen apuntando a nodos validos.
  // [IA] Generada con Claude (Prompt 15, Apendice A: "inicializacion segura").
  void intercambiar(TableroRisk &otro);

private:
  // Un tablero no se copia: la copia compartiria punteros a los territorios del original.
  // [IA] Declarados privados y sin implementar con Claude (Prompt 15, Apendice A).
  TableroRisk(const TableroRisk &);
  TableroRisk &operator=(const TableroRisk &);
};

#endif // TABLERORISK_H

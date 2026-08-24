#ifndef TERRITORIO_H
#define TERRITORIO_H

#include <string>
#include "ListaLineal.h"

/*
================================================================================
TAD: Territorio
================================================================================
Descripción:
  Representa una de las 42 regiones geográficas del mapa de Risk.
  Almacena su código único (ej. "1.1"), su nombre oficial ("Alaska"), el ID 
  del continente al que pertenece, el color del jugador que lo controla actualmente,
  la cantidad de unidades de infantería apostadas y una lista lineal de punteros
  hacia sus territorios vecinos adyacentes.

Recomendaciones de Implementación:
  1. En Entrega 1, la adyacencia se modela como un grafo de listas de adyacencias
     mediante 'ListaLineal<Territorio*>'.
  2. Evitar que un territorio se agregue a sí mismo como vecino.
  3. Asegurar que las modificaciones de ejércitos mantengan 'numEjercitos >= 1' 
     durante el transcurso normal de la partida.
================================================================================
*/

class Territorio {
public:
    std::string codigo;                       // Código oficial (ej: "1.1", "2.3")
    std::string nombre;                       // Nombre descriptivo (ej: "Alaska", "Brasil")
    int continenteId;                         // ID del continente (1: Norteamérica, 2: Sudamérica, etc.)
    std::string colorOcupante;                // Color del jugador dueño del territorio
    int numEjercitos;                         // Cantidad de unidades de infantería en el territorio
    ListaLineal<Territorio*> vecinos;         // Lista lineal de punteros a territorios adyacentes

    // Constructores
    Territorio();
    Territorio(const std::string& cod, const std::string& nom, int contId);

    // Métodos de gestión de vecindad
    // Agrega un puntero a un territorio vecino si no está registrado previamente
    bool agregarVecino(Territorio* vecinoPtr);

    // Consulta si este territorio tiene frontera directa con el territorio del código dado
    bool esVecinoDe(const std::string& codigoDestino) const;

    // Métodos de control y tropas
    // Asigna el jugador ocupante inicial y la cantidad de tropas
    void asignarOcupante(const std::string& nuevoColor, int cantidad);

    // Suma o resta tropas al territorio (delta positivo o negativo)
    void modificarEjercitos(int delta);
};

#endif // TERRITORIO_H

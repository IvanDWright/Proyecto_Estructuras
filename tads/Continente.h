#ifndef CONTINENTE_H
#define CONTINENTE_H

#include <string>
#include "ListaLineal.h"
#include "Territorio.h"

/*
================================================================================
TAD: Continente
================================================================================
Se define su id, nombre (apuntador y const por las mismas razones que en Carta.h)
bono de unidades y una lista de territorios (Es una lista de apuntadores a los territorios
que pertenecen a cada continente asi puede capturar todas las modificaciones de los
territorios, ya que no seran copias de cada lo que ahorra memoria). 

Tiene constructor vacio para poder hacer

continente c;

*define los atributos despues*


Valores Reglamentarios de Bonos:
  - América del Sur: 2
  - Australia: 2
  - África: 3
  - América del Norte: 5
  - Europa: 5
  - Asia: 7


================================================================================
*/

class Continente {
public:
    int id;                                   // ID numérico (1: NA, 2: SA, 3: EU, 4: AF, 5: AS, 6: OC)
    std::string nombre;                       // Nombre oficial del continente
    int bonoUnidades;                         // Tropas de bonificación otorgadas
    ListaLineal<Territorio*> territorios;     // Punteros a los territorios del continente

    // Constructores
    Continente();
    Continente(int ID, const std::string& nom, int bono);

    // Agrega un territorio al continente
    void agregarTerritorio(Territorio* t);

    // Verifica si un jugador domina todos los territorios del continente
    bool esControladoTotalmentePor(const std::string& colorJugador) const;
};

#endif // CONTINENTE_H

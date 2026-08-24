#ifndef CARTA_H
#define CARTA_H

#include <string>

/*
=======================================================================================
El tad de carta es algo que se le da a cada jugador cuando conquista un
territorio, pero solamente tiene limite de 1 carta por turno, amenos que elimine
a un jugador, lo cual le da el derecho de quedarse con todas las cartas del
jugador eliminado.

primero hay un enum de tipoTropa, que define las diferentes tropas que la carta
puede tener.

la clase carta como tal, incluye un string de codigoTerritorio y un tipoTropa.
Esto ya que por cada territorio conquistado hay un acarta y se le otorga un tipo
de tropa el constructor por defecto inicializa el codigoTerritorio en "" y el
tipoTropa en INFANTERIA. recibe el tipo de tropa base a un const, el cual es
const y una referencia para evitar que el codigo sea una copia de el dato
original y que no se pueda modificar.
=======================================================================================
*/

enum TipoTropa { INFANTERIA, CABALLERIA, ARTILLERIA, COMODIN };

class Carta {
public:
  std::string codigoTerritorio; // Código del territorio o "COMODIN"
  TipoTropa tipo;               // Tipo de tropa que figura en la carta

  Carta() : codigoTerritorio(""), tipo(INFANTERIA) {}
  Carta(const std::string &cod, TipoTropa t) : codigoTerritorio(cod), tipo(t) {}
};

#endif // CARTA_H

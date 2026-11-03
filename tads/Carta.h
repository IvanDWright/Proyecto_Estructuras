#ifndef CARTA_H
#define CARTA_H

#include <string>

// Enum TipoTropa que define las diferentes tropas que la carta puede tener.
enum TipoTropa { INFANTERIA, CABALLERIA, ARTILLERIA, COMODIN };

class Carta {
public:
  // =========================================================================
  // Atributos de Carta
  // =========================================================================
  
  // Código del territorio (o "COMODIN"). Por cada territorio conquistado hay una carta.
  std::string idAsociado;       
  
  // Tipo de tropa que figura en la carta (Infantería, Caballería, Artillería o Comodín).
  TipoTropa clasificacion;      

  // =========================================================================
  // Constructores
  // =========================================================================

  // Constructor por defecto, inicializa el idAsociado en "" y la clasificacion en INFANTERIA.
  Carta() : idAsociado(""), clasificacion(INFANTERIA) {}

  // Constructor parametrizado. Recibe el tipo de tropa en base a un const
  // y una referencia para evitar copias y modificaciones.
  Carta(const std::string &cod, TipoTropa t) : idAsociado(cod), clasificacion(t) {}
};

#endif // CARTA_H

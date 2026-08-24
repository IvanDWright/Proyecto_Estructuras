#ifndef CARTA_H
#define CARTA_H

#include <string>

/*

*/

enum TipoTropa {
    INFANTERIA,
    CABALLERIA,
    ARTILLERIA,
    COMODIN
};

class Carta {
public:
    std::string codigoTerritorio;             // Código del territorio o "COMODIN"
    TipoTropa tipo;                           // Tipo de tropa que figura en la carta

    Carta() : codigoTerritorio(""), tipo(INFANTERIA) {}
    Carta(const std::string& cod, TipoTropa t) : codigoTerritorio(cod), tipo(t) {}
};

#endif // CARTA_H

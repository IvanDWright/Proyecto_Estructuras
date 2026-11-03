#ifndef CONTINENTE_H
#define CONTINENTE_H

#include <string>
#include "ListaLineal.h"
#include "Territorio.h"

// =========================================================================
// TAD Continente
// =========================================================================

class Continente {
public:
    // =========================================================================
    // Atributos de Continente
    // =========================================================================
    
    int idContinente;                             // ID numérico (1: NA, 2: SA, 3: EU, 4: AF, 5: AS, 6: OC)
    std::string nombreContinente;                 // Nombre oficial del continente
    int tropasAdicionales;                        // Tropas de bonificación otorgadas al conquistar el continente entero
    ListaLineal<Territorio*> territoriosInternos; // Punteros a los territorios del continente, se usa punteros para evitar copias.

    // =========================================================================
    // Constructores
    // =========================================================================
    
    // Constructor vacío por defecto
    Continente();
    
    // Constructor parametrizado con id, nombre y tropas adicionales.
    Continente(int identificador, const std::string& nom, int bono);

    // =========================================================================
    // Métodos Principales
    // =========================================================================

    // Verifica si un jugador domina todos los territorios del continente
    // Retorna true si el color del jugador corresponde a todos los territorios
    bool esControladoTotalmentePor(const std::string& colorJugador) const;

    // Agrega un territorio al continente validando que no esté ya en la lista
    void agregarTerritorio(Territorio* t);

};

#endif // CONTINENTE_H

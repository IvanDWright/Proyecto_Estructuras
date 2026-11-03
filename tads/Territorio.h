#ifndef TERRITORIO_H
#define TERRITORIO_H

#include <string>
#include "ListaLineal.h"

class Territorio {
public:
    // =========================================================================
    // Atributos de Territorio
    // =========================================================================
    
    std::string idTerritorio;                 // Código oficial que identifica el territorio (ej: "1.1", "2.3")
    std::string nombreTerritorio;             // Nombre descriptivo oficial del territorio (ej: "Alaska", "Brasil")
    int idContinente;                         // ID del continente al que pertenece (1: Norteamérica, 2: Sudamérica, etc.)
    std::string colorPropietario;             // Color del jugador que ocupa actualmente el territorio
    int cantidadTropas;                       // Cantidad de unidades de infantería apostadas en el territorio
    ListaLineal<Territorio*> adyacentes;      // Lista lineal de punteros a los territorios vecinos que limitan con este

    // =========================================================================
    // Constructores
    // =========================================================================

    // Constructor por defecto, inicializa los valores en blanco o cero
    Territorio();

    // Constructor parametrizado: recibe el código como referencia constante (para que no sea copia y no sea editable),
    // el nombre (igual que el código) y el ID del continente al que pertenece.
    Territorio(const std::string& idTer, const std::string& nomTer, int idCont);

    // =========================================================================
    // Métodos Principales
    // =========================================================================

    // Asigna el jugador ocupante inicial y la cantidad de tropas que se colocan
    void asignarOcupante(const std::string& nuevoColor, int cantidad);

    // Suma o resta tropas al territorio (delta puede ser positivo o negativo)
    void modificarEjercitos(int delta);

    // Agrega punteros a territorios vecinos que no están registrados previamente.
    // Retorna true si se agregó con éxito.
    bool agregarVecino(Territorio* ptrVecino);

    // Consulta si este territorio tiene frontera directa con el territorio cuyo código es dado.
    bool esVecinoDe(const std::string& idDestino) const;

    // Indica si al menos un vecino pertenece a un jugador de color distinto al dado.
    bool tieneVecinoEnemigo(const std::string& colorJugador) const;

    // Indica si al menos un vecino pertenece al jugador del color dado.
    bool tieneVecinoPropio(const std::string& colorJugador) const;

};

#endif // TERRITORIO_H

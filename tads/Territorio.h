#ifndef TERRITORIO_H
#define TERRITORIO_H

#include <string>
#include "ListaLineal.h"

/*
===========================================================================================
La clase Territorio representa un territorio del tablero de risk, este tiene cun codigo
que identifica este, el nombre oficial de el territorio, el id de el continente al
que pertenece, y la informacion de el ocupante de el territorio, el cual puede cambiar
tiene un numero de cantidad de tropas y apuntadores a los territorios vecinos que es a los 
que puede llamar. El constructor es por defecto vacio, pero existe otro que recibe el codigo
como referencia y constante (igual que en los otros ejemplos es para que no se pueda editar 
y no sea una copia de el dato) el nombre igual que el codigo, y ademas recibe el id del 
continente al que pertenece.
metodos:
agregarVecino: 
Agrega punteros a territorios vecinos que no estan registrados previamente.
esVecinoDe: 
Consulta si este territorio tiene frontera directa con el territorio del código dado
asignarOcupante: 
Asigna el jugador ocupante inicial y la cantidad de tropas
modificarEjercitos: 
Suma o resta tropas al territorio (delta positivo o negativo)
========================================================================================
*/

class Territorio {
public:
    std::string codigo;                       // Código oficial (ej: "1.1", "2.3")
    std::string nombre;                       // Nombre descriptivo (ej: "Alaska", "Brasil")
    int continenteId;                         // ID del continente (1: Norteamérica, 2: Sudamérica, etc.)
    std::string colorOcupante;                // Color del jugador dueño del territorio
    int numEjercitos;                         // Cantidad de unidades de infantería en el territorio
    ListaLineal<Territorio*> vecinos;         // Lista lineal de punteros a territorios adyacentes


    Territorio();
    Territorio(const std::string& cod, const std::string& nom, int contId);

    bool agregarVecino(Territorio* vecinoPtr);

    bool esVecinoDe(const std::string& codigoDestino) const;

    void asignarOcupante(const std::string& nuevoColor, int cantidad);

    void modificarEjercitos(int delta);
};

#endif // TERRITORIO_H

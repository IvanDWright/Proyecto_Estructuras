#ifndef COLAPRIORIDAD_H
#define COLAPRIORIDAD_H

#include "ListaLineal.h"
#include "NodoHuffman.h"

// =========================================================================
// TAD ColaPrioridad
// =========================================================================

class ColaPrioridad {
public:
    // =========================================================================
    // Atributos de ColaPrioridad
    // =========================================================================

    // Lista lineal de punteros a los nodos pendientes por combinar, siempre
    // ordenada de menor a mayor frecuencia (el primero es el de menor frecuencia).
    ListaLineal<NodoHuffman*> elementos;

    // =========================================================================
    // Constructores
    // =========================================================================

    // Constructor por defecto, inicializa la cola vacía.
    ColaPrioridad();

    // =========================================================================
    // Métodos Principales
    // =========================================================================

    // Consulta si la cola no tiene elementos - O(1)
    bool esVacia() const;

    // Retorna la cantidad de nodos que hay en la cola - O(1)
    int tamano() const;

    // Inserta el nodo en la posición que le corresponde según su frecuencia - O(N)
    void insertar(NodoHuffman* nodo);

    // Saca de la cola el nodo de menor frecuencia y lo retorna.
    // Si la cola está vacía retorna NULL - O(1)
    NodoHuffman* extraerMinimo();
};

#endif // COLAPRIORIDAD_H

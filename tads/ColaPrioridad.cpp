#include "ColaPrioridad.h"

// Función de comparación para ordenar por frecuencia; Recibe: NodoHuffman* const &a, NodoHuffman* const &b; Retorna: bool
// Retorna true si el primer nodo tiene menor frecuencia que el segundo.
static bool menorFrecuencia(NodoHuffman* const &a, NodoHuffman* const &b) {
    return a->frecuencia < b->frecuencia;
}

// Constructor por defecto, inicializa la cola vacía; Recibe: nada; Retorna: nada
ColaPrioridad::ColaPrioridad() {}

// Consulta si la cola no tiene elementos; Recibe: nada; Retorna: bool
bool ColaPrioridad::esVacia() const {
    return elementos.estaVacia();
}

// Retorna la cantidad de nodos que hay en la cola; Recibe: nada; Retorna: int
int ColaPrioridad::tamano() const {
    return elementos.longitud();
}

// Inserta un nodo en la cola; Recibe: NodoHuffman* nodo; Retorna: void
// Lo inserta de forma ordenada usando menorFrecuencia como criterio para mantener la cola de menor a mayor.
void ColaPrioridad::insertar(NodoHuffman* nodo) {
    elementos.insertarOrdenado(nodo, menorFrecuencia);
}

// Saca de la cola el nodo de menor frecuencia; Recibe: nada; Retorna: NodoHuffman*
// Como la lista esta ordenada, el minimo es el primero (indice 0). Lo guarda, lo elimina de la lista y lo retorna.
NodoHuffman* ColaPrioridad::extraerMinimo() {
    if (elementos.estaVacia()) {
        return NULL;
    }
    NodoHuffman* minimo = elementos.obtener(0);
    elementos.eliminarEn(0);
    return minimo;
}

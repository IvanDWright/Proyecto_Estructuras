#ifndef NODOHUFFMAN_H
#define NODOHUFFMAN_H

#include <cstddef>

// =========================================================================
// TAD NodoHuffman (Nodo del árbol binario de Huffman)
// =========================================================================

struct NodoHuffman {
    // Caracter (código ASCII) que guarda el nodo. Solo tiene sentido en las hojas.
    unsigned char simbolo;

    // Cantidad de veces que aparece el símbolo en el mensaje. En un nodo interno
    // es la suma de las frecuencias de sus dos hijos.
    unsigned long long frecuencia;

    // Hijo izquierdo (la arista hacia este hijo se etiqueta con '0').
    NodoHuffman* hijoIzq;

    // Hijo derecho (la arista hacia este hijo se etiqueta con '1').
    NodoHuffman* hijoDer;

    // Constructor: recibe el símbolo, la frecuencia y opcionalmente los dos hijos.
    // Por default los hijos son nulos (o sea que el nodo es una hoja).
    NodoHuffman(unsigned char sim, unsigned long long frec,
                NodoHuffman* izq = NULL, NodoHuffman* der = NULL)
        : simbolo(sim), frecuencia(frec), hijoIzq(izq), hijoDer(der) {}

    // Indica si el nodo es una hoja (no tiene hijos), o sea, si guarda un símbolo.
    bool esHoja() const { return hijoIzq == NULL && hijoDer == NULL; }
};

#endif // NODOHUFFMAN_H

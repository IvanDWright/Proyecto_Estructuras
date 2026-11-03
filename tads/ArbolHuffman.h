#ifndef ARBOLHUFFMAN_H
#define ARBOLHUFFMAN_H

#include <string>
#include "NodoHuffman.h"
#include "ColaPrioridad.h"

// =========================================================================
// TAD ArbolHuffman (Estructura Jerárquica: árbol binario)
// =========================================================================

class ArbolHuffman {
public:
    // =========================================================================
    // Atributos de ArbolHuffman
    // =========================================================================

    // Raíz del árbol de codificación (NULL si el árbol está vacío)
    NodoHuffman* raiz;

    // Cantidad de veces que aparece cada caracter ASCII (posiciones 0 a 255)
    unsigned long long tablaFrecuencias[256];

    // Secuencia de '0' y '1' asignada a cada caracter ASCII (posiciones 0 a 255)
    std::string tablaCodigos[256];

    // =========================================================================
    // Constructores y Destructor
    // =========================================================================

    // Inicializa el árbol vacío con todas las frecuencias y códigos en vacío.
    ArbolHuffman();

    // Libera toda la memoria dinámica de los nodos del árbol.
    ~ArbolHuffman();

    // =========================================================================
    // Métodos Principales
    // =========================================================================

    // Consulta si el árbol no tiene nodos - O(1)
    bool esVacio() const;

    // Cuenta cuántas veces aparece cada caracter del texto y lo guarda en tablaFrecuencias - O(N)
    void calcularFrecuencias(const std::string& texto);

    // Asigna la tabla de frecuencias recibida (se usa al leer un archivo comprimido) - O(1)
    void fijarFrecuencias(const unsigned long long nuevasFrecuencias[256]);

    // Retorna cuántos caracteres distintos tienen frecuencia mayor a 0 - O(1)
    int cantidadSimbolos() const;

    // Construye el árbol de Huffman a partir de tablaFrecuencias usando una ColaPrioridad.
    void construirArbol();

    // Recorre el árbol desde la raíz y llena tablaCodigos (izquierda '0', derecha '1').
    void generarCodigos();

    // Recorre el subárbol que empieza en 'nodo' acumulando en 'prefijo' el camino recorrido,
    // al llegar a una hoja guarda el prefijo como código de su símbolo.
    void generarCodigos(NodoHuffman* nodo, const std::string& prefijo);

    // Retorna la secuencia de '0' y '1' que resulta de reemplazar cada caracter del texto por su código.
    std::string codificar(const std::string& texto) const;

    // Recorre el árbol con cada bit ('0' izquierda, '1' derecha) y cada vez que llega a una hoja
    // agrega su símbolo al resultado, hasta completar 'longitud' caracteres. Retorna el texto original.
    std::string decodificar(const std::string& bits, unsigned long long longitud) const;

    // Libera todos los nodos del árbol y deja la raíz en NULL.
    void destruir();

    // Libera los nodos del subárbol que empieza en 'nodo' (recorrido en posorden).
    void destruir(NodoHuffman* nodo);

private:
    // El árbol maneja memoria dinámica, por eso se prohíbe copiarlo.
    ArbolHuffman(const ArbolHuffman& otro);
    ArbolHuffman& operator=(const ArbolHuffman& otro);
};

#endif // ARBOLHUFFMAN_H

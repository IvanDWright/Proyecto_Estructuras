#ifndef LISTALINEAL_H
#define LISTALINEAL_H

#include <iostream>

/*
================================================================================
TAD: ListaLineal<T> / Nodo<T> (Estructura Lineal Dinámica Genérica)
================================================================================
Descripción:
  Estructura de datos lineal genérica basada en nodos enlazados. 
  Cumple con la restricción RNF-01 (Uso exclusivo de estructuras lineales en la Entrega 1).

Recomendaciones de Implementación:
  1. Mantener un puntero 'cabeza' (inicio) y un puntero 'cola' (fin) para permitir 
     inserciones en O(1) con agregarAlFinal().
  2. Implementar un destructor adecuado que llame a limpiar() para liberar todos 
     los nodos dinámicos y evitar fugas de memoria (Memory Leaks).
  3. Para gnu-g++ y templates en C++, separar la definición en ListaLineal.h y 
     la implementación de los métodos en ListaLineal.hxx (incluido al final de este archivo).
================================================================================
*/

template <typename T>
struct Nodo {
    T dato;
    Nodo<T>* siguiente;

    Nodo(const T& valor) : dato(valor), siguiente(NULL) {}
};

template <typename T>
class ListaLineal {
private:
    Nodo<T>* cabeza;
    Nodo<T>* cola;
    int tamano;

public:
    // Constructor: Inicializa la lista vacía
    ListaLineal();

    // Destructor: Libera toda la memoria dinámica de los nodos
    ~ListaLineal();

    // Consulta si la lista no contiene elementos - O(1)
    bool estaVacia() const;

    // Retorna la cantidad de elementos en la lista - O(1)
    int longitud() const;

    // Inserta un elemento al final de la lista - O(1)
    void agregarAlFinal(const T& valor);

    // Retorna una referencia al elemento en la posición 'indice' - O(N)
    T& obtener(int indice);
    const T& obtener(int indice) const;

    // Elimina el elemento en la posición 'indice' y libera memoria - O(N)
    bool eliminarEn(int indice);

    // Libera todos los nodos y resetea cabeza, cola y tamaño a 0 - O(N)
    void limpiar();
};

#include "ListaLineal.hxx"

#endif // LISTALINEAL_H

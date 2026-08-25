#ifndef LISTALINEAL_H
#define LISTALINEAL_H

#include <iostream>

/*
================================================================================
TAD: ListaLineal<T> / Nodo<T> (Estructura Lineal Dinámica Genérica)
================================================================================

La estructura Nodo<T> guarda un tipo de dato dentro de cada nodo.
T dato (crea un dato de el tipo de dato definido por T)
Nodo<T>* siguiente (guarda la posicion de memoria del siguiente nodo)
El constructor de el nodo se hace con (const T& valor) de ahi se saca el valor y 
por default el siguiente Nodo es NULL

Funciones:

ListaLineal();                    -> Incializa la lista vacia 
~ListaLineal();                   -> Libera toda la memoria dinamica de los nodos
estaVacia() const;                -> Consulta si la lista no contiene elementos - O(1)
longitud() const;                 -> Retorna la cantidad de elementos en la lista - O(1)
agregarAlFinal(const T& valor);   -> Inserta un elemento al final de la lista - O(1)
obtener(int indice);              -> Retorna una referencia al elemento en la posición 'indice' - O(N)
obtener(int indice) const;        -> Retorna una referencia constante al elemento en la posición 'indice' - O(N)
eliminarEn(int indice);           -> Elimina el elemento en la posición 'indice' y libera memoria - O(N)
limpiar();                        -> Libera todos los nodos y resetea cabeza, cola y tamaño a 0 - O(N)

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

    ListaLineal();

    ~ListaLineal();

    bool estaVacia() const;

    int longitud() const;

    void agregarAlFinal(const T& valor);

    T& obtener(int indice);
    const T& obtener(int indice) const;

    bool eliminarEn(int indice);

    void limpiar();
};

#include "ListaLineal.hxx"

#endif // LISTALINEAL_H

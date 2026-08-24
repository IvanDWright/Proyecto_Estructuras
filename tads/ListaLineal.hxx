#ifndef LISTALINEAL_HXX
#define LISTALINEAL_HXX

#include "ListaLineal.h"
#include <cstddef>
#include <stdexcept>

/*
================================================================================
Implementación de Métodos de ListaLineal<T>
================================================================================

ListaLineal<T>::ListaLineal();
  es el constructor por defecto; Recibe: nada; Retorna: nada

ListaLineal<T>::~ListaLineal();
  es el destructor por defecto; Libera la memoria utilizada por la lista

ListaLineal<T>::estaVacia() const;
  Consulta si la lista no contiene elementos; Recibe: nada; Retorna: bool

ListaLineal<T>::longitud() const;
  Retorna la cantidad de elementos en la lista; Recibe: nada; Retorna: int

ListaLineal<T>::agregarAlFinal(const T& valor);
  Inserta un elemento al final de la lista; Recibe: const T& valor; Retorna: void
  Lo hace al crear un nuevo nodo con el valor recibido, luego revisa si la lista esta
  vacia, si esta vacia el nodo igual a la cola y a cabeza, si no esta vacia le asigna
  a la cola de la lista la direccion de memoria del nuevo nodo y pone a este mismo como
  la cola
  
ListaLineal<T>::obtener(int indice);
  Retorna una referencia al elemento en la posición 'indice'; Recibe: int indice; Retorna: T&
  Recibe un indice en forma de int, busca esa posicion dentro de la lista, si no existe
  lanza un error de out of range, y si existe retorna una referencia al elemento en esa
  misma posicion. retorna un dato editable.

ListaLineal<T>::obtener(int indice) const;
  Retorna una referencia constante al elemento en la posición 'indice'; Recibe: int indice; Retorna: const T&
  Hace lo mismo que el anterior pero el dato que retorna lo retorna de manera no editable.

ListaLineal<T>::eliminarEn(int indice);
  Elimina el elemento en la posición 'indice' y libera memoria; Recibe: int indice; Retorna: bool
  
ListaLineal<T>::limpiar();
  Libera todos los nodos y resetea cabeza, cola y tamaño a 0; Recibe: nada; Retorna: void

================================================================================
*/

template <typename T>
ListaLineal<T>::ListaLineal() : cabeza(NULL), cola(NULL), tamano(0) {}

template <typename T>
ListaLineal<T>::~ListaLineal() {
    limpiar();
}

template <typename T>
bool ListaLineal<T>::estaVacia() const {
    return tamano == 0;
}

template <typename T>
int ListaLineal<T>::longitud() const {
    return tamano;
}

template <typename T>
void ListaLineal<T>::agregarAlFinal(const T& valor) {
    Nodo<T>* nuevoNodo = new Nodo<T>(valor);
    if (estaVacia()) {
        cabeza = nuevoNodo;
        cola = nuevoNodo;
    } else {
        cola->siguiente = nuevoNodo;
        cola = nuevoNodo;
    }
    tamano++;
}

template <typename T>
T& ListaLineal<T>::obtener(int indice) {
    if (indice < 0 || indice >= tamano) {
        throw std::out_of_range("Indice fuera de rango en ListaLineal");
    }
    Nodo<T>* actual = cabeza;
    for (int i = 0; i < indice; i++) {
        actual = actual->siguiente;
    }
    return actual->dato;
}

template <typename T>
const T& ListaLineal<T>::obtener(int indice) const {
    if (indice < 0 || indice >= tamano) {
        throw std::out_of_range("Indice fuera de rango en ListaLineal");
    }
    Nodo<T>* actual = cabeza;
    for (int i = 0; i < indice; i++) {
        actual = actual->siguiente;
    }
    return actual->dato;
}

template <typename T>
bool ListaLineal<T>::eliminarEn(int indice) {
    if (indice < 0 || indice >= tamano) {
        return false;
    }
    Nodo<T>* aEliminar = NULL;
    if (indice == 0) {
        aEliminar = cabeza;
        cabeza = cabeza->siguiente;
        if (tamano == 1) {
            cola = NULL;
        }
    } else {
        Nodo<T>* anterior = cabeza;
        for (int i = 0; i < indice - 1; i++) {
            anterior = anterior->siguiente;
        }
        aEliminar = anterior->siguiente;
        anterior->siguiente = aEliminar->siguiente;
        if (aEliminar == cola) {
            cola = anterior;
        }
    }
    delete aEliminar;
    tamano--;
    return true;
}

template <typename T>
void ListaLineal<T>::limpiar() {
    Nodo<T>* actual = cabeza;
    while (actual != NULL) {
        Nodo<T>* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    cabeza = NULL;
    cola = NULL;
    tamano = 0;
}

#endif // LISTALINEAL_HXX

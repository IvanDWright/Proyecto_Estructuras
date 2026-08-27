#ifndef LISTALINEAL_HXX
#define LISTALINEAL_HXX

#include "ListaLineal.h"
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
  Inserta un elemento al final de la lista; Recibe: const T& valor; Retorna:
void Lo hace al crear un nuevo nodo con el valor recibido, luego revisa si la
lista esta vacia, si esta vacia el nodo igual a la cola y a cabeza, si no esta
vacia le asigna a la cola de la lista la direccion de memoria del nuevo nodo y
pone a este mismo como la cola

ListaLineal<T>::obtener(int indice);
  Retorna una referencia al elemento en la posición 'indice'; Recibe: int
indice; Retorna: T& Recibe un indice en forma de int, busca esa posicion dentro
de la lista, si no existe lanza un error de out of range, y si existe retorna
una referencia al elemento en esa misma posicion. retorna un dato editable.

ListaLineal<T>::obtener(int indice) const;
  Retorna una referencia constante al elemento en la posición 'indice'; Recibe:
int indice; Retorna: const T& Hace lo mismo que el anterior pero el dato que
retorna lo retorna de manera no editable.

ListaLineal<T>::eliminarEn(int indice);
  Elimina el elemento en la posición 'indice' y libera memoria; Recibe: int
indice; Retorna: bool Esta funcion primero e verifica si este valor entra en el
tamano, si no entra da un retorna false. Si el indice es 0, el apuntador de a
eliminar apunta a cabeza, y el apuntadora cabeza se cambia a cabeza ->
siguiente, despues si aEliminar=cola, se pone el apuntador de cola como null. Si
el indice es mayor a 0, recorre la lista, con un apuntador a el dato anterior,
  cuando el indice iguala a i-1, el dato de aEliminar se iguala al siguiente de
anterior. despues el apontador de siguiente a anterior se iguala al siguiente de
aEliminar. Al final de esta funcion se libera la memoria de aEliminar.

ListaLineal<T>::limpiar();
  Libera todos los nodos y resetea cabeza, cola y tamaño a 0; Recibe: nada;
Retorna: void Recorre la lista, liberando la memoria de cada nodo, hasta que la
lista este vacia.

================================================================================
*/

template <typename T>
ListaLineal<T>::ListaLineal() : cabeza(nullptr), cola(nullptr), tamano(0) {}

template <typename T> ListaLineal<T>::~ListaLineal() { limpiar(); }

template <typename T> bool ListaLineal<T>::estaVacia() const {
  return tamano == 0;
}

template <typename T> int ListaLineal<T>::longitud() const { return tamano; }

template <typename T> void ListaLineal<T>::agregarAlFinal(const T &valor) {
  Nodo<T> *nuevoNodo = new Nodo<T>(valor);
  if (estaVacia()) {
    cabeza = nuevoNodo;
    cola = nuevoNodo;
  } else {
    cola->siguiente = nuevoNodo;
    cola = nuevoNodo;
  }
  tamano++;
}

template <typename T> T &ListaLineal<T>::obtener(int indice) {
  if (indice < 0 || indice >= tamano) {
    throw std::out_of_range("Indice fuera de rango en ListaLineal");
  }
  Nodo<T> *actual = cabeza;
  for (int i = 0; i < indice; i++) {
    actual = actual->siguiente;
  }
  return actual->dato;
}

template <typename T> const T &ListaLineal<T>::obtener(int indice) const {
  if (indice < 0 || indice >= tamano) {
    throw std::out_of_range("Indice fuera de rango en ListaLineal");
  }
  Nodo<T> *actual = cabeza;
  for (int i = 0; i < indice; i++) {
    actual = actual->siguiente;
  }
  return actual->dato;
}

template <typename T> bool ListaLineal<T>::eliminarEn(int indice) {
  if (indice < 0 || indice >= tamano) {
    return false;
  }
  Nodo<T> *aEliminar = nullptr;
  if (indice == 0) {
    aEliminar = cabeza;
    cabeza = cabeza->siguiente;
    if (tamano == 1) {
      cola = nullptr;
    }
  } else {
    Nodo<T> *anterior = cabeza;
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

template <typename T> void ListaLineal<T>::limpiar() {
  // codigo asistido por IA
  Nodo<T> *actual = cabeza;
  while (actual != nullptr) {
    Nodo<T> *siguiente = actual->siguiente;
    delete actual;
    actual = siguiente;
  }
  cabeza = nullptr;
  cola = nullptr;
  tamano = 0;
}

#endif // LISTALINEAL_HXX

#ifndef LISTALINEAL_H
#define LISTALINEAL_H

#include <cstddef>

/*
================================================================================
TAD: ListaLineal<T> / Nodo<T> (Estructura Lineal Dinámica Genérica)
Primero se crea una estructura de un nodo, el cual tiene un tipo de dato,
definido por el dato typename T, asi puede ser cualquier tipo de dato, y en T
dato se guarda la informacion que se contiene en el Nodo y guarda un apuntador
de tipo estructura Nodo<T> con el mismo tipo de dato, este apuntador es el que
hace que la lista sea lineal y dinamica, ya que apunta al siguiente nodo de la
lista. El constructor de la estructura tiene el apuntador en null por default
pero siempre recibe un dato para guardar en el nodo. La clase ListaLineal<T>
contiene solamente 2 apuntadores de Nodo<T> directamente, y tambien incluye el
tamano de esta, lo que le permite recorrer la lista de manera rapida y
eficiente.

funciones:
    ListaLineal(): constructor por defecto, inicializa la lista vacia
    ~ListaLineal(): destructor por defecto, libera toda la memoria dinamica de
los nodos estaVacia(): consulta si la lista no contiene elementos - O(1)
    longitud(): retorna la cantidad de elementos en la lista - O(1)
    agregarAlFinal(const T& valor): inserta un elemento al final de la lista -
O(1) obtener(int indice): retorna una referencia al elemento en la posicion
'indice' - O(N) obtener(int indice) const: retorna una referencia constante al
elemento en la posicion 'indice' - O(N) eliminarEn(int indice): elimina el
elemento en la posicion 'indice' y libera memoria - O(N) limpiar(): libera todos
los nodos y resetea cabeza, cola y tamano a 0 - O(N)
================================================================================
*/

template <typename T> struct Nodo {
  T dato;
  Nodo<T> *siguiente;

  Nodo(const T &valor) : dato(valor), siguiente(nullptr) {}
};

template <typename T> class ListaLineal {
private:
  Nodo<T> *cabeza;
  Nodo<T> *cola;
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
  void agregarAlFinal(const T &valor);

  // Retorna una referencia al elemento en la posición 'indice' - O(N)
  T &obtener(int indice);
  const T &obtener(int indice) const;

  // Elimina el elemento en la posición 'indice' y libera memoria - O(N)
  bool eliminarEn(int indice);

  // Libera todos los nodos y resetea cabeza, cola y tamaño a 0 - O(N)
  void limpiar();
};

#include "ListaLineal.hxx"

#endif // LISTALINEAL_H

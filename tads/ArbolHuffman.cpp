#include "ArbolHuffman.h"

// Inicializa el árbol vacío: raiz nula y todas las frecuencias en 0.
// Las tablas de códigos son strings, por lo que quedan vacías por defecto.
ArbolHuffman::ArbolHuffman() : raiz(NULL) {
  for (int i = 0; i < 256; i++) {
    tablaFrecuencias[i] = 0;
  }
}

// Destructor: libera la memoria de todos los nodos.
ArbolHuffman::~ArbolHuffman() { destruir(); }

// Consulta si el arbol esta vacio; Recibe: nada; Retorna: bool
bool ArbolHuffman::esVacio() const { return raiz == NULL; }

// Cuenta la frecuencia de cada caracter; Recibe: const string& texto; Retorna:
// void Primero pone todas las frecuencias en 0. Luego recorre el texto y por
// cada caracter suma 1 en la posicion que corresponde a su codigo ASCII (se
// convierte a unsigned char para que los caracteres con valor negativo no se
// salgan del arreglo).
void ArbolHuffman::calcularFrecuencias(const std::string &texto) {
  for (int i = 0; i < 256; i++) {
    tablaFrecuencias[i] = 0;
  }
  for (size_t i = 0; i < texto.size(); i++) {
    tablaFrecuencias[(unsigned char)texto[i]]++;
  }
}

// Asigna una tabla de frecuencias ya conocida; Recibe: arreglo de 256
// frecuencias; Retorna: void. Copia posicion por posicion los valores
// recibidos.
void ArbolHuffman::fijarFrecuencias(
    const unsigned long long nuevasFrecuencias[256]) {
  for (int i = 0; i < 256; i++) {
    tablaFrecuencias[i] = nuevasFrecuencias[i];
  }
}

// Cuenta los caracteres distintos; Recibe: nada; Retorna: int
// Recorre la tabla y cuenta las posiciones con frecuencia mayor a 0.
int ArbolHuffman::cantidadSimbolos() const {
  int cantidad = 0;
  for (int i = 0; i < 256; i++) {
    if (tablaFrecuencias[i] > 0) {
      cantidad++;
    }
  }
  return cantidad;
}

// Construye el arbol de Huffman; Recibe: nada; Retorna: void
// 1. Libera el arbol anterior (si habia).
// 2. Crea un nodo hoja por cada caracter con frecuencia mayor a 0 y lo inserta
// en la cola
//    de prioridad (se recorren en orden ASCII, asi el arbol siempre queda igual
//    para las mismas frecuencias, lo cual es necesario para poder decodificar
//    un archivo).
// 3. Mientras la cola tenga mas de 1 nodo: saca los 2 de menor frecuencia, crea
// un nodo
//    padre con la suma de las frecuencias (primero sacado = hijo izquierdo,
//    segundo = hijo derecho) y lo inserta en la cola.
// 4. El nodo que queda en la cola es la raiz. Si no habia ningun caracter el
// arbol queda vacio.
void ArbolHuffman::construirArbol() {
  destruir();
  ColaPrioridad cola;
  for (int i = 0; i < 256; i++) {
    if (tablaFrecuencias[i] > 0) {
      cola.insertar(new NodoHuffman((unsigned char)i, tablaFrecuencias[i]));
    }
  }
  while (cola.tamano() > 1) {
    NodoHuffman *izq = cola.extraerMinimo();
    NodoHuffman *der = cola.extraerMinimo();
    cola.insertar(
        new NodoHuffman(0, izq->frecuencia + der->frecuencia, izq, der));
  }
  raiz = cola.extraerMinimo();
}

// Genera la tabla de codigos; Recibe: nada; Retorna: void
// Limpia la tabla de codigos. Si el arbol esta vacio termina. Si la raiz es una
// hoja (solo hay un caracter distinto) le asigna el codigo "0". En otro caso
// llama al recorrido recursivo.
void ArbolHuffman::generarCodigos() {
  for (int i = 0; i < 256; i++) {
    tablaCodigos[i] = "";
  }
  if (raiz == NULL) {
    return;
  }
  if (raiz->esHoja()) {
    tablaCodigos[raiz->simbolo] = "0";
  } else {
    generarCodigos(raiz, "");
  }
}

// Recorrido recursivo (preorden) para generar los codigos; Recibe: nodo actual
// y el prefijo del camino recorrido; Retorna: void Si el nodo es nulo no hace
// nada. Si es hoja guarda el prefijo como codigo del simbolo. Si no, baja por
// el hijo izquierdo agregando '0' y por el derecho agregando '1'.
void ArbolHuffman::generarCodigos(NodoHuffman *nodo,
                                  const std::string &prefijo) {
  if (nodo == NULL) {
    return;
  }
  if (nodo->esHoja()) {
    tablaCodigos[nodo->simbolo] = prefijo;
  } else {
    generarCodigos(nodo->hijoIzq, prefijo + "0");
    generarCodigos(nodo->hijoDer, prefijo + "1");
  }
}

// Codifica un texto en una secuencia binaria; Recibe: const string& texto; Retorna: string
// Recorre el texto y por cada caracter concatena al resultado el codigo precalculado en tablaCodigos.
std::string ArbolHuffman::codificar(const std::string &texto) const {
  std::string resultado = "";
  for (size_t i = 0; i < texto.size(); i++) {
    resultado += tablaCodigos[(unsigned char)texto[i]];
  }
  return resultado;
}

// Decodifica una secuencia de bits; Recibe: la secuencia de '0' y '1' y la
// cantidad esperada de caracteres; Retorna: el texto original (o uno más corto
// en caso de error si el archivo binario está corrupto o incompleto). Si el
// arbol esta vacio retorna "". Si la raiz es una hoja (un solo caracter
// distinto) el texto es ese caracter repetido 'longitud' veces. En otro caso
// parte de la raiz y por cada bit baja a la izquierda ('0') o derecha ('1');
// cuando llega a una hoja agrega su simbolo y vuelve a la raiz.
std::string ArbolHuffman::decodificar(const std::string &bits,
                                      unsigned long long longitud) const {
  std::string resultado = "";
  if (raiz == NULL) {
    return resultado;
  }
  if (raiz->esHoja()) {
    resultado.assign((size_t)longitud, (char)raiz->simbolo);
    return resultado;
  }
  NodoHuffman *actual = raiz;
  unsigned long long decodificados = 0;
  for (size_t i = 0; i < bits.size() && decodificados < longitud; i++) {
    if (bits[i] == '0') {
      actual = actual->hijoIzq;
    } else {
      actual = actual->hijoDer;
    }
    if (actual == NULL) {
      return resultado;
    }
    if (actual->esHoja()) {
      resultado += (char)actual->simbolo;
      decodificados++;
      actual = raiz;
    }
  }
  return resultado;
}

// Libera todo el arbol; Recibe: nada; Retorna: void
void ArbolHuffman::destruir() {
  destruir(raiz);
  raiz = NULL;
}

// Libera el subarbol de un nodo en posorden (primero los hijos y luego el
// nodo); Recibe: nodo; Retorna: void
void ArbolHuffman::destruir(NodoHuffman *nodo) {
  if (nodo != NULL) {
    destruir(nodo->hijoIzq);
    destruir(nodo->hijoDer);
    delete nodo;
  }
}

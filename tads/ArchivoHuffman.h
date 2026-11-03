#ifndef ARCHIVOHUFFMAN_H
#define ARCHIVOHUFFMAN_H

#include <string>
#include "ArbolHuffman.h"

// =========================================================================
// TAD ArchivoHuffman (lectura/escritura del archivo binario .bin)
// Formato del archivo (según el enunciado):  n c1 f1 ... cn fn w binary_code
//   n  : entero de 2 bytes, cantidad de caracteres diferentes.
//   ci : 1 byte, caracter (código ASCII).
//   fi : entero de 8 bytes, frecuencia del caracter ci.
//   w  : entero de 8 bytes, cantidad de caracteres del archivo original.
//   binary_code: bits codificados agrupados de a 8 (se completa con '0' al final).
// Los enteros se escriben byte a byte, empezando por el byte menos significativo.
// =========================================================================

class ArchivoHuffman {
public:
    // =========================================================================
    // Atributos de ArchivoHuffman
    // =========================================================================

    // Ruta del archivo binario que se va a leer o escribir
    std::string nombreArchivo;

    // Cantidad de caracteres distintos (el 'n' del archivo)
    unsigned short n;

    // Frecuencia de cada caracter ASCII (posiciones 0 a 255), 0 si no aparece.
    // Reemplaza a los pares (ci, fi) del archivo: el caracter es la posición.
    unsigned long long frecuencias[256];

    // Cantidad de caracteres del texto original (el 'w' del archivo)
    unsigned long long w;

    // Bytes del 'binary_code' leídos del archivo (cada caracter del string es un byte)
    std::string codigoBinario;

    // =========================================================================
    // Constructores
    // =========================================================================

    // Recibe la ruta del archivo e inicializa todos los datos en 0.
    ArchivoHuffman(const std::string& nombre);

    // =========================================================================
    // Métodos Principales
    // =========================================================================

    // Escribe el archivo binario con la cabecera (n, ci, fi, w) tomada del árbol y los bits
    // codificados empaquetados en bytes. Retorna true si se pudo escribir.
    bool escribir(const ArbolHuffman& arbol, const std::string& bitsCodificados);

    // Lee la cabecera y los bytes del código binario del archivo, validando el formato.
    // Retorna true si el archivo existe y tiene el formato correcto.
    bool leer();

    // Convierte una secuencia de '0' y '1' en bytes de 8 bits (completando con ceros el último).
    static std::string empaquetarBits(const std::string& bits);

    // Convierte los bytes de codigoBinario en una secuencia de '0' y '1' (8 por cada byte).
    std::string desempaquetarBits() const;

    // Indica si el nombre del archivo termina en ".bin" (archivo comprimido).
    static bool esArchivoComprimido(const std::string& nombre);
};

#endif // ARCHIVOHUFFMAN_H

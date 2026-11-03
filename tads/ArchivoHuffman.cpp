#include "ArchivoHuffman.h"
#include <fstream>

// Escribe un entero sin signo de 'cantidadBytes' bytes en el archivo; Recibe: ofstream& archivo, unsigned long long valor, int cantidadBytes; Retorna: void
// Escribe byte a byte empezando por el byte menos significativo (Little-Endian).
static void escribirEntero(std::ofstream &archivo, unsigned long long valor, int cantidadBytes) {
    for (int i = 0; i < cantidadBytes; i++) {
        archivo.put((char)((valor >> (8 * i)) & 0xFF));
    }
}

// Lee un entero sin signo de 'cantidadBytes' bytes del archivo; Recibe: ifstream& archivo, unsigned long long& valor, int cantidadBytes; Retorna: bool
// Lee byte a byte en el mismo orden que al escribir y retorna true si se pudieron leer todos los bytes.
static bool leerEntero(std::ifstream &archivo, unsigned long long &valor, int cantidadBytes) {
    valor = 0;
    for (int i = 0; i < cantidadBytes; i++) {
        char c;
        if (!archivo.get(c)) {
            return false;
        }
        valor |= ((unsigned long long)(unsigned char)c) << (8 * i);
    }
    return true;
}

// Constructor con la ruta del archivo; Recibe: const string& nombre; Retorna: nada
// Guarda la ruta del archivo e inicializa n, w y las frecuencias en 0.
ArchivoHuffman::ArchivoHuffman(const std::string& nombre)
    : nombreArchivo(nombre), n(0), w(0), codigoBinario("") {
    for (int i = 0; i < 256; i++) {
        frecuencias[i] = 0;
    }
}

// Escribe el archivo binario con el formato de Huffman; Recibe: const ArbolHuffman& arbol, const string& bitsCodificados; Retorna: bool
// 1. Abre el archivo en modo binario.
// 2. Escribe n (2 bytes) y los pares (ci, fi) de cada caracter con frecuencia > 0 (1 + 8 bytes).
// 3. Escribe w (8 bytes) con la suma total de frecuencias.
// 4. Empaqueta los bits codificados en bytes físicos y los escribe.
// 5. Cierra el archivo y retorna si no hubo errores de escritura.
bool ArchivoHuffman::escribir(const ArbolHuffman& arbol, const std::string& bitsCodificados) {
    std::ofstream archivo(nombreArchivo.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
    if (!archivo.is_open()) {
        return false;
    }

    unsigned long long total = 0;
    for (int i = 0; i < 256; i++) {
        total += arbol.tablaFrecuencias[i];
    }

    escribirEntero(archivo, (unsigned long long)arbol.cantidadSimbolos(), 2);
    for (int i = 0; i < 256; i++) {
        if (arbol.tablaFrecuencias[i] > 0) {
            escribirEntero(archivo, (unsigned long long)i, 1);
            escribirEntero(archivo, arbol.tablaFrecuencias[i], 8);
        }
    }
    escribirEntero(archivo, total, 8);

    std::string bytes = empaquetarBits(bitsCodificados);
    archivo.write(bytes.c_str(), (std::streamsize)bytes.size());

    archivo.close();
    return !archivo.fail();
}

// Lee el archivo binario y valida su formato; Recibe: nada; Retorna: bool
// 1. Abre el archivo en modo binario.
// 2. Lee n (debe estar entre 1 y 256) y luego n pares (caracter, frecuencia).
// 3. Lee w (debe ser mayor a 0 e igual a la suma de frecuencias).
// 4. Lee todos los bytes restantes como el codigo binario. Retorna false si algo no coincide.
bool ArchivoHuffman::leer() {
    std::ifstream archivo(nombreArchivo.c_str(), std::ios::in | std::ios::binary);
    if (!archivo.is_open()) {
        return false;
    }

    for (int i = 0; i < 256; i++) {
        frecuencias[i] = 0;
    }
    codigoBinario = "";

    unsigned long long valor = 0;
    if (!leerEntero(archivo, valor, 2) || valor < 1 || valor > 256) {
        return false;
    }
    n = (unsigned short)valor;

    unsigned long long suma = 0;
    for (int i = 0; i < n; i++) {
        unsigned long long caracter = 0;
        unsigned long long frecuencia = 0;
        if (!leerEntero(archivo, caracter, 1) || !leerEntero(archivo, frecuencia, 8)) {
            return false;
        }
        if (frecuencia == 0 || frecuencias[caracter] != 0) {
            return false;
        }
        frecuencias[caracter] = frecuencia;
        suma += frecuencia;
    }

    if (!leerEntero(archivo, w, 8) || w == 0 || w != suma) {
        return false;
    }

    char c;
    while (archivo.get(c)) {
        codigoBinario += c;
    }
    return true;
}

// Empaqueta una cadena de '0' y '1' en bytes reales de 8 bits; Recibe: const string& bits; Retorna: string
// Recorre los bits y arma cada byte; si al final quedan bits sueltos, se completa el ultimo byte con ceros.
std::string ArchivoHuffman::empaquetarBits(const std::string& bits) {
    std::string bytes = "";
    unsigned char actual = 0;
    int cantidad = 0;
    for (size_t i = 0; i < bits.size(); i++) {
        actual = (unsigned char)((actual << 1) | (bits[i] == '1' ? 1 : 0));
        cantidad++;
        if (cantidad == 8) {
            bytes += (char)actual;
            actual = 0;
            cantidad = 0;
        }
    }
    if (cantidad > 0) {
        actual = (unsigned char)(actual << (8 - cantidad));
        bytes += (char)actual;
    }
    return bytes;
}

// Desempaqueta los bytes en una cadena de bits '0' y '1'; Recibe: nada; Retorna: string
// Extrae los 8 bits de cada byte leído del archivo binario, desde el más al menos significativo.
std::string ArchivoHuffman::desempaquetarBits() const {
    std::string bits = "";
    for (size_t i = 0; i < codigoBinario.size(); i++) {
        unsigned char byte = (unsigned char)codigoBinario[i];
        for (int b = 7; b >= 0; b--) {
            bits += ((byte >> b) & 1) ? '1' : '0';
        }
    }
    return bits;
}

// Consulta si el nombre del archivo termina en ".bin"; Recibe: const string& nombre; Retorna: bool
// Compara los ultimos 4 caracteres del nombre (sin distinguir mayusculas/minusculas) con ".bin".
bool ArchivoHuffman::esArchivoComprimido(const std::string& nombre) {
    if (nombre.size() < 4) {
        return false;
    }
    std::string extension = nombre.substr(nombre.size() - 4);
    for (size_t i = 0; i < extension.size(); i++) {
        if (extension[i] >= 'A' && extension[i] <= 'Z') {
            extension[i] = (char)(extension[i] - 'A' + 'a');
        }
    }
    return extension == ".bin";
}

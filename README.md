# Proyecto de Estructuras de Datos - Sistema de Apoyo para Risk

Sistema de apoyo interactivo, por línea de comandos, para el juego de mesa **Risk**. Proyecto de la asignatura Estructuras de Datos (Pontificia Universidad Javeriana, Departamento de Ingeniería de Sistemas).

**Autores:** Ivan David Wright Gaviria y Juan Sebastián Martínez Gómez.

## Estado del proyecto

| Entrega | Contenido | Estado |
|---|---|---|
| Entrega 1 | Componente 1: configuración del juego con estructuras lineales | Completa |
| **Entrega 2** | **Componente 2: almacenamiento de partidas con árbol de Huffman**, y correcciones de la Entrega 1 | **Completa (rama `entrega_2`)** |
| Entrega 3 | Componente 3: estrategias de conquista con grafos | Pendiente |

## Características

### Componente 1: configuración del juego (estructuras lineales)
- **Consola interactiva (REPL):** ciclo de lectura de comandos tras el prompt `$ `.
- **Estructuras lineales propias:** los TADs del dominio usan la plantilla `ListaLineal<T>` (nodos y punteros), sin contenedores de la STL en el modelo del dominio. La STL se limita al análisis de texto de la consola en `main.cpp`.
- **Reglas del juego:** refuerzos (`max(3, territorios / 3)` más bono de continentes), combate con dados, invasión de territorios, fortificación por adyacencia y rotación de turnos.
- **Lógica en los TADs:** en la Entrega 2 la lógica de juego se trasladó de `main.cpp` a los TADs (`JuegoRisk`, `TableroRisk`, `Jugador`, `Territorio`, `Continente`).

### Componente 2: almacenamiento de partidas (árbol de Huffman)
- `guardar <archivo>`: guarda el estado de la partida en texto plano.
- `guardar_comprimido <archivo>`: guarda la partida comprimida con **codificación de Huffman** en un archivo binario `.bin`.
- `inicializar <archivo>`: carga una partida desde un archivo de texto o desde un `.bin` comprimido (si la partida ya fue inicializada, el comando lo avisa y no recarga).
- Nuevos TADs: `NodoHuffman`, `ColaPrioridad` (cola de prioridad propia para construir el árbol), `ArbolHuffman` (construcción, codificación y decodificación) y `ArchivoHuffman` (lectura y escritura del formato binario).
- Nota: en partidas pequeñas el `.bin` puede pesar un poco más que el texto original, porque guarda la tabla de frecuencias para poder decodificar.

### Componente 3: grafos (Entrega 3)
Los comandos `costo_conquista` y `conquista_mas_barata` existen como *stubs*: validan la entrada y muestran un mensaje genérico. Su lógica con grafos se implementará en la Entrega 3.

## Documentación

- [Documento de diseño y plan de pruebas - Entrega 2 (PDF)](docs/Proyecto_Risk_Entrega2.pdf): acta de evaluación de la Entrega 1, diseño de comandos y TADs, esquemáticos de Huffman y del archivo binario, plan de pruebas, decisiones de diseño y registro de prompts de IA.
- [Auditoría Técnica y Documentación - Entrega 1](docs/ENTREGA1_DOCUMENTACION.md)
- Otros documentos en `docs/`: requisitos de la Entrega 1, TADs de la Entrega 1 y documentación de la Entrega 0.

## Compilación y ejecución

El proyecto se compila con GNU C++ (`g++`). Desde la raíz del repositorio:

### 1. Compilar
```bash
g++ -std=c++17 -Wall -Wextra src/main.cpp tads/*.cpp -I tads -o risk_app
```

El código también compila en los modos C++98 y C++11. En Windows se puede usar MinGW-w64.

### 2. Ejecutar

**En Windows:**
```bash
.\risk_app.exe
```

**En Linux / macOS:**
```bash
./risk_app
```

### 3. Ejemplo rápido
```text
$ inicializar data/archivo_inicio.txt
$ estado_juego
$ guardar_comprimido partida.bin
$ salir
```
En una sesión nueva, `inicializar partida.bin` recupera la misma partida.

## Estructura del proyecto

- `src/`: programa principal (`main.cpp`).
- `tads/`: Tipos Abstractos de Datos (`.h`, `.hxx`, `.cpp`), incluidos los de Huffman.
- `data/`: archivos de partida de ejemplo (`.txt`).
- `docs/`: documentación, requisitos y especificaciones.

## Lista de comandos

Dentro de la aplicación (cuando veas el prompt `$ `):

- `ayuda`: muestra la lista de comandos.
- `ayuda <comando>`: muestra la sintaxis y la descripción de un comando.
- `inicializar <archivo>`: inicia la partida cargándola desde un archivo de texto o comprimido (`.bin`).
- `obtener_unidades <nombre_jugador>`: el jugador reclama sus unidades de refuerzo.
- `atacar <nombre_jugador>`: inicia el flujo de ataque contra territorios enemigos.
- `fortificar <nombre_jugador>`: mueve unidades entre territorios propios.
- `estado_juego`: muestra jugadores, turno, territorios y unidades.
- `guardar <nombre_archivo>`: guarda el estado en texto plano.
- `guardar_comprimido <nombre_archivo>`: guarda el estado comprimido con Huffman.
- `costo_conquista <nombre_jugador> <territorio>`: ruta de conquista (Entrega 3, aún sin lógica).
- `conquista_mas_barata <nombre_jugador>`: conquista de menor costo (Entrega 3, aún sin lógica).
- `salir`: termina el programa.

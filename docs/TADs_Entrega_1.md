# Documentación Técnica, Especificación Formal de TADs - Entrega 1

**Asignatura:** Estructuras de Datos  
**Componente:** Componente 1: Configuración del Juego y Estructuras Lineales (Risk)  
**Objetivo:** Documentación formal de ingeniería, tablas de operaciones con análisis de complejidad computacional ($O$), invariantes de representación de cada Tipo Abstracto de Datos (TAD).

---

## 1. Arquitectura General y Justificación de TADs

Para dar cumplimiento estricto a la restricción **RNF-01** (Uso exclusivo de estructuras lineales en la Entrega 1) y permitir un diseño desacoplado, modular y escalable hacia las siguientes entregas, se define la siguiente jerarquía de TADs:

```
+-------------------------------------------------------------------------------+
|                                  JuegoRisk                                    |
| (Controlador / Fachada principal de la partida y ciclo de turnos)             |
+-------------------+---------------------------------------+-------------------+
                    |                                       |
                    v                                       v
      +----------------------------+          +----------------------------+
      |     ListaLineal<Jugador>   |          |        TableroRisk         |
      | (Gestión lineal de turnos) |          | (Topología y Adyacencias)  |
      +-------------+--------------+          +-------------+--------------+
                    |                                       |
                    v                                       v
         +--------------------+                    +------------------+
         |      Jugador       |                    |    Territorio    |
         +----------+---------+                    +--------+---------+
                    |                                       |
                    v                                       v
         +--------------------+                    +--------------------+
         |  ListaLineal<Carta>|                    |ListaLineal<Territorio*>
         +--------------------+                    |  (Lista de Vecinos)|
                                                   +--------------------+
```

---

## 2. Documentación Formal de los TADs

---

### TAD 1: `Nodo<T>` y `ListaLineal<T>` (Estructura Lineal Genérica)

#### A. Propósito y Dominio
Estructura lineal dinámica basada en nodos simplemente enlazados. Almacena de forma secuencial cualquier colección de datos del sistema (`Jugador`, `Territorio`, `Carta`, punteros, cadenas), garantizando operaciones de inserción al final en tiempo constante $O(1)$ gracias al puntero `cola`.

#### B. Estructura y Atributos
*   `cabeza: Puntero a Nodo<T>` $\to$ Apuntador al primer elemento de la lista.
*   `cola: Puntero a Nodo<T>` $\to$ Apuntador al último elemento de la lista.
*   `tamano: Entero` $\to$ Conteo de elementos almacenados ($\ge 0$).

#### C. Invariantes de Representación
*   Si `tamano == 0` $\iff$ `cabeza == NULL` y `cola == NULL`.
*   Si `tamano == 1` $\iff$ `cabeza == cola` y `cabeza != NULL`.
*   Si `tamano > 1` $\iff$ `cabeza != cola`, `cabeza != NULL`, `cola != NULL` y `cola.siguiente == NULL`.
*   `tamano >= 0` en todo momento.

#### D. Tabla de Operaciones y Complejidad Algorítmica

| Operación | Entradas | Salidas | Precondiciones | Postcondiciones | Complejidad Temporal |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `crear()` | Ninguna | Instancia `ListaLineal` | Ninguna | `tamano = 0`, `cabeza = NULL`, `cola = NULL` | $O(1)$ |
| `estaVacia()` | Ninguna | `Booleano` | Lista instanciada | Retorna `tamano == 0` sin modificar la estructura | $O(1)$ |
| `longitud()` | Ninguna | `Entero` | Lista instanciada | Retorna el número de elementos | $O(1)$ |
| `agregarAlFinal(valor: T)` | `valor: T` | Ninguna | Memoria disponible | Elemento insertado al final, `cola` actualizado, `tamano++` | $O(1)$ |
| `obtener(indice: Entero)` | `indice: Entero` | `T&` (Referencia) | $0 \le indice < tamano$ | Retorna referencia al elemento en la posición dada | $O(N)$ |
| `eliminarEn(indice: Entero)` | `indice: Entero` | `Booleano` | $0 \le indice < tamano$ | Nodo liberado de memoria, enlaces reparados, `tamano--` | $O(N)$ |
| `limpiar()` | Ninguna | Ninguna | Ninguna | Libera todos los nodos, `cabeza = NULL`, `cola = NULL`, `tamano = 0` | $O(N)$ |


---

### TAD 2: `Territorio`

#### A. Propósito y Dominio
Modela una unidad geográfica del tablero de Risk. Mantiene su código de identificación, nombre descriptivo, continente al que pertenece, estado actual de ocupación (color del jugador y número de ejércitos) y la lista de adyacencias hacia territorios vecinos mediante una `ListaLineal<Territorio*>`.

#### B. Estructura y Atributos
*   `codigo: Cadena` $\to$ Identificador oficial (ej. "1.1", "2.4").
*   `nombre: Cadena` $\to$ Nombre oficial del territorio (ej. "Alaska", "Brasil").
*   `continenteId: Entero` $\to$ Identificador del continente padre ($1 \le contId \le 6$).
*   `colorOcupante: Cadena` $\to$ Color del jugador que posee el territorio.
*   `numEjercitos: Entero` $\to$ Cantidad de tropas de infantería apostadas.
*   `vecinos: ListaLineal<Territorio*>` $\to$ Lista de referencias a los territorios adyacentes.

#### C. Invariantes de Representación
*   `codigo` es una cadena no vacía y única en el tablero.
*   `numEjercitos >= 1` durante el transcurso normal de la partida (un territorio nunca queda con 0 tropas).
*   `colorOcupante` debe pertenecer al conjunto de colores válidos de los jugadores de la partida.
*   Un territorio no puede contenerse a sí mismo dentro de su lista de `vecinos`.

#### D. Tabla de Operaciones y Complejidad Algorítmica

| Operación | Entradas | Salidas | Precondiciones | Postcondiciones | Complejidad Temporal |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `crear(cod, nom, contId)` | `cod: Cadena`, `nom: Cadena`, `contId: Entero` | Instancia `Territorio` | Datos válidos | Territorio inicializado sin ocupante | $O(1)$ |
| `agregarVecino(vecinoPtr: Territorio*)` | `vecinoPtr: Territorio*` | `Booleano` | `vecinoPtr != NULL` y `vecinoPtr != this` | Puntero agregado a `vecinos` si no existía | $O(V)$ ($V \le 6$) |
| `esVecinoDe(codigoDestino: Cadena)` | `codigoDestino: Cadena` | `Booleano` | Ninguna | Retorna `true` si el código está en `vecinos` | $O(V)$ ($V \le 6$) |
| `asignarOcupante(color: Cadena, cant: Entero)` | `color: Cadena`, `cant: Entero` | Ninguna | `cant >= 1` | `colorOcupante = color`, `numEjercitos = cant` | $O(1)$ |
| `modificarEjercitos(delta: Entero)` | `delta: Entero` | Ninguna | `numEjercitos + delta >= 0` | `numEjercitos += delta` | $O(1)$ |


---

### TAD 3: `Continente`

#### A. Propósito y Dominio
Representa un grupo continental de territorios y define el valor del bono de ejércitos adicionales otorgado al jugador que conquiste y mantenga el control total de todos sus territorios constituyentes.

#### B. Estructura y Atributos
*   `id: Entero` $\to$ ID numérico ($1..6$).
*   `nombre: Cadena` $\to$ Nombre del continente.
*   `bonoUnidades: Entero` $\to$ Tropas adicionales por conquista completa:
    *   América del Sur ($2$), Australia ($2$), África ($3$), América del Norte ($5$), Europa ($5$), Asia ($7$).
*   `territorios: ListaLineal<Territorio*>` $\to$ Lista de referencias a los territorios que integran el continente.

#### C. Invariantes de Representación
*   `bonoUnidades > 0`.
*   `territorios.longitud() >= 4` (cada continente en Risk posee al menos 4 territorios).

#### D. Tabla de Operaciones y Complejidad Algorítmica

| Operación | Entradas | Salidas | Precondiciones | Postcondiciones | Complejidad Temporal |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `crear(id, nom, bono)` | `id: Entero`, `nom: Cadena`, `bono: Entero` | Instancia `Continente` | `bono > 0` | Continente creado con lista de territorios vacía | $O(1)$ |
| `agregarTerritorio(t: Territorio*)` | `t: Territorio*` | Ninguna | `t != NULL` | Territorio enlazado a la lista del continente | $O(1)$ |
| `esControladoTotalmentePor(color: Cadena)` | `color: Cadena` | `Booleano` | `color != ""` | Retorna `true` si TODOS sus territorios tienen `colorOcupante == color` | $O(K)$ ($K \le 12$) |


---

### TAD 4: `Carta` (Tarjeta de Equipamiento)

#### A. Propósito y Dominio
Modela las tarjetas que los jugadores reciben tras conquistar al menos un territorio en su turno. Se utilizan para canjes de tropas adicionales según combinaciones reglamentarias.


---

### TAD 5: `Jugador`

#### A. Propósito y Dominio
Modela a un jugador participante en la partida, su identificación visual (color), nombre, cartas acumuladas y su máquina de estados local para validar el orden secuencial estricto de las fases de su turno.

#### B. Estructura y Atributos
*   `nombre: Cadena` $\to$ Máximo 8 caracteres, sin espacios en blanco.
*   `color: Cadena` $\to$ Color asignado ("rojo", "azul", "verde", "amarillo", "negro", "gris").
*   `cartas: ListaLineal<Carta>` $\to$ Colección de cartas en posesión del jugador.
*   `faseActual: TipoEnumerado FaseTurno` $\to$ `FASE_OBTENER_UNIDADES`, `FASE_ATACAR`, `FASE_FORTIFICAR`, `FASE_FINALIZADA`.
*   `haConquistadoEnEsteTurno: Booleano` $\to$ Bandera de conquista para entrega de tarjeta al finalizar el turno.

#### C. Invariantes de Representación
*   `nombre != ""` y `longitud(nombre) <= 8`.
*   `color != ""`.

#### D. Tabla de Operaciones y Complejidad Algorítmica

| Operación | Entradas | Salidas | Precondiciones | Postcondiciones | Complejidad Temporal |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `crear(nom, col)` | `nom: Cadena`, `col: Cadena` | Instancia `Jugador` | Datos válidos | Inicializado en `FASE_OBTENER_UNIDADES` | $O(1)$ |
| `reiniciarFaseTurno()` | Ninguna | Ninguna | Ninguna | `faseActual = FASE_OBTENER_UNIDADES`, `haConquistado = false` | $O(1)$ |
| `avanzarFase(siguiente: FaseTurno)` | `siguiente: FaseTurno` | Ninguna | Transición válida | Actualiza `faseActual` | $O(1)$ |
| `agregarCarta(nuevaCarta: Carta)` | `nuevaCarta: Carta` | Ninguna | Carta válida | Carta insertada en `cartas` | $O(1)$ |


---

### TAD 6: `TableroRisk`

#### A. Propósito y Dominio
Estructura encargada del grafo implícito de adyacencias geográficas del mapa mundial de Risk. Gestiona los 42 territorios, los 6 continentes y provee métodos de búsqueda lineal, cálculo de bonos y conteo de territorios por jugador.

#### B. Estructura y Atributos
*   `territorios: ListaLineal<Territorio>` $\to$ Lista con los 42 territorios instanciados.
*   `continentes: ListaLineal<Continente>` $\to$ Lista con los 6 continentes.

#### C. Invariantes de Representación
*   Tras `inicializarMapa()`, `territorios.longitud() == 42` y `continentes.longitud() == 6`.
*   Toda relación de vecindad entre dos territorios $A$ y $B$ es simétrica ($A$ es vecino de $B \iff B$ es vecino de $A$).

#### D. Tabla de Operaciones y Complejidad Algorítmica

| Operación | Entradas | Salidas | Precondiciones | Postcondiciones | Complejidad Temporal |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `inicializarMapa()` | Ninguna | Ninguna | Ninguna | Crea los 42 territorios, 6 continentes y todas las adyacencias oficiales | $O(1)$ ($42$ nodos fijos) |
| `conectarBidireccional(c1, c2)` | `c1: Cadena`, `c2: Cadena` | Ninguna | Territorios existen | Enlace agregado mutuamente en ambas listas de vecinos | $O(1)$ |
| `buscarTerritorio(codigo: Cadena)` | `codigo: Cadena` | `Territorio*` | Ninguna | Retorna puntero al territorio o `NULL` si no existe | $O(N)$ ($N = 42$) |
| `contarTerritoriosDe(color: Cadena)` | `color: Cadena` | `Entero` | Ninguna | Retorna cantidad de territorios dominados por el color | $O(N)$ ($N = 42$) |
| `calcularBonoContinentes(color: Cadena)`| `color: Cadena` | `Entero` | Ninguna | Suma los bonos de continentes controlados al 100% | $O(C \times K) = O(42)$ |


---

### TAD 7: `JuegoRisk` (Controlador / Fachada del Sistema)

#### A. Propósito y Dominio
Controlador principal de la partida. Mantiene el estado global (`NO_INICIALIZADO`, `EN_CURSO`, `TERMINADO`), la lista lineal de jugadores, el índice del turno actual, la instancia del tablero y los métodos de apoyo para la gestión y rotación circular de turnos.

#### B. Estructura y Atributos
*   `estado: TipoEnumerado EstadoJuego` $\to$ `NO_INICIALIZADO`, `EN_CURSO`, `TERMINADO`.
*   `jugadores: ListaLineal<Jugador>` $\to$ Lista secuencial de participantes.
*   `indiceJugadorActual: Entero` $\to$ Posición del jugador en turno ($0 \le idx < jugadores.longitud()$).
*   `tablero: TableroRisk` $\to$ Mapa y topología.
*   `ganador: Cadena` $\to$ Nombre del jugador victorioso si la partida finalizó.

#### C. Invariantes de Representación
*   Si `estado == EN_CURSO` $\implies 3 \le jugadores.longitud() \le 6$ y $0 \le indiceJugadorActual < jugadores.longitud()$.
*   Si `estado == NO_INICIALIZADO` $\implies$ ningún comando de juego puede ejecutarse.

#### D. Tabla de Operaciones y Complejidad Algorítmica

| Operación | Entradas | Salidas | Precondiciones | Postcondiciones | Complejidad Temporal |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `crear()` | Ninguna | Instancia `JuegoRisk` | Ninguna | `estado = NO_INICIALIZADO`, `indiceJugadorActual = 0` | $O(1)$ |
| `estaInicializado()` | Ninguna | `Booleano` | Ninguna | Retorna `estado == EN_CURSO` | $O(1)$ |
| `estaTerminado()` | Ninguna | `Booleano` | Ninguna | Retorna `estado == TERMINADO` | $O(1)$ |
| `obtenerJugadorEnTurno()` | Ninguna | `Jugador*` | `estado == EN_CURSO` | Retorna puntero al jugador actual | $O(1)$ |
| `buscarJugadorPorNombre(nom)`| `nom: Cadena` | `Jugador*` | Ninguna | Retorna puntero al jugador si existe o `NULL` | $O(J)$ ($J \le 6$) |
| `pasarAlSiguienteTurno()` | Ninguna | Ninguna | Partida en curso | Rota índice circularmente $((i+1) \pmod J)$ y reinicia fase | $O(1)$ |
| `verificarGanadorGlobal()` | Ninguna | `Booleano` | Partida en curso | Si jugador actual tiene 42 territorios, `estado = TERMINADO` | $O(42)$ |


---

## 3. Matriz de Conectividad Geográfica Oficial (42 Territorios de Risk)

La siguiente tabla describe la topología oficial del mapa implementada en `TableroRisk::inicializarMapa()`:

| Código | Territorio | Continente | Territorios Vecinos Conectados |
| :--- | :--- | :--- | :--- |
| **1.1** | Alaska | Norteamérica | 1.2 (Alberta), 1.6 (Territorio Noroccidental), 5.6 (Kamchatka - Asia) |
| **1.2** | Alberta | Norteamérica | 1.1 (Alaska), 1.6 (Territorio Noroccidental), 1.7 (Ontario), 1.9 (EE.UU. Occidentales) |
| **1.3** | América Central | Norteamérica | 1.4 (EE.UU. Orientales), 1.9 (EE.UU. Occidentales), 2.4 (Venezuela - Sudamérica) |
| **1.4** | EE.UU. Orientales | Norteamérica | 1.3 (América Central), 1.7 (Ontario), 1.8 (Quebec), 1.9 (EE.UU. Occidentales) |
| **1.5** | Groenlandia | Norteamérica | 1.6 (Territorio Noroccidental), 1.7 (Ontario), 1.8 (Quebec), 3.2 (Islandia - Europa) |
| **1.6** | Territorio Noroccidental| Norteamérica | 1.1 (Alaska), 1.2 (Alberta), 1.5 (Groenlandia), 1.7 (Ontario) |
| **1.7** | Ontario | Norteamérica | 1.2 (Alberta), 1.4 (EE.UU. Orientales), 1.5 (Groenlandia), 1.6 (Territorio Noroccidental), 1.8 (Quebec), 1.9 (EE.UU. Occidentales) |
| **1.8** | Quebec | Norteamérica | 1.4 (EE.UU. Orientales), 1.5 (Groenlandia), 1.7 (Ontario) |
| **1.9** | EE.UU. Occidentales | Norteamérica | 1.2 (Alberta), 1.3 (América Central), 1.4 (EE.UU. Orientales), 1.7 (Ontario) |
| **2.1** | Argentina | Sudamérica | 2.2 (Brasil), 2.3 (Perú) |
| **2.2** | Brasil | Sudamérica | 2.1 (Argentina), 2.3 (Perú), 2.4 (Venezuela), 4.5 (África del Norte - África) |
| **2.3** | Perú | Sudamérica | 2.1 (Argentina), 2.2 (Brasil), 2.4 (Venezuela) |
| **2.4** | Venezuela | Sudamérica | 1.3 (América Central), 2.2 (Brasil), 2.3 (Perú) |
| **3.1** | Gran Bretaña | Europa | 3.2 (Islandia), 3.3 (Europa del Norte), 3.4 (Escandinavia), 3.7 (Europa Occidental) |
| **3.2** | Islandia | Europa | 1.5 (Groenlandia), 3.1 (Gran Bretaña), 3.4 (Escandinavia) |
| **3.3** | Europa del Norte | Europa | 3.1 (Gran Bretaña), 3.4 (Escandinavia), 3.5 (Europa del Sur), 3.6 (Ucrania), 3.7 (Europa Occidental) |
| **3.4** | Escandinavia | Europa | 3.1 (Gran Bretaña), 3.2 (Islandia), 3.3 (Europa del Norte), 3.6 (Ucrania) |
| **3.5** | Europa del Sur | Europa | 3.3 (Europa del Norte), 3.6 (Ucrania), 3.7 (Europa Occidental), 4.3 (Egipto), 4.5 (África del Norte), 5.7 (Medio Oriente) |
| **3.6** | Ucrania | Europa | 3.3 (Europa del Norte), 3.4 (Escandinavia), 3.5 (Europa del Sur), 5.1 (Afghanistán), 5.7 (Medio Oriente), 5.11 (Ural) |
| **3.7** | Europa Occidental | Europa | 3.1 (Gran Bretaña), 3.3 (Europa del Norte), 3.5 (Europa del Sur), 4.5 (África del Norte) |
| **4.1** | Congo | África | 4.2 (África Oriental), 4.5 (África del Norte), 4.6 (África del Sur) |
| **4.2** | África Oriental | África | 4.1 (Congo), 4.3 (Egipto), 4.4 (Madagascar), 4.5 (África del Norte), 4.6 (África del Sur), 5.7 (Medio Oriente) |
| **4.3** | Egipto | África | 3.5 (Europa del Sur), 4.2 (África Oriental), 4.5 (África del Norte), 5.7 (Medio Oriente) |
| **4.4** | Madagascar | África | 4.2 (África Oriental), 4.6 (África del Sur) |
| **4.5** | África del Norte | África | 2.2 (Brasil), 3.5 (Europa del Sur), 3.7 (Europa Occidental), 4.1 (Congo), 4.2 (África Oriental), 4.3 (Egipto) |
| **4.6** | África del Sur | África | 4.1 (Congo), 4.2 (África Oriental), 4.4 (Madagascar) |
| **5.1** | Afghanistán | Asia | 3.6 (Ucrania), 5.2 (China), 5.3 (India), 5.7 (Medio Oriente), 5.11 (Ural) |
| **5.2** | China | Asia | 5.1 (Afghanistán), 5.3 (India), 5.8 (Mongolia), 5.9 (Siam), 5.10 (Siberia), 5.11 (Ural) |
| **5.3** | India | Asia | 5.1 (Afghanistán), 5.2 (China), 5.7 (Medio Oriente), 5.9 (Siam) |
| **5.4** | Irkutsk | Asia | 5.6 (Kamchatka), 5.8 (Mongolia), 5.10 (Siberia), 5.12 (Yakutsk) |
| **5.5** | Japón | Asia | 5.6 (Kamchatka), 5.8 (Mongolia) |
| **5.6** | Kamchatka | Asia | 1.1 (Alaska), 5.4 (Irkutsk), 5.5 (Japón), 5.8 (Mongolia), 5.12 (Yakutsk) |
| **5.7** | Medio Oriente | Asia | 3.5 (Europa del Sur), 3.6 (Ucrania), 4.2 (África Oriental), 4.3 (Egipto), 5.1 (Afghanistán), 5.3 (India) |
| **5.8** | Mongolia | Asia | 5.2 (China), 5.4 (Irkutsk), 5.5 (Japón), 5.6 (Kamchatka), 5.10 (Siberia) |
| **5.9** | Siam | Asia | 5.2 (China), 5.3 (India), 6.2 (Indonesia - Australia) |
| **5.10**| Siberia | Asia | 5.2 (China), 5.4 (Irkutsk), 5.8 (Mongolia), 5.11 (Ural), 5.12 (Yakutsk) |
| **5.11**| Ural | Asia | 3.6 (Ucrania), 5.1 (Afghanistán), 5.2 (China), 5.10 (Siberia) |
| **5.12**| Yakutsk | Asia | 5.4 (Irkutsk), 5.6 (Kamchatka), 5.10 (Siberia) |
| **6.1** | Australia Oriental | Australia | 6.3 (Nueva Guinea), 6.4 (Australia Occidental) |
| **6.2** | Indonesia | Australia | 5.9 (Siam), 6.3 (Nueva Guinea), 6.4 (Australia Occidental) |
| **6.3** | Nueva Guinea | Australia | 6.1 (Australia Oriental), 6.2 (Indonesia), 6.4 (Australia Occidental) |
| **6.4** | Australia Occidental| Australia | 6.1 (Australia Oriental), 6.2 (Indonesia), 6.3 (Nueva Guinea) |

---

## 4. Guía de Implementación en C++ (Entrega 1)

1.  **Compatibilidad:** Código compatible con `gnu-g++ >= 4.0.0` utilizando plantillas (*templates*) estándar para la estructura lineal.
2.  **Manejo de Memoria:** Destructores explícitos en `ListaLineal` para prevenir fugas de memoria (*memory leaks*).
3.  **Modularidad:** Implementación de cada TAD en su respectivo par `.h` / `.cpp` (o `.h` / `.hxx` para plantillas genéricas).

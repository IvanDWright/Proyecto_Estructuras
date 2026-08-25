# Documentación y Auditoría Técnica Exhaustiva - Entrega 1 (Risk)

**Asignatura:** Estructuras de Datos
**Autor:** Ingeniero de Software Senior / Docente de Estructuras de Datos
**Fase del Proyecto:** Entrega 1 (Componente 1 - Configuración y Estructuras Lineales)

El presente documento constituye la auditoría técnica y documentación exhaustiva del desarrollo de la primera entrega del proyecto **Risk**. Su propósito es garantizar la trazabilidad del proceso de diseño e implementación, asegurando la adherencia estricta a los requerimientos de ingeniería de software y al rigor algorítmico establecido en el enunciado oficial.

---

## 1. Resumen de Requisitos Críticos de la Entrega 1

La arquitectura del sistema ha sido evaluada frente a los criterios fundamentales de la asignatura. A continuación, se consolida la verificación de los requisitos funcionales y no funcionales críticos:

### Modelo de Dominio y TADs
- **Restricción de Estructuras:** Se exige el uso estricto y exclusivo de estructuras lineales de construcción propia (nodos y punteros), implementadas a través de la plantilla `ListaLineal<T>`.
- **Prohibición de Contenedores STL:** Los contenedores de la Standard Template Library (STL) como `std::vector` o `std::list` están terminantemente prohibidos en el modelo del dominio (TADs principales). Su uso queda relegado única y exclusivamente al parseo temporal de la consola REPL en `main.cpp` (tokenización de *strings* y arreglos auxiliares de interfaz).

### Comandos Requeridos e Interfaz REPL
- **Consola Interactiva (REPL):** Implementación robusta de un ciclo infinito de escucha mediante el *prompt* `$ `.
- **Gestión de Turnos y Juego:** Completitud de la secuencia de turnos obligatoria mediante los comandos: `inicializar` (por defecto y por archivo), `obtener_unidades`, `atacar`, `fortificar`, `estado_juego` y `ayuda`.
- **Preparación de Funciones Futuras:** Inclusión de *stubs* funcionales (validaciones base y mensajes de interfaz) para comandos de entregas posteriores: `guardar`, `guardar_comprimido` (Huffman) y algoritmos de grafos (`costo_conquista`, `conquista_mas_barata`).

### Lógica Formal y Reglas de Juego
- **Refuerzos:** Implementación de la fórmula matemática para calcular infantería adicional: $\max(3, \lfloor \text{Territorios} / 3 \rfloor) + \text{Bono Continentes}$.
- **Mecánicas Aleatorias de Ataque:** Resolución de combates mediante simulación de dados (hasta 3 rojos y 2 blancos), iteración del ataque a voluntad del usuario e invasión de territorios (reducción de tropas defensores a 0).
- **Fortificación y Adyacencia:** Traslado de tropas validando que ambos territorios pertenecen al jugador en turno, manteniendo conectividad topológica en el tablero (grafo no dirigido).
- **Rotación Circular:** Gestión matemática del turno del jugador mediante aritmética modular.

### Gestión de Memoria y Encapsulamiento
- **Destrucción Segura en el Heap:** Implementación correcta de destructores (`~ListaLineal()`, etc.) y uso sistemático de `delete` para evitar *memory leaks*.
- **Encapsulamiento y Abstracción:** Protección del estado interno de los TADs (escondiendo punteros críticos como `cabeza` y `cola`) y delegación de las búsquedas a métodos públicos con complejidad temporal de iteración lineal $O(N)$.

---

## 2. Registro y Evolución de Prompts de Desarrollo (Auditoría Técnica)

La evolución del código ha sido guiada mediante ingeniería de *prompts*, refinando iterativamente los requerimientos iniciales hacia especificaciones técnicas avanzadas de software. 

| Etapa | Prompt Original (Intención) | Versión Optimizada / Refinada (Técnico) | Impacto Técnico en la Arquitectura |
| :--- | :--- | :--- | :--- |
| **1. Auditoría de Requisitos** | *"Revisa el enunciado y dime qué tengo que programar."* | **Precondición:** Carga del PDF oficial.<br>**Acción:** Extraer y formalizar los requisitos de la Entrega 1 en un documento Markdown detallado (RF, RNF, RVE).<br>**Postcondición:** Base de especificaciones estandarizada. | Delimitó el alcance del Sprint 1, aislando las restricciones de estructuras lineales y previniendo la implementación adelantada de árboles (Entrega 2) y grafos complejos (Entrega 3). |
| **2. Diseño TAD `TableroRisk`** | *"Haz la clase tablero y ponle todos los países y vecinos."* | **Acción:** Diseñar `TableroRisk.h` y `TableroRisk.cpp`. Modularizar la creación de los 42 nodos geográficos y configurar el grafo no dirigido implícito usando `ListaLineal<Territorio>`.<br>**Restricción:** Aplicar comentarios formales de autoría y complejidad $O(N)$. | Estableció una fachada central (`TableroRisk`) desacoplada, facilitando búsquedas lineales y cálculos de bonos sin exponer la estructura interna de punteros. |
| **3. Comando `inicializar`** | *"Haz que el programa lea el archivo de texto y empiece el juego."* | **Acción:** Implementar un *parser* transaccional usando `ifstream`. Validar la integridad del estado global antes de instanciar: cantidad de jugadores (3-6), existencia de colores y suma cruzada de tropas totales.<br>**Postcondición:** `juego.estaInicializado() == true`. | Introdujo robustez frente a entradas corruptas. Si el archivo no cumple el esquema relacional, la memoria es limpiada y el estado global permanece seguro (`NO_INICIALIZADO`). |
| **4. Auditoría y Marcaje** | *"Revisa si el código está bien y arréglalo si hay errores."* | **Acción:** Auditar los punteros, referencias mutables y reglas lógicas en `main.cpp` y TADs.<br>**Regla:** Marcar cualquier refactorización explícitamente en el código fuente usando el tag `//Correccion IA: [motivo técnico]`. | Garantizó la transparencia y el rigor académico, evidenciando exactamente qué fallas de memoria o semántica de C++ fueron reparadas por el asistente (ver Sección 3). |
| **5. Revisión Documental** | *"Arregla la ortografía y que todo suene profesional."* | **Acción:** Revisión editorial exhaustiva de salidas de consola, comentarios, nombres de variables (camelCase/PascalCase) y consistencia terminológica en la especificación formal (Markdown). | Aseguró que los mensajes de error coincidan bit a bit (*case-sensitive*) con la rúbrica de evaluación y que la documentación refleje el tono de un ingeniero de software. |

---

## 3. Auditoría de Correcciones en el Código Base

Durante las revisiones de código y simulación de flujos de control, se identificaron y corrigieron anti-patrones clásicos de C++ en la gestión de memoria y el encapsulamiento. A continuación se documentan las correcciones más notables:

### 3.1. Violaciones de Encapsulamiento y Fugas de Memoria
- **Falla original:** En métodos como `Continente::agregarTerritorio`, el código intentaba acceder directamente a variables protegidas de nodos (ej. manipulando directamente un puntero `cabeza` expuesto) o ejecutaba llamadas a `delete actual` intentando vaciar contenedores iterativamente de forma insegura, causando violaciones de segmento (*Segfaults*).
- **Corrección Técnica (`//Correccion IA: Respeto de abstracción y limpieza segura`):** Se blindaron los TADs forzando el uso estricto de las interfaces públicas (ej. `lista.agregarAlFinal()`, `lista.limpiar()`). Se corrigió el destructor genérico para que barra la memoria de la lista secuencialmente liberando el Heap de forma controlada.

### 3.2. Copias por Valor vs. Referencias Mutables
- **Falla original:** Al invocar la obtención del jugador en turno, se devolvía una copia temporal (`Jugador j = juego.obtenerJugadorEnTurno()`). Como resultado, los cambios de estado (e.g., `j.faseActual = FASE_ATACAR`) se perdían en el espacio temporal del Stack, sin afectar la partida real en el Heap.
- **Corrección Técnica (`//Correccion IA: Retorno de puntero a estado mutado`):** Se refactorizó la firma del método para que retorne un puntero/referencia directa al jugador alojado en la estructura lineal: 
  ```cpp
  Jugador* jugadorActual = &(jugadores.obtener(indiceJugadorActual));
  jugadorActual->faseActual = FASE_ATACAR;
  ```

### 3.3. Control Aritmético de la Rotación de Turnos
- **Falla original:** El paso de turno reiniciaba el contador manualmente mediante estructuras `if (indice == max) indice = 0;`, una práctica propensa a desbordes si la lista llegaba a alterarse asincrónicamente o si la evaluación lógica fallaba en estados frontera.
- **Corrección Técnica (`//Correccion IA: Rotación circular por aritmética modular`):** Se implementó la rotación algorítmica del índice empleando el operador de residuo modular circular, garantizando una complejidad de $O(1)$ sin ciclos anidados:
  ```cpp
  void JuegoRisk::pasarAlSiguienteTurno() {
      // Rotación circular perfecta garantizada matemáticamente
      indiceJugadorActual = (indiceJugadorActual + 1) % jugadores.longitud();
      obtenerJugadorEnTurno()->reiniciarFaseTurno();
  }
  ```

---

**Firma de Conformidad:**  
*Equipo Docente / Asesoría de Arquitectura de Software*  
*Proyecto Risk - Entrega 1*

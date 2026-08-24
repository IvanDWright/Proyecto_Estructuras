# User Paths (Flujos de Usuario) - Proyecto Risk

Con base en los requerimientos del documento oficial, la interacción del sistema se rige mediante una consola de comandos (prompt `$`). A continuación, se definen los principales **User Paths** (flujos de usuario) que describen cómo interactúan los jugadores con la aplicación para completar sus objetivos a lo largo del ciclo de vida del juego, detallando las interacciones, validaciones y salidas incorporadas para la **Entrega 1** (Componente 1: Configuración del juego y estructuras lineales).

---

## 1. Flujo de Onboarding e Inicialización de Partida
**Objetivo:** El usuario abre la aplicación y configura una partida nueva o retoma una sesión guardada anteriormente.

*   **Paso 1:** El usuario inicia el programa y recibe el prompt interactivo `$ `.
*   **Paso 2 (Opcional):** El usuario teclea `ayuda` para conocer qué comandos existen, o `ayuda inicializar` para ver los detalles y parámetros del comando de arranque.
*   **Paso 3:** El usuario teclea `inicializar <archivo_inicio.txt>` (o archivo `.bin` en entregas posteriores).
*   **Paso 4 (Validaciones del sistema):**
    *   *Archivo no encontrado / no legible:* `(Archivo erróneo) <archivo_inicio> no se encuentra o no puede leerse.`
    *   *Archivo vacío:* `(Archivo vacío) <archivo_inicio> no contiene información.`
    *   *Formato inválido o inconsistencias en reglas:* `(Archivo sin formato) <archivo_inicio> no contiene información en el formato esperado.` *(Valida: correspondencia de colores entre jugadores y territorios, asignación de los 42 territorios, y suma exacta de unidades iniciales: 3 jug = 35 c/u, 4 jug = 30 c/u, 5 jug = 25 c/u, 6 jug = 20 c/u).*
    *   *Partida previa activa:* `(Juego en curso) El juego ya ha sido inicializado.`
*   **Paso 5 (Éxito):** El sistema carga las entidades en las estructuras lineales de memoria, inicializa el turno en el primer jugador y muestra:  
    `(Comando correcto) El juego se ha inicializado correctamente.`

---

## 2. Flujo del Turno de Juego (Core Gameplay Loop - Entrega 1)
**Objetivo:** Un jugador activo realiza las fases reglamentarias de su turno en orden estricto (Reclutamiento -> Ataque -> Fortificación).

> **Validaciones Generales de Turno (aplican a Fases 1, 2 y 3):**
> *   Si no hay partida cargada: `(Juego no inicializado) Esta partida no ha sido inicializada correctamente.`
> *   Si la partida concluyó: `(Juego terminado) Esta partida ya tuvo un ganador.`
> *   Si el jugador no existe: `(Jugador no válido) El jugador <nombre_jugador> no forma parte de esta partida.`
> *   Si no es el turno del jugador: `(Jugador fuera de turno) No es el turno del jugador <nombre_jugador>.`

---

### Fase 1: Reclutamiento y Asignación de Unidades
*   **Paso 1:** El usuario teclea `obtener_unidades <nombre_jugador>`.
*   **Paso 2 (Cálculo del sistema):** El sistema calcula las unidades a reclamar:
    *   *Territorios ocupados:* $\lfloor \text{territorios} / 3 \rfloor$ (mínimo 3 según reglas estándar).
    *   *Continentes completos:* América del Sur (+2), Australia (+2), África (+3), América del Norte (+5), Europa (+5), Asia (+7).
    *   *Cartas / combinaciones:* Bonificaciones por grupos de cartas intercambiadas (si aplica).
*   **Paso 3 (Interacción de asignación):**
    *   El sistema notifica el total de unidades disponibles.
    *   Pregunta iterativamente al jugador: *"¿En cuál de tus territorios deseas ubicar unidades y qué cantidad?"*.
    *   Valida que el territorio pertenezca al jugador y que no exceda las unidades restantes por ubicar.
*   **Paso 4 (Éxito):** Al ubicar todas las tropas:  
    `(Comando correcto) El jugador <nombre_jugador> ha terminado de reclamar y ubicar sus unidades.`

---

### Fase 2: Combate y Conquista (Ataque)
*   **Paso 1:** El usuario teclea `atacar <nombre_jugador>`.
*   **Paso 2 (Validación de secuencia):** Si el jugador no ha realizado la Fase 1 en este turno:  
    `(Jugador no ha ubicado unidades) El jugador <nombre_jugador> no ha ejecutado el comando obtener_unidades.`
*   **Paso 3 (Sub-flujo interactivo de ataque):**
    1.  El sistema solicita el **territorio atacante** (debe pertenecer al jugador y tener al menos 2 unidades).
    2.  El sistema solicita el **territorio defensor** (debe ser vecino geográfico del atacante y pertenecer a un rival).
    3.  **Lanzamiento de dados:** 
        *   Atacante: hasta 3 dados rojos (según tropas en el origen).
        *   Defensor: hasta 2 dados blancos (según tropas en el destino).
        *   Se ordenan de mayor a menor y se comparan los pares más altos; los empates favorecen al defensor.
    4.  **Resolución de bajas:** El sistema actualiza las tropas en memoria e informa las unidades perdidas por cada bando.
    5.  **Conquista de territorio (si aplica):** Si las tropas del defensor llegan a 0:
        *   El atacante toma el control del territorio (se actualiza el color de ocupación).
        *   Se solicita al atacante cuántas unidades trasladar al nuevo territorio (mínimo las tropas que participaron en el dado ganador, dejando al menos 1 en el origen).
    6.  **Continuación:** El sistema pregunta si desea realizar otro ataque o terminar la fase de ataque.
*   **Paso 4 (Éxito):** Al decidir no atacar más o finalizar los combates:  
    `(Comando correcto) El jugador <nombre_jugador> ha terminado de atacar.`

---

### Fase 3: Fortificación de Posición
*   **Paso 1:** El usuario teclea `fortificar <nombre_jugador>`.
*   **Paso 2 (Validación de secuencia):** Si el jugador no ejecutó la fase de ataque:  
    `(Jugador no ha atacado) El jugador <nombre_jugador> no ha ejecutado el comando atacar.`
*   **Paso 3 (Sub-flujo interactivo de fortificación):**
    *   El sistema solicita el territorio origen (propio, con más de 1 unidad).
    *   El sistema solicita el territorio destino (propio y **vecino directo** del origen).
    *   El usuario indica la cantidad de unidades a trasladar (dejando mínimo 1 unidad en el origen).
    *   *(Nota: Solo se permite un único movimiento de fortificación entre dos territorios por turno).*
*   **Paso 4 (Éxito y cambio de turno):**  
    `(Comando correcto) El jugador <nombre_jugador> ha terminado de fortificar su posición.`  
    *El sistema avanza internamente el turno al siguiente jugador en la estructura lineal.*

---

## 3. Flujo de Información y Estado del Juego
**Objetivo:** Los jugadores consultan en cualquier momento la situación global de la partida.

*   **Consulta de Estado (`estado_juego`):**
    *   El usuario teclea `estado_juego`.
    *   *Validación:* Si la partida no está iniciada: `(Juego no inicializado) Esta partida no ha sido inicializada correctamente.`
    *   *Salida:* El sistema recorre las estructuras lineales e imprime:
        *   Número total de jugadores, nombres y colores.
        *   Identificación del jugador con el turno actual.
        *   Listado completo de los 42 territorios, indicando el código, nombre, color del jugador que lo controla y número de unidades presentes.

---

## 4. Flujo de Planeación y Estrategia (Grafos - Entregas Futuras)
**Objetivo:** El jugador se apoya en algoritmos de grafos (Componente 3) para planear rutas de conquista óptimas.

*   **Costo de Conquista:** `costo_conquista <nombre_jugador> <territorio_objetivo>`  
    *Salida:* `(Comando correcto) Para conquistar el territorio <territorio>, <nombre_jugador> debe atacar desde <territorio_1>, pasando por los territorios <territorio_2>, ..., <territorio_m>. Debe conquistar n unidades de ejército.`
*   **Conquista Más Barata:** `conquista_mas_barata <nombre_jugador>`  
    *Salida:* `(Comando correcto) La conquista más barata es avanzar sobre el territorio <territorio_1> desde el territorio <territorio_2>...`

---

## 5. Flujo de Guardado y Cierre de Sesión
**Objetivo:** Guardar la partida en archivo plano o binario comprimido (Huffman) y cerrar el programa.

*   **Guardado en Texto Plano:** `guardar <nombre_archivo.txt>` -> `(Comando correcto) La partida ha sido guardada correctamente.`
*   **Guardado Comprimido (Huffman - Componente 2):** `guardar_comprimido <nombre_archivo.bin>` -> `(Comando correcto) La partida ha sido codificada y guardada correctamente.`
*   **Cierre del Sistema:** El usuario teclea `salir`. El programa finaliza su ejecución sin mensajes adicionales en pantalla.

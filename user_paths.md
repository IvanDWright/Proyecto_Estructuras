# User Paths (Flujos de Usuario) - Proyecto Risk

Con base en los requerimientos del documento oficial, la interacción del sistema se rige mediante una consola de comandos (prompt `$`). A continuación, se definen los principales **User Paths** (flujos de usuario) que describen cómo interactúan los jugadores con la aplicación para completar sus objetivos a lo largo del ciclo de vida del juego.

---

## 1. Flujo de Onboarding e Inicialización de Partida
**Objetivo:** El usuario abre la aplicación y configura una partida nueva o retoma una sesión guardada anteriormente.

*   **Paso 1:** El usuario inicia el programa y recibe el prompt interactivo `$`.
*   **Paso 2 (Opcional):** El usuario teclea `ayuda` para conocer qué comandos existen, o `ayuda inicializar` para ver los detalles y parámetros del comando de arranque.
*   **Paso 3:** El usuario teclea `inicializar archivo_inicio.txt` (o el nombre del archivo `.bin` comprimido si es una partida guardada).
*   **Paso 4:** El sistema lee el archivo, valida el formato, carga las estructuras lineales y jerárquicas, y retorna el estado de éxito:  
    `(Comando correcto) El juego se ha inicializado correctamente.`

---

## 2. Flujo del Turno de Juego (Core Gameplay Loop)
**Objetivo:** Un jugador activo realiza las fases reglamentarias de su turno (Reclutamiento, Ataque, Fortificación).

*   **Fase 1 - Reclutamiento:**
    *   El usuario teclea `obtener_unidades <nombre_jugador>`.
    *   *Interacción:* El sistema le informa cuántas unidades extra obtuvo (por continentes, territorios o cartas) y le pregunta de forma dinámica en qué territorios desea ubicarlas y en qué cantidad.
    *   *Salida:* `(Comando correcto) El jugador <nombre_jugador> ha terminado de reclamar y ubicar sus unidades.`
*   **Fase 2 - Ataque:**
    *   El usuario teclea `atacar <nombre_jugador>`.
    *   *Interacción:* El sistema pregunta desde qué territorio se desea atacar y cuál es el territorio objetivo. Tras validar, informa el resultado simulado de los dados y la pérdida o ganancia de ejércitos. Este ciclo interactivo se repite paso a paso hasta que el jugador decida detenerse o se quede sin ejércitos atacantes válidos.
    *   *Salida:* `(Comando correcto) El jugador <nombre_jugador> ha terminado de atacar.`
*   **Fase 3 - Fortificación (Opcional):**
    *   El usuario teclea `fortificar <nombre_jugador>`.
    *   *Interacción:* El sistema solicita los territorios vecinos origen y destino, y la cantidad de unidades a trasladar.
    *   *Salida:* `(Comando correcto) El jugador <nombre_jugador> ha terminado de fortificar su posición.`

---

## 3. Flujo de Planeación y Estrategia (Consultas e IA)
**Objetivo:** El jugador obtiene información del estado actual del tablero y se apoya en los algoritmos de grafos (Componente 3) para decidir sus próximos movimientos tácticos.

*   **Consulta Global:**
    *   El usuario teclea `estado_juego`.
    *   *Salida:* El sistema imprime en pantalla un resumen del juego (número de jugadores, colores, turno actual, y lista total de territorios con el color de su dueño y la cantidad de unidades estacionadas).
*   **Planeación de Ataque a un Territorio Específico:**
    *   El usuario teclea `costo_conquista <nombre_jugador> <territorio_objetivo>`.
    *   *Salida:* El sistema calcula en sus grafos y muestra la ruta óptima de ataque:  
        `(Comando correcto) Para conquistar el territorio <territorio_objetivo>, <nombre_jugador> debe atacar desde <territorio_origen>, pasando por los territorios A, B... Debe conquistar n unidades de ejército.`
*   **Búsqueda de la Conquista Más Eficiente (Menor Costo de Ejércitos):**
    *   El usuario teclea `conquista_mas_barata <nombre_jugador>`.
    *   *Salida:* El sistema evalúa todos los territorios posibles y sugiere la cadena de invasión que implique la menor pérdida teórica:  
        `(Comando correcto) La conquista más barata es avanzar sobre el territorio X desde el territorio Y...`

---

## 4. Flujo de Guardado y Cierre de Sesión
**Objetivo:** El jugador decide suspender la partida para continuarla en otra ocasión, salvaguardando el estado completo del tablero de forma eficiente.

*   **Paso 1 (Decidir formato de guardado):**
    *   El usuario decide si usar texto plano o usar el algoritmo de compresión de Huffman (Componente 2).
    *   Teclea `guardar partida.txt` (para texto plano) o `guardar_comprimido partida.bin` (para formato binario codificado).
*   **Paso 2 (Confirmación):**
    *   El sistema escribe la estructura en disco y notifica al usuario:  
        `(Comando correcto) La partida ha sido [codificada y] guardada correctamente.`
*   **Paso 3 (Salir):**
    *   El usuario teclea `salir`.
    *   El programa finaliza su ejecución pacíficamente y devuelve el control a la consola original del sistema operativo.
